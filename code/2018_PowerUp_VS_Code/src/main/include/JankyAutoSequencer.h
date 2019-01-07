/*
 * JankyAutoSequencer.h
 *
 *  Created on: Feb 4, 2018
 *      Author: AnishaKabir
 */
#include "ReleaseCube.h"
#include "frc/WPILib.h"
#include "jankyStateMachine.h"
#include "ctre/Phoenix.h"
#include "JankyAutoEntry.h"
#include"InAndOut.h"
#include "UpAndDown.h"

#ifndef SRC_JANKYAUTOSEQUENCER_H_
#define SRC_JANKYAUTOSEQUENCER_H_

class JankyAutoSequencer:public JankyStateMachine {
public:
	JankyAutoSequencer(frc::RobotDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, InAndOut*inAndOut, UpAndDown*upAndDown);
	//JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, Encoder*encoder);
	virtual ~JankyAutoSequencer();
	void SetName(int state, const char* name, JankyAutoEntry*entry);
	enum StateValue {
		/*Rest,
		DrivingSwitchEdge,
		TurningToRightSwitchEdge,
		TurningToLeftSwitchEdge,
		DrivingToRightFieldEdge,
		DrivingToLeftFieldEdge,
		CrossRightAutoLine,
		CrossLeftAutoLine,
		DrivingMid,
		DrivingMidToAlignToLeftSwitch,
		DrivingMidToAlignToRightSwitch,
		DrivingLeftToAlignToRightSwitch,
		DrivingRightToAlignToLeftSwitch,
		DrivingToRightSwitchFront,
		DrivingToLeftSwitchFront,
		ReleaseCube*/
		Rest,
		TurnLeft90,
		TurnRight90,
		TurnLeft45,
		TurnRight45,
		TurnLeft30,
		TurnRight30,
		Drive6Inches,
		Drive10Inches,
		Drive40Inches, //driving to scale plate from mid scale/switch area -->turn 45 --> cubeupscale --> drive10in --> releasecube :)
		Drive50Inches,
		Drive52Inches,
		Drive60Inches,
		Drive72Inches,
		Drive120Inches,
		Drive144Inches,
		Drive162Inches,
		Drive210Inches, //driving to area between switch and scale to turn
		Drive240Inches, //driving in between scale and switch to get to opposite side scale plate
		Drive260Inches,
		CubeUp,
		CubeUpScale,
		ReleaseCube,
		VisionSegment,
		DriveBack10Inches,
		Stop
	};
	//void RunLeftCrossAutoLine();
	JankyAutoEntry* entries[MAX_NAMES];
	void SetMode(int mode);
	void EndSequence();
	void StateEngine(int curState);
	bool done;
};

#endif /* SRC_JANKYAUTOSEQUENCER_H_ */
