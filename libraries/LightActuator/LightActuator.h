/*
  LightActuator.h - Interface to be implemented by actuators 
  that want to show light signals. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef LightActuator_h
#define LightActuator_h

class LightActuator
{
  public:
	virtual void setLightOn(){};
	virtual void setLightOff(){};
};

class LightActuatorBasedOnGroveLED : public LightActuator
{  
  public:
	LightActuatorBasedOnGroveLED(int ledDigitalPin);
	void begin();
	void setLightOn();
	void setLightOff();
  private:
    int _ledPin;
};  

#endif