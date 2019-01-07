/*
 * CubeUp.h
 *
 *  Created on: Mar 3, 2018
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"
#include "InAndOut.h"
#include "UpAndDown.h"
#ifndef SRC_CUBEUP_H_
#define SRC_CUBEUP_H_

class CubeUp : public JankyAutoEntry{
public:
	CubeUp(InAndOut*inAndOut, UpAndDown*upAndDown, char height);
	virtual ~CubeUp();
	void Start();
	bool JobDone();
	void RunAction();
	void End();

	InAndOut*IAO;
	UpAndDown*UAD;
	char _height;
};

#endif /* SRC_CUBEUP_H_ */
