/*
  SoundActuator.cpp - Implementations of the actuators 
  that want to emit sound signals to user.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>

#include "SoundActuator.h"

#define TONE		1915
#define DURATION	100

SoundActuatorBasedOnGroveBuzzer::SoundActuatorBasedOnGroveBuzzer(int buzzerDigitalPin)
{
  _buzzerPin = buzzerDigitalPin;
}

void SoundActuatorBasedOnGroveBuzzer::begin()
{
  pinMode(_buzzerPin, OUTPUT);
}

void SoundActuatorBasedOnGroveBuzzer::playSound()
{
  //TODO Implement the logic in line with the hardware 
  playTone(TONE, DURATION);
}

void SoundActuatorBasedOnGroveBuzzer::playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(_buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(_buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}