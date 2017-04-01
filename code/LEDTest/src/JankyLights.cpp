/*
 * JankyLights.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: Elaine
 */

#include <JankyLights.h>

#define OFF 0
#define ON 1
#define BLINK 2

JankyLights::JankyLights(int relayChannel) {
	// TODO Auto-generated constructor stub
	lightState = OFF;
	blinkSpeed = 1; // seconds

	light = new Relay(1, Relay::kForwardOnly);

	Start();
}

JankyLights::~JankyLights() {
	// TODO Auto-generated destructor stub
	delete light;
}

void JankyLights::LightsOn()
{
	lightState = ON;
}

void JankyLights::LightsOff()
{
	lightState = OFF;
}

void JankyLights::Blink(double speed)
{
	lightState = BLINK;
	blinkSpeed = speed;
}


void JankyLights::BlinkLights()
{
	light->Set(Relay::Value::kOn);
	Wait(blinkSpeed);
	light->Set(Relay::Value::kOff);
	Wait(blinkSpeed);
}

void JankyLights::Run()
{
	switch (lightState)
	{
	case OFF:
		light->Set(Relay::Value::kOff);
		break;
	case ON:
		light->Set(Relay::Value::kOn);
		break;
	case BLINK:
		BlinkLights();
		break;
	}

}
