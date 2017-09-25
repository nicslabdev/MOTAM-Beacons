/*
  BicycleDetector.h - Interface to be implemented by sensors 
  that want to provide information about bicycle's state. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef BicycleDetector_h
#define BicycleDetector_h

#define	UNKNOWN		0x00
#define STOPPED		0x01
#define ONGOING		0x02
#define	ACCIDENT 	0x03

class BicycleDetector
{
  public:
	virtual int getBicycleState(){};
};

class BicycleDetectorBasedOn2TiltAndReed : public BicycleDetector
{  
  public:
	BicycleDetectorBasedOn2TiltAndReed(int tilt1DigitalPin, int tilt2DigitalPin, int reedDigitalPin);
	void begin();
	int getBicycleState();
  private:
  	int _tilt1Pin;
	int _tilt2Pin;
	int _reedPin;
};  

#endif