/*
  LightActuatorDemo - Demo code for use LightActuator devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <LightActuator.h>

LightActuatorBasedOnGroveLED lightActuator(5);

boolean ledOn = false;

void setup() {
  // put your setup code here, to run once:
  lightActuator.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (!ledOn)
  {
    lightActuator.setLightOn();
    ledOn = true;
  }
  else
  {
    lightActuator.setLightOff();
	ledOn = false;
  }
  
  delay(1000);
}