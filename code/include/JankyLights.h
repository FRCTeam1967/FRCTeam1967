/*
 * JankyLights.h
 *
 *  Created on: Mar 25, 2017
 *      Author: Elaine
 */
#include "WPILib.h"
#include "jankyTask.h"


#ifndef SRC_JANKYLIGHTS_H_
#define SRC_JANKYLIGHTS_H_

class JankyLights:public JankyTask {
public:
	JankyLights(int relayChannel);
	void Run();
	virtual ~JankyLights();

	void LightsOn();
	void LightsOff();
	void Blink(double speed);

private:
	int lightState;
	double blinkSpeed;

	Relay * light;

	void BlinkLights();

};

#endif /* SRC_JANKYLIGHTS_H_ */
