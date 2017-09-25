/*
  DisplayActuator.h - Interface to be implemented by actuators 
  that want to show text messages to user. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef DisplayActuator_h
#define DisplayActuator_h

#include <rgb_lcd.h>

class DisplayActuator
{
  public:
	virtual void setTextMessage(const char* message){};
};

class DisplayActuatorBasedOnGroveLCD : public DisplayActuator
{  
  public:
	DisplayActuatorBasedOnGroveLCD();
	void begin();
	void setTextMessage(const char* message);
  private:
    rgb_lcd _lcd;
};  

#endif