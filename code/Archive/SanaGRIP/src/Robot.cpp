#include <thread>

#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include "WPILib.h"
#include "GripPipeline.h"


/**
 * This is a demo program showing the use of OpenCV to do vision processing. The
 * image is acquired from the USB camera, then a rectangle is put on the image and
 * sent to the dashboard. OpenCV has many methods for different types of
 * processing.
 */
class Robot: public frc::IterativeRobot {
private:

	static void VisionThread() {
		SmartDashboard::PutString("does this loop work", "hello");
		grip::GripPipeline gp;
		//object = new UsbCamera ("cam0");
	   // object = new UsbCamera ("cam1");
		// Get the USB camera from CameraServer

		cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture(0);
	//	cs::UsbCamera cam1 = CameraServer::GetInstance()->StartAutomaticCapture(1);
		SmartDashboard::PutString("now does this work", "yes");



		// Set the resolution
		cam0.SetResolution(320, 240);
		//cam1.SetResolution(320,240);
		SmartDashboard::PutString("what about now (resolutioN)", "yes");

		//while (true) {
			//int count=0;

		// Get a CvSink. This will capture Mats from the Camera
		cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
		//cs::CvSink cvSink1 = CameraServer::GetInstance()->GetVideo("USB Camera 1");
		// Setup a CvSource. This will send images back to the Dashboard
		cs::CvSource outputStream = CameraServer::GetInstance()->
				PutVideo("Rectangle", 320, 240);
		//cs::CvSource outputStream1 = CameraServer::GetInstance()->
						//PutVideo("USB Camera 1", 500, 400);
		SmartDashboard::PutString("now?? (after sink and stream)", "yes");


		// Mats are very memory expensive. Let's reuse this Mat.
		cv::Mat mat;
		//SmartDashboard::PutNumber(name, thing)
		SmartDashboard::PutString("now?? after mat", "yes");

		while (true) {
			// Tell the CvSink to grab a frame from the camera and put it
			// in the source mat.  If there is an error notify the output.
			if (cvSink.GrabFrame(mat) == 0) {
				// Send the output the error.
				outputStream.NotifyError(cvSink.GetError());
				// skip the rest of the current iteration
				continue;
				SmartDashboard::PutString("after continue??", "yes");
			}
			// Put a rectangle on the image
			//rectangle(mat, cv::Point(100, 100), cv::Point(400, 400),
				//	cv::Scalar(255, 255, 255), 5);
			// Give the output stream a new image to display
			gp.Process(mat);
			outputStream.PutFrame(mat);
			//count++;
			//printf("\n count");
			SmartDashboard::PutString("after process is called", "yes");


				}
		}
	//}

	void RobotInit() {
		// We need to run our vision program in a separate Thread.
		// If not, our robot program will not run
		std::thread visionThread(VisionThread);
		visionThread.detach();
		SmartDashboard::PutString("does it go through vision thread?", "yes");
	}

};

START_ROBOT_CLASS(Robot)
