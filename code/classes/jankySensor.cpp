
#include "jankySensor.h"


JankySensor::JankySensor(UINT32 channel) : DigitalInput (channel) //pass in our sensorchannel value to DigitalInput
{
	rReporting =0;
}

JankySensor::JankySensor(UINT8 moduleNumber, UINT32 channel) : DigitalInput (moduleNumber, channel) //pass in our sensorchannel value to DigitalInput
{
	rReporting =0;
}

JankySensor::~JankySensor()
{
	
}

void JankySensor::AutoDebounce()
{
	cSensor= DigitalInput::Get(); //scoping-run the get function from the class DigitalInput
	
	if (cSensor != rReporting) {
		++cContiguous;
		
		if (cContiguous > THRESHOLD){
			rReporting = cSensor;
			cContiguous=0;
		}
	}
	
	else 
		cContiguous=0;
	
}

UINT32 JankySensor::Get()
{
	AutoDebounce();
	return rReporting;
	
}	

UINT32 JankySensor::GetRaw()
{
	return cSensor;
}
