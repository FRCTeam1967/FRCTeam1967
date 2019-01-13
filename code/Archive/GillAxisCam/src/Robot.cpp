#include "WPILib.h"
#include "JankyXboxJoystick.h"

/**
 * Uses AxisCamera class to manually acquire a new image each frame, and annotate the image by drawing
 * a circle on it, and show it on the FRC Dashboard.
 */
class AxisCameraSample : public SampleRobot
{
	//IMAQdxSession session;
	//Image *frame;
	//IMAQdxError imaqError;
	AxisCamera *camera;
	jankyXboxJoystick * stick;

public:
	void RobotInit() override {
	    // create an image

		// open the camera at the IP address assigned. This is the IP address that the camera
		// can be accessed through the web interface.
		camera = new AxisCamera("10.19.67.11");
		stick = new jankyXboxJoystick (0);
		camera->WriteResolution (AxisCamera::kResolution_320x240);
		camera->WriteCompression(30);
		camera->WriteRotation(AxisCamera::kRotation_0);
		camera->WriteMaxFPS(15);
		camera->WriteColorLevel(25);
		camera->WriteBrightness(50);
		camera->WriteWhiteBalance(AxisCamera::kWhiteBalance_Automatic);
		camera->WriteExposureControl(AxisCamera::kExposureControl_Automatic);

	}

	void OperatorControl() override {
		while(IsOperatorControl() && IsEnabled()) {
			/*if (stick->GetButtonLB() == true) {     //Customize the video feed while viewing using the Xbox controller (first line)
						camera->WriteColorLevel(0);
						printf("It got here\n");
						printf("after getbutton\n");
					}
			else if (stick->GetButtonLB() == false) {
				camera->WriteColorLevel(25);
				printf("It ends here\n");
				printf("before getbutton\n");
			}*/ //Customize the video feed while viewing using the Xbox controller (last line)

			Wait(0.05);
		}
	}
};

START_ROBOT_CLASS(AxisCameraSample);

