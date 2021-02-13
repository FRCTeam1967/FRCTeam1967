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
#include "AutoDriveSubsystems.h"

#ifndef AUTO_SEQUENCER_H_
#define AUTO_SEQUENCER_H_

class AutoSequencer:public JankyStateMachine {
public:
	AutoSequencer(AutoDriveSubsystem * m_drive, int autoMode); //TODO: add shooter as a parameter
	virtual ~AutoSequencer();
	void SetName(int state, const char* name, AutoEntry*entry);

	enum StateValue {
		Trajectory1,
		Shoot
	};

	AutoEntry* entries[MAX_NAMES];
	void SetMode(int mode);
	void EndSequence();
	void StateEngine(int curState);
	bool done;
};

#endif /* AUTO_SEQUENCER_H_ */