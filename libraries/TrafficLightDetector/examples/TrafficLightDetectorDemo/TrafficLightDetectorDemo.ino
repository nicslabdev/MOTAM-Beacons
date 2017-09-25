/*
  TrafficLightDetectorDemo - Demo code for use TrafficLightDetector devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <TrafficLightDetector.h>

TrafficLightDetectorBasedOn3LDR trafficLightDetector(4, 5, 6);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  trafficLightDetector.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int res = trafficLightDetector.getTrafficLightState();
  switch (res)
  {
    case GREEN: Serial.println("The traffic light is GREEN"); break;
    case YELLOW: Serial.println("The traffic light is YELLOW"); break;
    case RED: Serial.println("The traffic light is RED"); break;
    case FLASHING_YELLOW: Serial.println("The traffic light is FLASHING-YELLOW"); break;
    case FLASHING_RED: Serial.println("The traffic light is FLASHING-RED"); break;
    case OFF: Serial.println("The traffic light is OFF"); break;
    default: Serial.println("Unknown traffic light's state");
  }
  
  delay(1000);
}