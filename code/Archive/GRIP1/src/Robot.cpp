#include <thread>

#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include "WPILib.h"
#include "GripPipeline.h"


#define X_RESOLUTION 320
#define Y_RESOLUTION 240

class Robot: public frc::IterativeRobot
{
private:

	static void VisionThread()
	{
		grip::GripPipeline gp;

		cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture(0);

		cam0.SetResolution(X_RESOLUTION, Y_RESOLUTION);
		cam0.SetExposureManual(30); //the higher exposure allows to see the tape from farther
		cam0.SetBrightness(0);

		cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
		cs::CvSource outputStream = CameraServer::GetInstance()->PutVideo("Lines", 320, 240);

		cv::Mat mat;

		while (true)
		{
			if (cvSink.GrabFrame(mat) == 0)
			{
				outputStream.NotifyError(cvSink.GetError());
				continue;
			}
			gp.Process(mat);
			outputStream.PutFrame(mat);
		}
	}

	void RobotInit()
	{
		std::thread visionThread(VisionThread);
		visionThread.detach();
	}

};

START_ROBOT_CLASS(Robot)
