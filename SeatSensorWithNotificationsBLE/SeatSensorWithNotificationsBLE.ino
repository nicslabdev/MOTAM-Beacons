/*
  SeatSensorWithNotificationsBLE - BLE code for seat sensors with notifications in RFduino devices.
  Created by Jesus Rodriguez, June 8, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>
#include <SeatSensor.h>

SeatSensorBasedOnReedAndFSR seatSensor(6, 5);

void setup() {
  seatSensor.begin();
  
  RFduinoBLE.deviceName = "SeatSWN:35:CS1";

  RFduinoBLE.begin();
}

void loop() {
  RFduino_ULPDelay( SECONDS(1) );

  byte state = seatSensor.getSeatState();

  RFduinoBLE.sendByte(state);
}
