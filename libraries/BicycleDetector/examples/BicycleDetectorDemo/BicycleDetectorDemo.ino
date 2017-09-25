/*
  BicycleDetectorDemo - Demo code for use BicycleDetector devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <BicycleDetector.h>

BicycleDetectorBasedOn2TiltAndReed bicycleDetector(4, 5, 6);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  bicycleDetector.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int res = bicycleDetector.getBicycleState();
  switch (res)
  {
    case ONGOING: Serial.println("The bicycle's state is ONGOING"); break;
    case STOPPED: Serial.println("The bicycle's state is STOPPED"); break;
    case ACCIDENT: Serial.println("The bicycle's state is ACCIDENT"); break;
    default: Serial.println("Unknown bicycle's state");
  }
  
  delay(1000);
}