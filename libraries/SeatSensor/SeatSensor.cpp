/*
  SeatSensor.cpp - Implementations of the sensors 
  that want to provide information about security state of 
  the car's seats.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
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

int SeatSensorBasedOnReedAndFSR::getSeatState()
{
  int reedValue = digitalRead(_reedPin);
  int fsrValue = analogRead(_fsrPin);
  
  //Serial.println(fsrValue); //-> El valor sin presion es de 3 y con presion ha llegado hasta casi 1000
  Serial.print("R: "); Serial.print(reedValue); Serial.print(", FSR: "); Serial.println(fsrValue);
  
  if (reedValue == LOW && fsrValue > FSR_THRESHOLD)
  {
	return NON_SAFE;
  }
  else
  {
	return SAFE;
  }
  
  //return -1;
}