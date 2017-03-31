
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
	isReadytoPushGear=false;



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
//og .03
	kP=0.001;

	PID=new PIDController(kP, 0.2 , 0.0, this, this);
	PID->SetInputRange(-(X_RESOLUTION/2), (X_RESOLUTION/2));
	PID->SetOutputRange(-1,1);
	PID->SetSetpoint(0.0);
	 inputCount=0;
	 closeEnough=false;

}
//PIDInit?  joysticks give up access/we give up access (disable PID controller)

void PIDVision::DriveToPeg() {
	if(!isCapturing) {
		StartCapturing();
		PID->Enable();
	}
}

bool PIDVision::ReadyToPushGearOut(){
	return isReadytoPushGear;
}

void PIDVision::CancelDrivetoPeg(){
	StopCapturing();
	PID->Disable();
	driveRobot->Drive(0.0, 0.0);
	isReadytoPushGear=false;
	closeEnough=false;
}
 double PIDVision::PIDGet(){
	int localOffset = gp.getOffset();
	 if(localOffset<=(X_RESOLUTION/2) && localOffset>=-(X_RESOLUTION/2)){
		 return  localOffset;
	 }

	 else {
		 inputCount++;
		 SmartDashboard::PutNumber("# of clipped PID inputs", inputCount);
		 if( localOffset>(X_RESOLUTION/2)) {

			 return X_RESOLUTION/2;

		 }
		 else {

			 return -X_RESOLUTION/2;

		 }
	 }
 }

 void PIDVision::PIDWrite(double output){
int localDistance = gp.getDistance();
//	if (gp.getDistance() >1000) {
//		 driveRobot->Drive(0.0, -output);
//		 isReadytoPushGear=true;
//		 printf("PID Write is done \n ");
//	 }
	 if (localDistance>1000 && closeEnough) {
			 driveRobot->Drive(0.0, -output);
					 isReadytoPushGear=true;
			 SmartDashboard::PutString("Inbound Speed", "0");
		 }
 if(localDistance >30 ){
		//TODO set PID vals for each new speed
		 driveRobot->Drive(-0.3, -output/3);
		 SmartDashboard::PutString("Inbound Speed", "high");
	 }
	 else if(localDistance > 18 ) {
			driveRobot->Drive(-0.3, -output/3); //was -0.3 -changed for testing purposes
			closeEnough=true;
			 SmartDashboard::PutString("Inbound Speed", "med");
		}
	 else if (localDistance<=18 && localDistance>0) {
		 driveRobot->Drive(0.0, -output);
				 isReadytoPushGear=true;
				 printf("PID Write is done \n ");
		 SmartDashboard::PutString("Inbound Speed", "0");
	 }

SmartDashboard::PutNumber("output value PW", output/3);

 }

 void PIDVision::Run(){
	 if (isCapturing){

		 VisionLoop();
		 SmartDashboard::PutNumber("BadDifference", gp.getBadDifference());
		 SmartDashboard::PutNumber("BadOffset", gp.getBadOffset());
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
