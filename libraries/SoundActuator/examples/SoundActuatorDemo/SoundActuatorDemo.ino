/*
  SoundActuatorDemo - Demo code for use SoundActuator devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <SoundActuator.h>

SoundActuatorBasedOnGroveBuzzer soundActuator(1);

void setup() {
  // put your setup code here, to run once:
  soundActuator.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  soundActuator.playSound();
  
  delay(2000);
}