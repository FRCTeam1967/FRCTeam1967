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
#include "ReleaseCube.h"
#include "ctre/Phoenix.h"
#include "jankyStateMachine.h"
#include"InAndOut.h"
#include "UpAndDown.h"
#include "CubeUp.h"
#include "VisionSegment.h"

#define MAX_NAMES 32

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
#define L_SAME_SCALE 10
#define R_SAME_SCALE 11
#define DONE 12

#define TURN_SPEED 0.4
#define DRIVE_SPEED 0.4
float aMode;
float turn_kP = 0.05;
float turn_kI = 0.0;
float turn_kD = 0.06;

float drive_kP = 0.03;
float drive_kI = 0.0;
float drive_kD = 0.0;
int c;

TurnSegment*turnLeft90;
TurnSegment*turnRight90;
TurnSegment*turnLeft45;
TurnSegment*turnRight45;
DriveSegment*drive6Inches;
DriveSegment*drive10Inches;
DriveSegment*drive60Inches;
DriveSegment*drive72Inches;
DriveSegment*drive120Inches;
DriveSegment*drive144Inches;
DriveSegment*drive162Inches;
DriveSegment*drive260Inches;
CubeUp*cubeUp;
CubeUp*cubeUpScale;
ReleaseCube*releaseCube;
VisionSegment*visionSegment;

JankyAutoSequencer::JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, InAndOut*inAndOut, UpAndDown*upAndDown) {
//JankyAutoSequencer::JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, Encoder*encoder) {
	for(int i = 0; i<MAX_NAMES; i++){
		entries[i]=NULL;
	}
	turnLeft90 = new TurnSegment(gyro, drive, -90.0, TURN_SPEED, turn_kP, turn_kI, turn_kD);
	turnRight90 = new TurnSegment(gyro, drive, 90.0, TURN_SPEED, turn_kP, turn_kI, turn_kD);
	turnLeft45 = new TurnSegment(gyro, drive, -45.0, TURN_SPEED, turn_kP, turn_kI, turn_kD);
	turnRight45 = new TurnSegment(gyro, drive, 45.0, TURN_SPEED, turn_kP, turn_kI, turn_kD);
	drive6Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 0, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive10Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 10, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive60Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 60, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive72Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 72, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive120Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 120, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive144Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 144, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive162Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 140, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	drive260Inches = new DriveSegment(gyro, drive, leftEncoder, rightEncoder, leftmotor, rightmotor, 260, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
	cubeUp = new ::CubeUp(inAndOut, upAndDown, 'l');
	cubeUpScale = new ::CubeUp(inAndOut, upAndDown, 'm');
	releaseCube = new ::ReleaseCube(drive, inAndOut, upAndDown, 'l');
	visionSegment = new ::VisionSegment(drive, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);

	SetMachineName("JankyAutoSequencer");
	JankyStateMachine::SetName(Rest, "Rest");
	SetName(TurnLeft90, "Turn Left 90 Degrees", turnLeft90);
	SetName(TurnRight90, "Turn Right 90 Degrees", turnRight90);
	SetName(TurnLeft45, "Turn Left 45 Degrees", turnLeft45);
	SetName(TurnRight45, "Turn Right 45 Degrees", turnRight45);
	SetName(Drive6Inches, "Drive straight 6 inches", drive6Inches);
	SetName(Drive10Inches, "Drive straight 10 inches", drive10Inches);
	SetName(Drive60Inches, "Drive straight 60 inches", drive60Inches);
	SetName(Drive72Inches, "Drive straight 72 inches", drive72Inches);
	SetName(Drive120Inches, "Drive straight 120 inches", drive120Inches);
	SetName(Drive144Inches, "Drive straight 144 inches", drive144Inches);
	SetName(Drive162Inches, "Drive straight 162 inches", drive162Inches);
	SetName(Drive260Inches, "Drive straight 260 inches", drive260Inches);
	SetName(CubeUp, "Lift cube to switch height", cubeUp);
	SetName(CubeUpScale, "Lift cube to scale height", cubeUpScale);
	SetName(ReleaseCube, "Release cube onto the switch", releaseCube);
	SetName(VisionSegment, "Drive to the switch with vision", visionSegment);
	JankyStateMachine::SetName(Stop, "End of Sequence");

	c = 0;
	done = false;
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
	delete drive10Inches;
	delete drive60Inches;
	delete drive72Inches;
	delete drive120Inches;
	delete drive144Inches;
	delete drive162Inches;
	delete drive260Inches;
	delete cubeUp;
	delete cubeUpScale;
	delete releaseCube;
	delete visionSegment;
}

void JankyAutoSequencer::SetName(int state, const char* name, JankyAutoEntry*entry){
	JankyStateMachine::SetName(state, name);
	entries[state]=entry;
}

void JankyAutoSequencer::SetMode(int mode){ //call set mode in autoPeriodic
	if(aMode!=DONE){
		aMode=mode;
	}
}

void JankyAutoSequencer::EndSequence(){
	if(entries[GetCurrentState()]){
		entries[GetCurrentState()]->Abort();
		printf("Aborted Entry: %s \n", names[GetCurrentState()]);
	}
	Terminate();
}

void JankyAutoSequencer::StateEngine(int curState)
{
	switch(curState){
		case Rest:
			if(aMode==DONE){
				if(!done){
					printf("SEQUENCE DONE!!! YAYAYAY\n");
				 	done=true;
				}
			}
			else if(aMode==L_CROSS_AUTOLINE){
				NewState(TurnLeft45, "Left Cross Auto Line selected");
			}
			else if(aMode==L_SAME_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==L_OPPOSITE_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==M_LEFT_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==M_RIGHT_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==R_CROSS_AUTOLINE){
				NewState(TurnRight45, "Right Cross Auto Line selected");
			}
			else if(aMode==R_SAME_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==R_OPPOSITE_SWITCH){
				NewState(CubeUp, "Lift cube to switch level");
			}
			else if(aMode==L_SAME_SCALE){
				NewState(Drive260Inches, "Left Same Scale Selected");
			}
			else if(aMode==R_SAME_SCALE){
				NewState(Drive260Inches, "Right Same Scale Selected");
			}
			break;
		case CubeUp:
			if(cubeUp->IsComplete()){
				if(aMode==L_SAME_SWITCH){
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
				else if(aMode==R_SAME_SWITCH){
					NewState(Drive162Inches, "Right Same Switch selected");
				}
				else if(aMode==R_OPPOSITE_SWITCH){
					NewState(Drive60Inches, "Right to Left Switch selected");
				}
			}
			break;
		case CubeUpScale:
			if(cubeUpScale->IsComplete()){
				NewState(Drive10Inches, "Done lifting cube to scale height");
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
				else if(aMode==R_SAME_SCALE){
					NewState(CubeUpScale, "Need to bring cube to right height");
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
				else if(aMode==L_SAME_SCALE){
					NewState(CubeUpScale, "Need to bring cube to right height");
				}
			}
			break;
		case Drive6Inches:
			if(drive6Inches->IsComplete()){
				NewState(ReleaseCube, "Aligned with switch edge");
			}
			break;
		case Drive10Inches:
			if(drive10Inches->IsComplete()){
				NewState(ReleaseCube, "Aligned with scale edge");
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
		//case VisionSegment:
			if(drive72Inches->IsComplete()){
			//if(visionSegment->IsComplete()){
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
		case Drive260Inches:
			if(drive260Inches->IsComplete()){
				if(aMode==L_SAME_SCALE){
					NewState(TurnRight45, "Done Driving to Left Scale Edge");
				}
				else if(aMode==R_SAME_SCALE){
					NewState(TurnLeft45, "Done Driving to Right Scale Edge");
				}
			}
			break;
		case ReleaseCube:
			if(releaseCube->IsComplete()){
				NewState(Stop, "Done releasing cube on switch");
			}
			break;
		case Stop:
			aMode=DONE;
			NewState(Rest, "Sequence Finished");
			break;
	}
}
}
