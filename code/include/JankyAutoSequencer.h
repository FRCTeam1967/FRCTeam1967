/*
 * JankyAutoSequencer.h
 *
 *  Created on: Feb 4, 2018
 *      Author: AnishaKabir
 */
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "ctre/Phoenix.h"

#ifndef SRC_JANKYAUTOSEQUENCER_H_
#define SRC_JANKYAUTOSEQUENCER_H_

class JankyAutoSequencer:public JankyStateMachine {
public:
	JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder);
	//JankyAutoSequencer(RobotDrive*drive, frc::ADXRS450_Gyro*gyro, Encoder*encoder);
	virtual ~JankyAutoSequencer();
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
		Drive6Inches,
		Drive60Inches,
		Drive72Inches,
		Drive120Inches,
		Drive144Inches,
		Drive162Inches,
		ReleaseCube,
		Stop
	};
	//void RunLeftCrossAutoLine();
	void SetMode(int mode);
	void EndSequence();
	void StateEngine(int curState);
};

#endif /* SRC_JANKYAUTOSEQUENCER_H_ */
