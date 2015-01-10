
#include "jankySensor.h"


JankySensor::JankySensor(uint32_t channel) : DigitalInput (channel) //pass in our sensorchannel value to DigitalInput
{
	rReporting =0;
}

JankySensor::JankySensor(uint8_t moduleNumber, uint32_t channel) : DigitalInput (moduleNumber, channel) //pass in our sensorchannel value to DigitalInput
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

uint32_t JankySensor::Get()
{
	AutoDebounce();
	return rReporting;
	
}	

uint32_t JankySensor::GetRaw()
{
	return cSensor;
}
