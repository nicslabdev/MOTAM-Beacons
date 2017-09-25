/*
  LightActuator.cpp - Implementations of the actuators 
  that want to show light signals.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>

#include "LightActuator.h"

LightActuatorBasedOnGroveLED::LightActuatorBasedOnGroveLED(int ledDigitalPin)
{
  _ledPin = ledDigitalPin;
}

void LightActuatorBasedOnGroveLED::begin()
{
  pinMode(_ledPin, OUTPUT);
}

void LightActuatorBasedOnGroveLED::setLightOn()
{
  //TODO Implement the logic in line with the hardware 
  digitalWrite(_ledPin, HIGH);
}

void LightActuatorBasedOnGroveLED::setLightOff()
{
  //TODO Implement the logic in line with the hardware 
  digitalWrite(_ledPin, LOW);
}