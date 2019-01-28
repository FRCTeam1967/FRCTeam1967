#include <cscore.h>
#include <stdexcept>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define CHECK_OP(op) 													\
	do {																\
		(op);															\
		if (m_camera->GetLastStatus() != CS_OK) {						\
			std::string line = TOSTRING(__LINE__);						\
			throw std::runtime_error(__FILE__ ":" + line + " failed");	\
		}																\
	} while(0)

const int INVALID_PORT_ID = -1;

namespace jt {

class UsbCamera {
public:
	UsbCamera(int cameraID, int port = INVALID_PORT_ID)
	{
		m_camera = new cs::UsbCamera("CoprocessorCamera", cameraID);
		if (m_camera->GetLastStatus() != CS_OK) {
			std::cerr << "failed to open: " << m_camera->GetPath() << std::endl;
			throw std::runtime_error("error");
		}

		/* init camera parameters */
		CHECK_OP(m_camera->SetResolution(640, 480));
		CHECK_OP(m_camera->SetExposureAuto());
		CHECK_OP(m_camera->SetExposureManual(1));
		CHECK_OP(m_camera->SetBrightness(10));

		/* bind camera source to sink */
		m_sink = new cs::CvSink("CV Image Grabber");
		if (m_sink->GetLastStatus() != CS_OK) {
			std::cerr << "failed to create CvSink" << std::endl;
			throw std::runtime_error("error");
		}
		m_sink->SetSource(*m_camera);

		/* create raw camera streamer */
		if (port != INVALID_PORT_ID) {
			m_streamer = new cs::MjpegServer("Raw Camera Server", port);
			m_streamer->SetSource(*m_camera);
		}
	};

	uint64_t grabFrame(cv::Mat &inputImage)
	{
		uint64_t frameTime = m_sink->GrabFrame(inputImage);
		if (frameTime == 0) {
			std::cerr << "failed to grab frame" << std::endl;
		}
		return frameTime;
	};

private:
	cs::UsbCamera *m_camera;
	cs::CvSink *m_sink;
	cs::MjpegServer *m_streamer;
};


/*
 * Create a MJPEG streamer from CV source
 */
class CVStreamer {
public:
	CVStreamer(const char *name, int port)
		: m_port(port),
		  m_streamer(name, port)
	{ };

	void setResolutionFPS(int width, int height, int fps)
	{
		char name[32];
		snprintf(name, sizeof(name), "CV Image Source %d", m_port);
		m_source = new cs::CvSource(name, cs::VideoMode::PixelFormat::kMJPEG,
									width, height, fps);

		// connect CV source to MJPEG stream
		m_streamer.SetSource(*m_source);
	};

	void putCVFrame(cv::Mat &image) { m_source->PutFrame(image); };

private:
	int m_port;
	cs::CvSource *m_source;
	cs::MjpegServer m_streamer;
};

}	// namespace jt
