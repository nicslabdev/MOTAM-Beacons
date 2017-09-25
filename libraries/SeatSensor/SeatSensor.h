/*
  SeatSensor.h - Interface to be implemented by sensors 
  that want to provide information about security state of 
  the car's seats.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef SeatSensor_h
#define SeatSensor_h

#define SAFE		0
#define NON_SAFE	1

class SeatSensor
{
  public:
	virtual int getSeatState(){};
};

class SeatSensorBasedOnReedAndFSR : public SeatSensor
{  
  public:
    //TODO Change FSR pin from analog pin to digital pin?
	SeatSensorBasedOnReedAndFSR(int reedDigitalPin, int fsrAnalogPin);
	void begin();
	int getSeatState();
  private:
  	int _reedPin;
	int _fsrPin;
};  

#endif