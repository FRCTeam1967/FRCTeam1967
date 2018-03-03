/*
 * ReleaseCube.h

 *
 *  Created on: Feb 11, 2018
 *      Author: Maxine
 */

#ifndef SRC_RELEASECUBE_H_
#define SRC_RELEASECUBE_H_
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "JankyAutoEntry.h"
#include "InAndOut.h"
#include "UpAndDown.h"
#include <string>

class ReleaseCube : public JankyAutoEntry{
public:
	ReleaseCube(RobotDrive*drive, InAndOut*INO,UpAndDown*UAD, char height);
	virtual ~ReleaseCube();

protected:
	/**
	 * @brief One time action for preparing or starting
	 * Move the mechanism up by the height and release the item
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void Start();

	/**
	 * @brief Returns whether the turn is complete or not
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	bool JobDone();

	/**
	 * @brief is called repeatedly by jankyAutoEntry by IsComplete()
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void RunAction();



	/**
	 * @brief disables the motor
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void End();

private:
	bool startDriving;
	char _height;
	InAndOut*IAO;
	UpAndDown*UAD;
	RobotDrive*chassis;
};

#endif /* SRC_RELEASECUBE_H_ */
