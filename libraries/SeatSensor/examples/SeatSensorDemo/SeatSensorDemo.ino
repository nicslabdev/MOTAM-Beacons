/*
  SeatSensorDemo - Demo code for use SeatSensor devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <SeatSensor.h>

SeatSensorBasedOnReedAndFSR seatSensor(5, 6);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  seatSensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int res = seatSensor.getSeatState();
  switch (res)
  {
    case SAFE: Serial.println("SAFE seat"); break;
    case NON_SAFE: Serial.println("NON-SAFE seat"); break;
    default: Serial.println("Unknown seat's state");
  }
  
  delay(1000);
}