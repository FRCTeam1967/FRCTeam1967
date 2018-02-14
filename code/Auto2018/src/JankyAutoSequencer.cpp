/*
 * JankyAutoSequencer.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: AnishaKabir
 */
#include "WPILib.h"
#include <JankyAutoSequencer.h>
#include "DriveSegment.h"
#include "TurnSegment.h"
#include "ctre/Phoenix.h"

//auto modes
#define DEFAULT_MODE 1
#define L_CROSS_AUTOLINE 2
#define L_SAME_SWITCH 3
#define L_OPPOSITE_SWITCH 4
#define M_LEFT_SWITCH 5
#define M_RIGHT_SWITCH 6
#define R_CROSS_AUTOLINE 7
#define R_SAME_SWITCH 8
#define R_OPPOSITE_SWITCH 9
#define DONE 10

#define TURN_SPEED 0.4
#define DRIVE_SPEED 0.4
float aMode;
float kP = 0.05;
float kI = 0.0;
float kD = 0.06;
int c;

TurnSegment*turnLeft90;
TurnSegment*turnRight90;
TurnSegment*turnLeft45;
TurnSegment*turnRight45;
DriveSegment*drive6Inches;
DriveSegment*drive60Inches;
DriveSegment*drive72Inches;
DriveSegment*drive120Inches;
DriveSegment*drive144Inches;
DriveSegment*drive162Inches;

//JankyAutoSequencer::JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder) {
JankyAutoSequencer::JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, Encoder*encoder) {
	SetMachineName("JankyAutoSequencer");
	SetName(Rest, "Rest");
	SetName(TurnLeft90, "Turn Left 90 Degrees");
	SetName(TurnRight90, "Turn Right 90 Degrees");
	SetName(TurnLeft45, "Turn Left 45 Degrees");
	SetName(TurnRight45, "Turn Right 45 Degrees");
	SetName(Drive6Inches, "Drive straight 6 inches");
	SetName(Drive60Inches, "Drive straight 60 inches");
	SetName(Drive72Inches, "Drive straight 72 inches");
	SetName(Drive120Inches, "Drive straight 120 inches");
	SetName(Drive144Inches, "Drive straight 144 inches");
	SetName(Drive162Inches, "Drive straight 162 inches");
	SetName(ReleaseCube, "Release cube onto the switch");
	SetName(Stop, "End of Sequence");

	turnLeft90 = new TurnSegment(gyro, drive, -90.0, TURN_SPEED, kP, kI, kD);
	turnRight90 = new TurnSegment(gyro, drive, 90.0, TURN_SPEED, kP, kI, kD);
	turnLeft45 = new TurnSegment(gyro, drive, -45.0, TURN_SPEED, kP, kI, kD);
	turnRight45 = new TurnSegment(gyro, drive, 45.0, TURN_SPEED, kP, kI, kD);
	drive6Inches = new DriveSegment(drive, encoder, 6, DRIVE_SPEED);
	drive60Inches = new DriveSegment(drive, encoder, 60, DRIVE_SPEED);
	drive72Inches = new DriveSegment(drive, encoder, 72, DRIVE_SPEED);
	drive120Inches = new DriveSegment(drive, encoder, 120, DRIVE_SPEED);
	drive144Inches = new DriveSegment(drive, encoder, 144, DRIVE_SPEED);
	drive162Inches = new DriveSegment(drive, encoder, 162, DRIVE_SPEED);
	/*drive6Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 6, DRIVE_SPEED);
	drive60Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 60, DRIVE_SPEED);
	drive72Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 72, DRIVE_SPEED);
	drive120Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 120, DRIVE_SPEED);
	drive144Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 144, DRIVE_SPEED);
	drive162Inches = new DriveSegment(drive, leftEncoder, rightEncoder, 162, DRIVE_SPEED);*/
	c = 0;
	aMode = DEFAULT_MODE;
	gyro->Calibrate();
	Start(); //most important part!!
	/*SetName(DrivingSwitchEdge, "Drive Straight 162 inches");
	SetName(TurningToRightSwitchEdge, "Turn left 90 degrees and drive forward 6 inches");
	SetName(TurningToLeftSwitchEdge, "Turn right 90 degrees and drive forward 6 inches");
	SetName(DrivingToRightFieldEdge, "Turn right 45 degrees and drive forward 60 inches");
	SetName(DrivingToLeftFieldEdge, "Turn right 45 degrees and drive forward 60 inches");
	SetName(CrossRightAutoLine, "Turn right 45 degrees and drive forward 120 inches");
	SetName(CrossLeftAutoLine, "Turn left 45 degrees and drive forward 120 inches");
	SetName(DrivingMid, "Drive straight 60 inches");
	SetName(DrivingMidToAlignToLeftSwitch, "Turn left 90 degrees and drive forward until encoder distance is 60 inches");
	SetName(DrivingMidToAlignToRightSwitch, "Turn right 90 degrees and drive forward until encoder distance is 60 inches");
	SetName(DrivingLeftToAlignToRightSwitch, "Turn right 90 degrees and drive forward until encoder distance is 144 inches");
	SetName(DrivingRightToAlignToLeftSwitch, "Turn left 90 degrees and drive forward until encoder distance is 144 inches");
	SetName(DrivingToRightSwitchFront, "Turn left 90 degrees and drive forward until encoder distance is 72 inches");
	SetName(DrivingToLeftSwitchFront, "Turn right 90 degrees and drive forward until encoder distance is 72 inches");
	SetName(ReleaseCube, "Release the cube onto the switch");*/
}

JankyAutoSequencer::~JankyAutoSequencer() {
	// TODO Auto-generated destructor stub
	delete turnLeft90;
	delete turnRight90;
	delete turnLeft45;
	delete turnRight45;
	delete drive6Inches;
	delete drive60Inches;
	delete drive72Inches;
	delete drive120Inches;
	delete drive144Inches;
	delete drive162Inches;
}

void JankyAutoSequencer::SetMode(int mode){ //call set mode in autoPeriodic
	if(aMode!=DONE){
		aMode=mode;
	}
}

void JankyAutoSequencer::StateEngine(int curState)
{
	switch(curState){
		case Rest:
			if(aMode==DONE){
				printf("SEQUENCE DONE!!! \n");
			}
			else if(aMode==L_CROSS_AUTOLINE){
				NewState(TurnLeft45, "Left Cross Auto Line selected");
			}
			else if(aMode==L_SAME_SWITCH){
				NewState(Drive162Inches, "Left Same Switch selected");
			}
			else if(aMode==L_OPPOSITE_SWITCH){
				NewState(Drive60Inches, "Left to Right Switch selected");
			}
			else if(aMode==M_LEFT_SWITCH){
				NewState(Drive60Inches, "Middle to Left Switch selected");
			}
			else if(aMode==M_RIGHT_SWITCH){
				NewState(Drive60Inches, "Middle to Right Switch selected");
			}
			else if(aMode==R_CROSS_AUTOLINE){
				NewState(TurnRight45, "Right Cross Auto Line selected");
			}
			else if(aMode==R_SAME_SWITCH){
				NewState(Drive162Inches, "Right Same Switch selected");
			}
			else if(aMode==R_OPPOSITE_SWITCH){
				NewState(Drive60Inches, "Right to Left Switch selected");
			}
			break;
		case TurnLeft90:
			if(turnLeft90->IsComplete()){
				if(aMode==M_LEFT_SWITCH){
					NewState(Drive60Inches, "Done turning left 90 degrees to drive to left switch");
				}
				else if(aMode==M_RIGHT_SWITCH){
					NewState(Drive72Inches, "Done turning left 90 degrees to align to right switch front");
				}
				else if(aMode==R_SAME_SWITCH){
					NewState(Drive6Inches, "Done turning left 90 degrees to align to right switch edge");
				}
				else if(aMode==L_OPPOSITE_SWITCH){
					NewState(Drive72Inches, "Done turning left 90 degrees to align to right switch front");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(Drive144Inches, "Done turning left 90 degrees to drive to left switch");
				}
			}
			break;
		case TurnRight90:
			if(turnRight90->IsComplete()){
				if(aMode==M_RIGHT_SWITCH){
					NewState(Drive60Inches, "Done turning right 90 degrees to drive to right switch");
				}
				else if(aMode==M_LEFT_SWITCH){
					NewState(Drive72Inches, "Done turning right 90 degrees to align to left switch front");
				}
				else if(aMode==L_SAME_SWITCH){
					NewState(Drive6Inches, "Done turning right 90 degrees to align to left switch edge");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(Drive72Inches, "Done turning right 90 degrees to align to left switch front");
				}
				else if(aMode==L_OPPOSITE_SWITCH){
					NewState(Drive144Inches, "Done turning right 90 degrees to drive to right switch");
				}
			break;
		case TurnLeft45:
			if(turnLeft45->IsComplete()){
				if(aMode==L_CROSS_AUTOLINE){
					NewState(Drive60Inches, "Done Turning Left 45 Degrees");
				}
				else if(aMode==R_CROSS_AUTOLINE){
					NewState(Drive120Inches, "Done Turning Left 45 Degrees at Right Field Edge");
				}

			}
			break;
		case TurnRight45:
			if(turnRight45->IsComplete()){
				if(aMode==R_CROSS_AUTOLINE){
					NewState(Drive60Inches, "Done Turning Right 45 Degrees");
				}
				else if(aMode==L_CROSS_AUTOLINE){
					NewState(Drive120Inches, "Done Turning Right 45 Degrees at Left Field Edge");
				}
			}
			break;
		case Drive6Inches:
			if(drive6Inches->IsComplete()){
				NewState(ReleaseCube, "Aligned with switch edge");
			}
			break;
		case Drive60Inches:
			if(drive60Inches->IsComplete()){
				if(aMode==L_CROSS_AUTOLINE){
					NewState(TurnRight45, "Done driving to left field edge");
				}
				else if(aMode==R_CROSS_AUTOLINE){
					NewState(TurnLeft45, "Done driving to right field edge");
				}
				else if(aMode==M_LEFT_SWITCH&&(c==0)){
					c++;
					NewState(TurnLeft90, "Done driving to mid");
				}
				else if(aMode==M_LEFT_SWITCH&&(c==1)){
					NewState(TurnRight90, "Done aligning to left switch");
				}
				else if(aMode==M_RIGHT_SWITCH&&(c==0)){
					c++;
					NewState(TurnRight90, "Done driving to mid");
				}
				else if(aMode==M_RIGHT_SWITCH&&(c==1)){
					NewState(TurnLeft90, "Done aligning to left switch");
				}
				else if(aMode==L_OPPOSITE_SWITCH){
					NewState(TurnRight90, "Done driving mid");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(TurnLeft90, "Done driving mid");
				}
			}
			break;
		case Drive72Inches:
			if(drive72Inches->IsComplete()){
				if(aMode==M_LEFT_SWITCH){
					NewState(ReleaseCube, "Done driving to left switch front");
				}
				else if(aMode==M_RIGHT_SWITCH){
					NewState(ReleaseCube, "Done driving to right switch front");
				}
				else if(aMode==L_OPPOSITE_SWITCH){
					NewState(ReleaseCube, "Done driving to left switch front");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(ReleaseCube, "Done driving to right switch front");
				}
			}
			break;
		case Drive120Inches:
			if(drive120Inches->IsComplete()){
				if(aMode==L_CROSS_AUTOLINE){
					NewState(Stop, "Crossed Auto Line on left side");
				}
				else if(aMode==R_CROSS_AUTOLINE){
					NewState(Stop, "Crossed Auto Line on right side");
				}
			}
			break;
		case Drive144Inches:
			if(drive144Inches->IsComplete()){
				if(aMode==L_OPPOSITE_SWITCH){
					NewState(TurnLeft90, "Done driving to align to right switch");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(TurnRight90, "Done driving to align to left switch");
				}
			}
			break;
		case Drive162Inches:
			if(drive162Inches->IsComplete()){
				if(aMode==L_SAME_SWITCH){
					NewState(TurnRight90, "Done Driving to Left Switch Edge");
				}
				else if(aMode==R_SAME_SWITCH){
					NewState(TurnLeft90, "Done Driving to Right Switch Edge");
				}
			}
			break;
		case ReleaseCube:
			//ADD GC CODE
				NewState(Stop, "Done releasing cube on switch");
			break;
		case Stop:
			aMode=DONE;
			NewState(Rest, "Sequence Finished");
			break;
	}
}
}
