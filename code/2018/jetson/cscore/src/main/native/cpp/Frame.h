/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_FRAME_H_
#define CSCORE_FRAME_H_

#include <atomic>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <llvm/SmallVector.h>
#include <support/mutex.h>

#include "Image.h"
#include "cscore_cpp.h"

namespace cs {

class SourceImpl;

class Frame {
  friend class SourceImpl;

 public:
  typedef uint64_t Time;

 private:
  struct Impl {
    explicit Impl(SourceImpl& source_) : source(source_) {}

    wpi::recursive_mutex mutex;
    std::atomic_int refcount{0};
    Time time{0};
    SourceImpl& source;
    std::string error;
    llvm::SmallVector<Image*, 4> images;
    std::vector<int> compressionParams;
  };

 public:
  Frame() noexcept : m_impl{nullptr} {}

  Frame(SourceImpl& source, llvm::StringRef error, Time time);

  Frame(SourceImpl& source, std::unique_ptr<Image> image, Time time);

  Frame(const Frame& frame) noexcept : m_impl{frame.m_impl} {
    if (m_impl) ++m_impl->refcount;
  }

  Frame(Frame&& other) noexcept : Frame() { swap(*this, other); }

  ~Frame() { DecRef(); }

  Frame& operator=(Frame other) noexcept {
    swap(*this, other);
    return *this;
  }

  explicit operator bool() const { return m_impl && m_impl->error.empty(); }

  friend void swap(Frame& first, Frame& second) noexcept {
    using std::swap;
    swap(first.m_impl, second.m_impl);
  }

  Time GetTime() const { return m_impl ? m_impl->time : 0; }

  llvm::StringRef GetError() const {
    if (!m_impl) return llvm::StringRef{};
    return m_impl->error;
  }

  int GetOriginalWidth() const {
    if (!m_impl) return 0;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    if (m_impl->images.empty()) return 0;
    return m_impl->images[0]->width;
  }

  int GetOriginalHeight() const {
    if (!m_impl) return 0;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    if (m_impl->images.empty()) return 0;
    return m_impl->images[0]->height;
  }

  int GetOriginalPixelFormat() const {
    if (!m_impl) return 0;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    if (m_impl->images.empty()) return 0;
    return m_impl->images[0]->pixelFormat;
  }

  Image* GetExistingImage(size_t i = 0) const {
    if (!m_impl) return nullptr;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    if (i >= m_impl->images.size()) return nullptr;
    return m_impl->images[i];
  }

  Image* GetExistingImage(int width, int height) const {
    if (!m_impl) return nullptr;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    for (auto i : m_impl->images) {
      if (i->Is(width, height)) return i;
    }
    return nullptr;
  }

  Image* GetExistingImage(int width, int height,
                          VideoMode::PixelFormat pixelFormat) const {
    if (!m_impl) return nullptr;
    std::lock_guard<wpi::recursive_mutex> lock(m_impl->mutex);
    for (auto i : m_impl->images) {
      if (i->Is(width, height, pixelFormat)) return i;
    }
    return nullptr;
  }

  Image* GetNearestImage(int width, int height) const;
  Image* GetNearestImage(int width, int height,
                         VideoMode::PixelFormat pixelFormat) const;

  Image* Convert(Image* image, VideoMode::PixelFormat pixelFormat,
                 int jpegQuality = 80);
  Image* ConvertMJPEGToBGR(Image* image);
  Image* ConvertMJPEGToGray(Image* image);
  Image* ConvertYUYVToBGR(Image* image);
  Image* ConvertBGRToRGB565(Image* image);
  Image* ConvertRGB565ToBGR(Image* image);
  Image* ConvertBGRToGray(Image* image);
  Image* ConvertGrayToBGR(Image* image);
  Image* ConvertBGRToMJPEG(Image* image, int quality);
  Image* ConvertGrayToMJPEG(Image* image, int quality);

  Image* GetImage(int width, int height, VideoMode::PixelFormat pixelFormat,
                  int jpegQuality = 80);

  bool GetCv(cv::Mat& image) {
    return GetCv(image, GetOriginalWidth(), GetOriginalHeight());
  }
  bool GetCv(cv::Mat& image, int width, int height);

 private:
  void DecRef() {
    if (m_impl && --(m_impl->refcount) == 0) ReleaseFrame();
  }
  void ReleaseFrame();

  Impl* m_impl;
};

}  // namespace cs

#endif  // CSCORE_FRAME_H_
