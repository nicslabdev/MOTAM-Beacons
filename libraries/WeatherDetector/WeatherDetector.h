/*
  WeatherDetector.h - Interface to be implemented by sensors 
  that want to provide information about weather road conditions. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef WeatherDetector_h
#define WeatherDetector_h

#include <DHT.h>

#define UNKNOWN	0x00
#define DRY		0x01
#define WET		0x02
#define	ICE		0x03
#define SNOW	0x04

class WeatherDetector
{
  public:
	virtual int getRoadState(){};
};

class WeatherDetectorBasedOnGRainAndGHTPro : public WeatherDetector
{  
  public:
	WeatherDetectorBasedOnGRainAndGHTPro(int rainDigitalPin, int htDigitalPin);
	void begin();
	int getRoadState();
  private:
  	int _rainPin;
	int _htPin;
	DHT *_dhtPointer;
};  

#endif