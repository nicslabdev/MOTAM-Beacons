/*
  StopSignBeacon - Beacon code for stop signs in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>

#define STOP_SIGN  0x10

#define DNSIZE  9
#define ADSIZE  6

// DEPHISIT beacon mark
const byte DEPHISIT_BEACON = 0xFF;

// Geographic position (latitude, longitude)
const float LATITUDE = 28.481768;
const float LONGITUDE = -16.324577;

// Type of beacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
const byte BEACON_TYPE = 0x01;

// Set stop as traffic sign
const byte TRAFFIC_SIGN = STOP_SIGN;

// From/To direction that applies
const unsigned int FROM_DIR = 35;  //TODO Values under 256, set 0x00 byte and is considered as string end by RFduinoBLE -> ERROR 
const unsigned int TO_DIR = 280;

char deviceNameField[DNSIZE+1];
char advertisementDataField[ADSIZE+1]; 

  
void setup() {

  Serial.begin(9600);
  
  memset(deviceNameField, 0, sizeof(deviceNameField));
  memcpy(deviceNameField, &DEPHISIT_BEACON, 1);
  memcpy(deviceNameField+1, &LATITUDE, 4);
  memcpy(deviceNameField+5, &LONGITUDE, 4);
  RFduinoBLE.deviceName = deviceNameField;
  
  memset(advertisementDataField, 0, sizeof(advertisementDataField));
  memcpy(advertisementDataField, &BEACON_TYPE, 1);
  memcpy(advertisementDataField+1, &TRAFFIC_SIGN, 1);
  memcpy(advertisementDataField+2, &FROM_DIR, 2);
  memcpy(advertisementDataField+4, &TO_DIR, 2);
  RFduinoBLE.advertisementData = advertisementDataField;
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  RFduino_ULPDelay(INFINITE);
}
