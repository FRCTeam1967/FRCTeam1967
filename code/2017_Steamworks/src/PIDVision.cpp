
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
//	printf("PIDVision Constructor\n");



	//starts the task; always should be at the end of the constructor because anything that's used in run has to be initialized first
	Start();
}

PIDVision::~PIDVision(void) {

}

//////////////

void PIDVision::VisionInit(){
//	printf("VisionInit start\n");
	cam0= CameraServer::GetInstance()->StartAutomaticCapture(0);


	cam0.SetResolution(X_RESOLUTION, Y_RESOLUTION);
	cam0.SetExposureManual(-100); //the higher exposure allows to see the tape from farther
	// TODO: don't hardcode the value for resolution
	cam0.SetBrightness(-20);

	 cvSink = CameraServer::GetInstance()->GetVideo();
	 // outputStream = CameraServer::GetInstance()->PutVideo("Lines", 320, 240);
	 outputStream = CameraServer::GetInstance()->PutVideo("Lines", X_RESOLUTION, Y_RESOLUTION);
	 isCapturing = false;

//	 printf("VisionInit done\n");
}

void PIDVision::PIDInit(){
//og .03
	kP=2.0;
//	kP=-0.05; // sana original

//	printf("PIDInit start\n");
	PID=new PIDController(kP, 0.0 , 0.0, this, this);
	PID->SetInputRange(-(X_RESOLUTION/2), (X_RESOLUTION/2));
	PID->SetOutputRange(-1,1);
	PID->SetSetpoint(0.0);
	 inputCount=0;
	 closeEnough=false;
//	 printf("PIDInit end\n");

}
//PIDInit?  joysticks give up access/we give up access (disable PID controller)

void PIDVision::DriveToPeg() {
//	printf("DriveToPeg start\n");
	if(!isCapturing) {
//		printf("DriveToPeg start capturing\n");
		StartCapturing();
		PID->Enable();
	}
//	printf("DriveToPeg end\n");
}

bool PIDVision::ReadyToPushGearOut(){
//	printf("ReadyToPushGearOut called\n");
	return isReadytoPushGear;
}

void PIDVision::CancelDrivetoPeg(){
//	printf("CancelDriveToPeg start\n");
	StopCapturing();
	PID->Disable();
	driveRobot->Drive(0.0, 0.0);
	isReadytoPushGear=false;
	closeEnough=false;
//	printf("CancelDriveToPeg end\n");

}
 double PIDVision::PIDGet(){
//	printf("PIDGet start\n");

	int localOffset = gp.getOffset();
	 if(localOffset<=(X_RESOLUTION/2) && localOffset>=-(X_RESOLUTION/2)){
		 return  localOffset;
	 }

	 else {
		 inputCount++;
		 SmartDashboard::PutNumber("# of clipped PID inputs", inputCount);
		 if( localOffset>(X_RESOLUTION/2)) {
//				printf("PIDGet if end\n");

			 return X_RESOLUTION/2;

		 }
		 else {
//				printf("PIDGet else end\n");

			 return -X_RESOLUTION/2;

		 }
	 }
 }

 void PIDVision::PIDWrite(double output){
int localDistance = gp.getDistance();
//printf("PIDWrite start\n");
//printf("Localdistance %d", localDistance);

//	if (gp.getDistance() >1000) {
//		 driveRobot->Drive(0.0, -output);
//		 isReadytoPushGear=true;
//		 printf("PID Write is done \n ");
//	 }
	 if (localDistance>1000 && closeEnough) {	// if localDistance too close (to see distance) but close enough, then
//		 driveRobot->Drive(0.0, -output);
		 isReadytoPushGear=true;
//		 printf("is close enough? >1000 %d \n",closeEnough );
/*		 printf("local distance > 1000\n");
		 printf("magnitude: %f \n",-output );*/
//			 SmartDashboard::PutString("Inbound Speed", "0");
		 }
	 else if(localDistance > 30){
		//TODO set PID vals for each new speed
//		 driveRobot->Drive(-0.3, -output/3);
/*		 printf("magnitude: %f \n",-output/3 );
		 printf("local distance > 30\n");*/
//		 printf("is close enough? >30 %d \n",closeEnough );
//		 SmartDashboard::PutString("Inbound Speed", "high");
	 }
	 else if(localDistance > 18 && localDistance < 30) {
//		 driveRobot->Drive(-0.3, -output/3); //was -0.3 -changed for testing purposes
		 closeEnough=true;
/*		 printf("magnitude: %f \n",-output/3 );
		 printf("local distance > 18 && < 30\n");*/
//		 printf("is close enough? >18 %d \n",closeEnough );
//			 SmartDashboard::PutString("Inbound Speed", "med");
		}
	 else if (localDistance<=18 && localDistance>0) {
//		 driveRobot->Drive(0.0, -output);
		 isReadytoPushGear=true;
/*		 printf("magnitude: %f \n",-output );
		 printf("local distance <= 18 && local distance > 0\n");*/
//		 printf("is close enough? <18 %d \n",closeEnough );
//		 printf("PID Write is done \n ");
//		 SmartDashboard::PutString("Inbound Speed", "0");
	 }

SmartDashboard::PutNumber("output value PW", output/3);
//printf("PIDWrite end\n");
 }

 void PIDVision::Run(){
	 if (isCapturing){

		 VisionLoop();
		 SmartDashboard::PutNumber("BadDifference", gp.getBadDifference());
		 SmartDashboard::PutNumber("BadOffset", gp.getBadOffset());
	 }

 }
 void PIDVision::StartCapturing(){
//	 printf("StartCapturing called\n");
	 isCapturing=true;
 }
 void PIDVision::StopCapturing(){
//	 printf("StopCapturing called\n");

	 isCapturing=false;

 }
 double PIDVision:: GetDistanceToTape(){
//	 printf("GetDistanceToTape called\n");

	 return gp.getDistance();

 }
int PIDVision::GetPegOffsetFromImageCenter(){
//	 printf("GetPegOffsetFromImageCenter called\n");
	 return gp.getOffset();

 }
bool PIDVision::CapturingVal(){
//	printf("CapturingVal called\n");
	 return isCapturing;
}

 void PIDVision::VisionLoop(){
//	 printf("VisionLoop called\n");
	 		if (cvSink.GrabFrame(mat) == 0 )
			{
//	 			printf("VisionLoop if called\n");
				outputStream.NotifyError(cvSink.GetError());

			}
			gp.Process(mat);
			outputStream.PutFrame(mat);
//			 printf("VisionLoop end\n");
 }
