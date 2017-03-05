
#include "PIDVision.h"

#define X_RESOLUTION 320
#define Y_RESOLUTION 240


/*
 * PIDVision.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: Sana
 */


PIDVision::PIDVision(RobotDrive*drive) {
	VisionInit();
	driveRobot=drive;
	PIDInit();



	//starts the task; always should be at the end of the constructor because anything that's used in run has to be initialized first
	Start();
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
	 isCapturing = false;
}

void PIDVision::PIDInit(){

	kP=0.03;

	PID=new PIDController(kP, 0.0 , 0.0, this, this);
	PID->SetInputRange(-(X_RESOLUTION/2), (X_RESOLUTION/2));
	PID->SetOutputRange(-1,1);
	PID->SetSetpoint(0.0);

}
//PIDInit?  joysticks give up access/we give up access (disable PID controller)

void PIDVision::DriveToPeg() {
	StartCapturing();
//	PID->Enable();
	//TODO enable PID loop
}

bool PIDVision::ReadyToPushGearOut(){
	return false; //placeholder
}

void PIDVision::CancelDrivetoPeg(){
	StopCapturing();
	PID->Disable();
	//TODO disable PID loop
}
 double PIDVision::PIDGet(){
	 return gp.getOffset();

 }

 void PIDVision::PIDWrite(double output){
	if (gp.getDistance() >1000) {
		 driveRobot->Drive(0.0, output);
		 //TODO disable PID Loop??
	 }
	else if(gp.getDistance() >30 ){
		//TODO set PID vals for each new speed
		 driveRobot->Drive(0.5, output);
	 }
	 else if(gp.getDistance() >= 18 ) {
			driveRobot->Drive(0.25, output);
		}
	 else if (gp.getDistance() < 18) {
		 driveRobot->Drive(0.15, output);
	 }


 }
 void PIDVision::Run(){
	 if (isCapturing){

		 VisionLoop();
	 }

 }
 void PIDVision::StartCapturing(){
	 isCapturing=true;
 }
 void PIDVision::StopCapturing(){
	 isCapturing=false;

 }
 double PIDVision:: GetDistanceToTape(){
	 return gp.getDistance();

 }
int PIDVision::GetPegOffsetFromImageCenter(){
	 return gp.getOffset();

 }
bool PIDVision::CapturingVal(){
	 return isCapturing;
}

 void PIDVision::VisionLoop(){
	 		if (cvSink.GrabFrame(mat) == 0)
			{
				outputStream.NotifyError(cvSink.GetError());

			}
			gp.Process(mat);
			outputStream.PutFrame(mat);

 }
