/*
  TrafficLightDetector.h - Interface to be implemented by sensors 
  that want to provide information about traffic light's state. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef TrafficLightDetector_h
#define TrafficLightDetector_h

#include <Arduino.h>

#define GREEN			0x01
#define YELLOW			0x02
#define	RED				0x03
#define FLASHING_YELLOW 0x04
#define FLASHING_RED	0x05
#define OFF				0x06
#define UNKNOWN			0x0F

class TrafficLightDetector
{
  public:
	virtual int getTrafficLightState(){};
};

class TrafficLightDetectorBasedOn3LDR : public TrafficLightDetector
{  
  public:
	TrafficLightDetectorBasedOn3LDR(int ldrRedDigitalPin, int ldrYellowDigitalPin, int ldrGreenDigitalPin);
	void begin();
	int getTrafficLightState();
  private:
  	int _ldrRedPin;
	int _ldrYellowPin;
	int _ldrGreenPin;
};  

#endif