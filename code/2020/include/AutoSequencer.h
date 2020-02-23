/*
 * AutoSequencer.h
 *
 *  Created on: Feb 13, 2020
 *      Author: Maddie
 */

#include "jankyStateMachine.h"
#include "ctre/Phoenix.h"
#include <frc/RobotDrive.h>
#include <frc/ADXRS450_Gyro.h>
#include "AutoEntry.h"

#ifndef AUTO_SEQUENCER_H_
#define AUTO_SEQUENCER_H_

class AutoSequencer:public JankyStateMachine {
public:
	AutoSequencer(frc::RobotDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor); //TODO: add shooter as a parameter
	virtual ~AutoSequencer();
	void SetName(int state, const char* name, AutoEntry*entry);

	enum StateValue {
		Rest,
        BackUp12Inches,
        CenterRobotWithTarget,
        ShootBalls,
		Stop
	};

	AutoEntry* entries[MAX_NAMES];
	void SetMode(int mode);
	void EndSequence();
	void StateEngine(int curState);
	bool done;
};

#endif /* AUTO_SEQUENCER_H_ */