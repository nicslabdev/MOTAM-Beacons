/*
  SeatSensor.cpp - Implementations of the sensors 
  that want to provide information about security state of 
  the car's seats.
  Created by Manuel Montenegro, September 24, 2019.
  Developed for MOTAM project. 
*/
#include <Arduino.h>

#include "SeatSensor.h"

#define FSR_THRESHOLD	512

//TODO Change FSR pin from analog pin to digital pin?

SeatSensorBasedOnReedAndFSR::SeatSensorBasedOnReedAndFSR(int reedDigitalPin, int fsrAnalogPin)
{
  _reedPin = reedDigitalPin;

  _fsrPin = fsrAnalogPin;
}

void SeatSensorBasedOnReedAndFSR::begin()
{
  pinMode(_reedPin, INPUT);
}


int SeatSensorBasedOnReedAndFSR::getPresenceState()
{
  int fsrValue = analogRead(_fsrPin);
  
  //Serial.println(fsrValue); //-> El valor sin presion es de 3 y con presion ha llegado hasta casi 1000
  Serial.print(" FSR: "); Serial.println(fsrValue);
  
  if (fsrValue > FSR_THRESHOLD)
  {
	return PRESENCE;
  }
  else
  {
	return NON_PRESENCE;
  }
}


int SeatSensorBasedOnReedAndFSR::getLockState()
{
  int reedValue = digitalRead(_reedPin);
  Serial.print(" R: "); Serial.print(reedValue);
  if (reedValue == LOW)
  {
    return UNLOCK;
  }
  else
  {
    return LOCK;
  }
}