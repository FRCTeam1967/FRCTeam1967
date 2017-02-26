
#include "PIDVision.h"

#define X_RESOLUTION 320
#define Y_RESOLUTION 240


/*
 * PIDVision.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: Sana
 */


PIDVision::PIDVision() {
	VisionInit();



}

PIDVision::~PIDVision(void) {

}

//////////////

void PIDVision::VisionInit(){

	cam0= CameraServer::GetInstance()->StartAutomaticCapture(0);

	cam0.SetResolution(X_RESOLUTION, Y_RESOLUTION);
	cam0.SetExposureManual(30); //the higher exposure allows to see the tape from farther
	// TODO: don't hardcode the value for resolution
	cam0.SetBrightness(0);

	 cvSink = CameraServer::GetInstance()->GetVideo();
	 outputStream = CameraServer::GetInstance()->PutVideo("Lines", 320, 240);


}


void PIDVision::DriveToPeg() {

}
bool PIDVision::ReadyToPushGearOut(){
	return false; //placeholder
}

void PIDVision::CancelDrivetoPeg(){

}
 double PIDVision::PIDGet(){
	 return 1.0; //placeholder
 }

 void PIDVision::PIDWrite(double output){

 }
 void PIDVision::Run(){

  VisionLoop();


 }
 void PIDVision::StartCapturing(){

 }
 void PIDVision::StopCapturing(){

 }
 void PIDVision:: GetDistanceToTape(){

 }
 void PIDVision::GetPegOffsetFromImageCenter(){

 }
 void PIDVision::VisionLoop(){
		if (cvSink.GrabFrame(mat) == 0)
			{
				outputStream.NotifyError(cvSink.GetError());

			}
			gp.Process(mat);
			outputStream.PutFrame(mat);

 }
