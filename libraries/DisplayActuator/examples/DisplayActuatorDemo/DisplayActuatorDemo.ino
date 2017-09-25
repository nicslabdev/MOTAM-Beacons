/*
  DisplayActuatorDemo - Demo code for use DisplayActuator devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <DisplayActuator.h>
#include <Wire.h>
#include <rgb_lcd.h>

DisplayActuatorBasedOnGroveLCD displayActuator;

void setup() {
  // put your setup code here, to run once:
  displayActuator.begin();
  
  // Print a message to the LCD.
  displayActuator.setTextMessage("DisplayActuatorDemo");
  
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  char str[32+1];
  
  sprintf(str, "DisplayActuatorDemo - %d", millis()/1000);
  displayActuator.setTextMessage(str);
  
  delay(1000);
}