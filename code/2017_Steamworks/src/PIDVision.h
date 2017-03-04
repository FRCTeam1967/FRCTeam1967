#include "WPILib.h"
#include <thread>

#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include "GripPipeline.h"
#include "jankyTask.h"

/*
 * PIDVision.h
 *
 *  Created on: Feb 25, 2017
 *      Author: Sana
 */

#ifndef SRC_PIDVISION_H_
#define SRC_PIDVISION_H_


class PIDVision : public frc::PIDSource, public frc::PIDOutput, JankyTask {

public:

 PIDVision();
 virtual ~PIDVision();


 bool ReadyToPushGearOut();//if true, stop
 void DriveToPeg();
 void CancelDrivetoPeg();

protected:
double PIDGet();
void PIDWrite(double output);
void Run();

private:
//variables
cv::Mat mat;
grip::GripPipeline gp;
cs::UsbCamera cam0;
cs::CvSink cvSink;
cs::CvSource outputStream;
void StartCapturing();
void StopCapturing();
//not sure about visibility yet (last 2) -leaning to private
void GetDistanceToTape();
void GetPegOffsetFromImageCenter();
void VisionLoop();
void VisionInit();
bool isCapturing;


//run vision in run
};


#endif /* SRC_PIDVISION_H_ */
