/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HttpCameraImpl.h"

#include <llvm/STLExtras.h>
#include <support/timestamp.h>
#include <tcpsockets/TCPConnector.h>

#include "Handle.h"
#include "JpegUtil.h"
#include "Log.h"
#include "Notifier.h"
#include "Telemetry.h"
#include "c_util.h"

using namespace cs;

HttpCameraImpl::HttpCameraImpl(llvm::StringRef name, CS_HttpCameraKind kind)
    : SourceImpl{name}, m_kind{kind} {}

HttpCameraImpl::~HttpCameraImpl() {
  m_active = false;

  // Close file if it's open
  {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    if (m_streamConn) m_streamConn->stream->close();
    if (m_settingsConn) m_settingsConn->stream->close();
  }

  // force wakeup of camera thread in case it's waiting on cv
  m_sinkEnabledCond.notify_one();

  // join camera thread
  if (m_streamThread.joinable()) m_streamThread.join();

  // force wakeup of settings thread
  m_settingsCond.notify_one();

  // join settings thread
  if (m_settingsThread.joinable()) m_settingsThread.join();
}

void HttpCameraImpl::Start() {
  // Kick off the stream and settings threads
  m_streamThread = std::thread(&HttpCameraImpl::StreamThreadMain, this);
  m_settingsThread = std::thread(&HttpCameraImpl::SettingsThreadMain, this);
}

#ifdef __linux__
static inline void DoFdSet(int fd, fd_set* set, int* nfds) {
  if (fd >= 0) {
    FD_SET(fd, set);
    if ((fd + 1) > *nfds) *nfds = fd + 1;
  }
}
#endif

void HttpCameraImpl::StreamThreadMain() {
  while (m_active) {
    SetConnected(false);

    // sleep between retries
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // disconnect if no one is listening
    if (m_numSinksEnabled == 0) {
      std::unique_lock<wpi::mutex> lock(m_mutex);
      if (m_streamConn) m_streamConn->stream->close();
      // Wait for a sink to enable
      m_sinkEnabledCond.wait(
          lock, [=] { return !m_active || m_numSinksEnabled != 0; });
      if (!m_active) return;
    }

    // connect
    llvm::SmallString<64> boundary;
    wpi::HttpConnection* conn = DeviceStreamConnect(boundary);

    if (!m_active) break;

    // keep retrying
    if (!conn) continue;

    // update connected since we're actually connected
    SetConnected(true);

    // stream
    DeviceStream(conn->is, boundary);
  }

  SDEBUG("Camera Thread exiting");
  SetConnected(false);
}

wpi::HttpConnection* HttpCameraImpl::DeviceStreamConnect(
    llvm::SmallVectorImpl<char>& boundary) {
  // Build the request
  wpi::HttpRequest req;
  {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    if (m_locations.empty()) {
      SERROR("locations array is empty!?");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      return nullptr;
    }
    if (m_nextLocation >= m_locations.size()) m_nextLocation = 0;
    req = wpi::HttpRequest{m_locations[m_nextLocation++], m_streamSettings};
    m_streamSettingsUpdated = false;
  }

  // Try to connect
  auto stream = wpi::TCPConnector::connect(req.host.c_str(), req.port,
                                           Logger::GetInstance(), 1);

  if (!m_active || !stream) return nullptr;

  auto connPtr = llvm::make_unique<wpi::HttpConnection>(std::move(stream), 1);
  wpi::HttpConnection* conn = connPtr.get();

  // update m_streamConn
  {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_streamConn = std::move(connPtr);
  }

  std::string warn;
  if (!conn->Handshake(req, &warn)) {
    SWARNING(GetName() << ": " << warn);
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_streamConn = nullptr;
    return nullptr;
  }

  // Parse Content-Type header to get the boundary
  llvm::StringRef mediaType, contentType;
  std::tie(mediaType, contentType) = conn->contentType.str().split(';');
  mediaType = mediaType.trim();
  if (mediaType != "multipart/x-mixed-replace") {
    SWARNING("\"" << req.host << "\": unrecognized Content-Type \"" << mediaType
                  << "\"");
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_streamConn = nullptr;
    return nullptr;
  }

  // media parameters
  boundary.clear();
  while (!contentType.empty()) {
    llvm::StringRef keyvalue;
    std::tie(keyvalue, contentType) = contentType.split(';');
    contentType = contentType.ltrim();
    llvm::StringRef key, value;
    std::tie(key, value) = keyvalue.split('=');
    if (key.trim() == "boundary") {
      value = value.trim().trim('"');  // value may be quoted
      boundary.append(value.begin(), value.end());
    }
  }

  if (boundary.empty()) {
    SWARNING("\"" << req.host
                  << "\": empty multi-part boundary or no Content-Type");
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_streamConn = nullptr;
    return nullptr;
  }

  return conn;
}

void HttpCameraImpl::DeviceStream(wpi::raw_istream& is,
                                  llvm::StringRef boundary) {
  // Stored here so we reuse it from frame to frame
  std::string imageBuf;

  // keep track of number of bad images received; if we receive 3 bad images
  // in a row, we reconnect
  int numErrors = 0;

  // streaming loop
  while (m_active && !is.has_error() && m_numSinksEnabled > 0 &&
         numErrors < 3 && !m_streamSettingsUpdated) {
    if (!FindMultipartBoundary(is, boundary, nullptr)) break;

    // Read the next two characters after the boundary (normally \r\n)
    char eol[2];
    is.read(eol, 2);
    if (!m_active || is.has_error()) break;
    // End-of-stream is indicated with trailing --
    if (eol[0] == '-' && eol[1] == '-') break;

    if (!DeviceStreamFrame(is, imageBuf))
      ++numErrors;
    else
      numErrors = 0;
  }
}

bool HttpCameraImpl::DeviceStreamFrame(wpi::raw_istream& is,
                                       std::string& imageBuf) {
  // Read the headers
  llvm::SmallString<64> contentTypeBuf;
  llvm::SmallString<64> contentLengthBuf;
  if (!ParseHttpHeaders(is, &contentTypeBuf, &contentLengthBuf)) {
    SWARNING("disconnected during headers");
    PutError("disconnected during headers", wpi::Now());
    return false;
  }

  // Check the content type (if present)
  if (!contentTypeBuf.str().empty() &&
      !contentTypeBuf.str().startswith("image/jpeg")) {
    llvm::SmallString<64> errBuf;
    llvm::raw_svector_ostream errMsg{errBuf};
    errMsg << "received unknown Content-Type \"" << contentTypeBuf << "\"";
    SWARNING(errMsg.str());
    PutError(errMsg.str(), wpi::Now());
    return false;
  }

  unsigned int contentLength = 0;
  if (contentLengthBuf.str().getAsInteger(10, contentLength)) {
    // Ugh, no Content-Length?  Read the blocks of the JPEG file.
    int width, height;
    if (!ReadJpeg(is, imageBuf, &width, &height)) {
      SWARNING("did not receive a JPEG image");
      PutError("did not receive a JPEG image", wpi::Now());
      return false;
    }
    PutFrame(VideoMode::PixelFormat::kMJPEG, width, height, imageBuf,
             wpi::Now());
    return true;
  }

  // We know how big it is!  Just get a frame of the right size and read
  // the data directly into it.
  auto image = AllocImage(VideoMode::PixelFormat::kMJPEG, 0, 0, contentLength);
  is.read(image->data(), contentLength);
  if (!m_active || is.has_error()) return false;
  int width, height;
  if (!GetJpegSize(image->str(), &width, &height)) {
    SWARNING("did not receive a JPEG image");
    PutError("did not receive a JPEG image", wpi::Now());
    return false;
  }
  image->width = width;
  image->height = height;
  PutFrame(std::move(image), wpi::Now());
  return true;
}

void HttpCameraImpl::SettingsThreadMain() {
  for (;;) {
    wpi::HttpRequest req;
    {
      std::unique_lock<wpi::mutex> lock(m_mutex);
      m_settingsCond.wait(lock, [=] {
        return !m_active || (m_prefLocation != -1 && !m_settings.empty());
      });
      if (!m_active) break;

      // Build the request
      req = wpi::HttpRequest{m_locations[m_prefLocation], m_settings};
    }

    DeviceSendSettings(req);
  }

  SDEBUG("Settings Thread exiting");
}

void HttpCameraImpl::DeviceSendSettings(wpi::HttpRequest& req) {
  // Try to connect
  auto stream = wpi::TCPConnector::connect(req.host.c_str(), req.port,
                                           Logger::GetInstance(), 1);

  if (!m_active || !stream) return;

  auto connPtr = llvm::make_unique<wpi::HttpConnection>(std::move(stream), 1);
  wpi::HttpConnection* conn = connPtr.get();

  // update m_settingsConn
  {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    m_settingsConn = std::move(connPtr);
  }

  // Just need a handshake as settings are sent via GET parameters
  std::string warn;
  if (!conn->Handshake(req, &warn)) SWARNING(GetName() << ": " << warn);

  conn->stream->close();
}

CS_HttpCameraKind HttpCameraImpl::GetKind() const {
  std::lock_guard<wpi::mutex> lock(m_mutex);
  return m_kind;
}

bool HttpCameraImpl::SetUrls(llvm::ArrayRef<std::string> urls,
                             CS_Status* status) {
  std::vector<wpi::HttpLocation> locations;
  for (const auto& url : urls) {
    bool error = false;
    std::string errorMsg;
    locations.emplace_back(url, &error, &errorMsg);
    if (error) {
      SERROR(GetName() << ": " << errorMsg);
      *status = CS_BAD_URL;
      return false;
    }
  }

  std::lock_guard<wpi::mutex> lock(m_mutex);
  m_locations.swap(locations);
  m_nextLocation = 0;
  m_streamSettingsUpdated = true;
  return true;
}

std::vector<std::string> HttpCameraImpl::GetUrls() const {
  std::lock_guard<wpi::mutex> lock(m_mutex);
  std::vector<std::string> urls;
  for (const auto& loc : m_locations) urls.push_back(loc.url);
  return urls;
}

void HttpCameraImpl::CreateProperty(llvm::StringRef name,
                                    llvm::StringRef httpParam, bool viaSettings,
                                    CS_PropertyKind kind, int minimum,
                                    int maximum, int step, int defaultValue,
                                    int value) const {
  std::lock_guard<wpi::mutex> lock(m_mutex);
  m_propertyData.emplace_back(llvm::make_unique<PropertyData>(
      name, httpParam, viaSettings, kind, minimum, maximum, step, defaultValue,
      value));

  Notifier::GetInstance().NotifySourceProperty(
      *this, CS_SOURCE_PROPERTY_CREATED, name, m_propertyData.size() + 1, kind,
      value, llvm::StringRef{});
}

template <typename T>
void HttpCameraImpl::CreateEnumProperty(
    llvm::StringRef name, llvm::StringRef httpParam, bool viaSettings,
    int defaultValue, int value, std::initializer_list<T> choices) const {
  std::lock_guard<wpi::mutex> lock(m_mutex);
  m_propertyData.emplace_back(llvm::make_unique<PropertyData>(
      name, httpParam, viaSettings, CS_PROP_ENUM, 0, choices.size() - 1, 1,
      defaultValue, value));

  auto& enumChoices = m_propertyData.back()->enumChoices;
  enumChoices.clear();
  for (const auto& choice : choices) enumChoices.emplace_back(choice);

  Notifier::GetInstance().NotifySourceProperty(
      *this, CS_SOURCE_PROPERTY_CREATED, name, m_propertyData.size() + 1,
      CS_PROP_ENUM, value, llvm::StringRef{});
  Notifier::GetInstance().NotifySourceProperty(
      *this, CS_SOURCE_PROPERTY_CHOICES_UPDATED, name,
      m_propertyData.size() + 1, CS_PROP_ENUM, value, llvm::StringRef{});
}

std::unique_ptr<PropertyImpl> HttpCameraImpl::CreateEmptyProperty(
    llvm::StringRef name) const {
  return llvm::make_unique<PropertyData>(name);
}

bool HttpCameraImpl::CacheProperties(CS_Status* status) const {
  std::lock_guard<wpi::mutex> lock(m_mutex);

  // Pretty typical set of video modes
  m_videoModes.clear();
  m_videoModes.emplace_back(VideoMode::kMJPEG, 640, 480, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 320, 240, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 160, 120, 30);

  m_properties_cached = true;
  return true;
}

void HttpCameraImpl::SetProperty(int property, int value, CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetStringProperty(int property, llvm::StringRef value,
                                       CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetBrightness(int brightness, CS_Status* status) {
  // TODO
}

int HttpCameraImpl::GetBrightness(CS_Status* status) const {
  // TODO
  return 0;
}

void HttpCameraImpl::SetWhiteBalanceAuto(CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetWhiteBalanceHoldCurrent(CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetWhiteBalanceManual(int value, CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetExposureAuto(CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetExposureHoldCurrent(CS_Status* status) {
  // TODO
}

void HttpCameraImpl::SetExposureManual(int value, CS_Status* status) {
  // TODO
}

bool HttpCameraImpl::SetVideoMode(const VideoMode& mode, CS_Status* status) {
  if (mode.pixelFormat != VideoMode::kMJPEG) return false;
  std::lock_guard<wpi::mutex> lock(m_mutex);
  m_mode = mode;
  m_streamSettingsUpdated = true;
  return true;
}

void HttpCameraImpl::NumSinksChanged() {
  // ignore
}

void HttpCameraImpl::NumSinksEnabledChanged() {
  m_sinkEnabledCond.notify_one();
}

bool AxisCameraImpl::CacheProperties(CS_Status* status) const {
  CreateProperty("brightness", "ImageSource.I0.Sensor.Brightness", true,
                 CS_PROP_INTEGER, 0, 100, 1, 50, 50);
  CreateEnumProperty("white_balance", "ImageSource.I0.Sensor.WhiteBalance",
                     true, 0, 0,
                     {"auto", "hold", "fixed_outdoor1", "fixed_outdoor2",
                      "fixed_indoor", "fixed_fluor1", "fixed_fluor2"});
  CreateProperty("color_level", "ImageSource.I0.Sensor.ColorLevel", true,
                 CS_PROP_INTEGER, 0, 100, 1, 50, 50);
  CreateEnumProperty("exposure", "ImageSource.I0.Sensor.Exposure", true, 0, 0,
                     {"auto", "hold", "flickerfree50", "flickerfree60"});
  CreateProperty("exposure_priority", "ImageSource.I0.Sensor.ExposurePriority",
                 true, CS_PROP_INTEGER, 0, 100, 1, 50, 50);

  // TODO: get video modes from device
  std::lock_guard<wpi::mutex> lock(m_mutex);
  m_videoModes.clear();
  m_videoModes.emplace_back(VideoMode::kMJPEG, 640, 480, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 480, 360, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 320, 240, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 240, 180, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 176, 144, 30);
  m_videoModes.emplace_back(VideoMode::kMJPEG, 160, 120, 30);

  m_properties_cached = true;
  return true;
}

namespace cs {

CS_Source CreateHttpCamera(llvm::StringRef name, llvm::StringRef url,
                           CS_HttpCameraKind kind, CS_Status* status) {
  std::shared_ptr<HttpCameraImpl> source;
  switch (kind) {
    case CS_HTTP_AXIS:
      source = std::make_shared<AxisCameraImpl>(name);
      break;
    default:
      source = std::make_shared<HttpCameraImpl>(name, kind);
      break;
  }
  std::string urlCopy{url};
  if (!source->SetUrls(urlCopy, status)) return 0;
  auto handle = Sources::GetInstance().Allocate(CS_SOURCE_HTTP, source);
  auto& notifier = Notifier::GetInstance();
  notifier.NotifySource(name, handle, CS_SOURCE_CREATED);
  source->Start();
  return handle;
}

CS_Source CreateHttpCamera(llvm::StringRef name,
                           llvm::ArrayRef<std::string> urls,
                           CS_HttpCameraKind kind, CS_Status* status) {
  if (urls.empty()) {
    *status = CS_EMPTY_VALUE;
    return 0;
  }
  auto source = std::make_shared<HttpCameraImpl>(name, kind);
  if (!source->SetUrls(urls, status)) return 0;
  auto handle = Sources::GetInstance().Allocate(CS_SOURCE_HTTP, source);
  auto& notifier = Notifier::GetInstance();
  notifier.NotifySource(name, handle, CS_SOURCE_CREATED);
  source->Start();
  return handle;
}

CS_HttpCameraKind GetHttpCameraKind(CS_Source source, CS_Status* status) {
  auto data = Sources::GetInstance().Get(source);
  if (!data || data->kind != CS_SOURCE_HTTP) {
    *status = CS_INVALID_HANDLE;
    return CS_HTTP_UNKNOWN;
  }
  return static_cast<HttpCameraImpl&>(*data->source).GetKind();
}

void SetHttpCameraUrls(CS_Source source, llvm::ArrayRef<std::string> urls,
                       CS_Status* status) {
  if (urls.empty()) {
    *status = CS_EMPTY_VALUE;
    return;
  }
  auto data = Sources::GetInstance().Get(source);
  if (!data || data->kind != CS_SOURCE_HTTP) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  static_cast<HttpCameraImpl&>(*data->source).SetUrls(urls, status);
}

std::vector<std::string> GetHttpCameraUrls(CS_Source source,
                                           CS_Status* status) {
  auto data = Sources::GetInstance().Get(source);
  if (!data || data->kind != CS_SOURCE_HTTP) {
    *status = CS_INVALID_HANDLE;
    return std::vector<std::string>{};
  }
  return static_cast<HttpCameraImpl&>(*data->source).GetUrls();
}

}  // namespace cs

extern "C" {

CS_Source CS_CreateHttpCamera(const char* name, const char* url,
                              CS_HttpCameraKind kind, CS_Status* status) {
  return cs::CreateHttpCamera(name, url, kind, status);
}

CS_Source CS_CreateHttpCameraMulti(const char* name, const char** urls,
                                   int count, CS_HttpCameraKind kind,
                                   CS_Status* status) {
  llvm::SmallVector<std::string, 4> vec;
  vec.reserve(count);
  for (int i = 0; i < count; ++i) vec.push_back(urls[i]);
  return cs::CreateHttpCamera(name, vec, kind, status);
}

CS_HttpCameraKind CS_GetHttpCameraKind(CS_Source source, CS_Status* status) {
  return cs::GetHttpCameraKind(source, status);
}

void CS_SetHttpCameraUrls(CS_Source source, const char** urls, int count,
                          CS_Status* status) {
  llvm::SmallVector<std::string, 4> vec;
  vec.reserve(count);
  for (int i = 0; i < count; ++i) vec.push_back(urls[i]);
  cs::SetHttpCameraUrls(source, vec, status);
}

char** CS_GetHttpCameraUrls(CS_Source source, int* count, CS_Status* status) {
  auto urls = cs::GetHttpCameraUrls(source, status);
  char** out = static_cast<char**>(std::malloc(urls.size() * sizeof(char*)));
  *count = urls.size();
  for (size_t i = 0; i < urls.size(); ++i) out[i] = cs::ConvertToC(urls[i]);
  return out;
}

void CS_FreeHttpCameraUrls(char** urls, int count) {
  if (!urls) return;
  for (int i = 0; i < count; ++i) std::free(urls[i]);
  std::free(urls);
}

}  // extern "C"
