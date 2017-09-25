/*
  SimulatedTemperatureSensorBLE - BLE code for simulated temperature sensors in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>

void setup() {
  RFduinoBLE.deviceName = "TS:45:EngTemp";

  RFduinoBLE.begin();
}

void loop() {
  RFduino_ULPDelay( SECONDS(1) );

  float temp = RFduino_temperature(CELSIUS);

  RFduinoBLE.sendFloat(temp);
}
