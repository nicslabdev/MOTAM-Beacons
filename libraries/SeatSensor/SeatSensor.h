/*
  SeatSensor.cpp - Implementations of the sensors 
  that want to provide information about security state of 
  the car's seats.
  Created by Manuel Montenegro, September 24, 2019.
  Developed for MOTAM project. 
*/
#ifndef SeatSensor_h
#define SeatSensor_h

#define PRESENCE		  1
#define NON_PRESENCE	0
#define LOCK          1
#define UNLOCK        0

class SeatSensor
{
  public:
	virtual int getPresenceState(){};
  virtual int getLockState(){};
};

class SeatSensorBasedOnReedAndFSR : public SeatSensor
{  
  public:
    //TODO Change FSR pin from analog pin to digital pin?
	SeatSensorBasedOnReedAndFSR(int reedDigitalPin, int fsrAnalogPin);
	void begin();
	int getPresenceState();
  int getLockState();
  private:
  	int _reedPin;
	  int _fsrPin;
};  

#endif