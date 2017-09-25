/*
  TrafficLightDetectorBeacon - Beacon code for use TrafficLightDetector in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>
#include <TrafficLightDetector.h>

#define DNSIZE  9
#define ADSIZE  6

// DEPHISIT beacon mark
const byte DEPHISIT_BEACON = 0xFF;

// Geographic position (latitude, longitude)
const float LATITUDE = 28.481531;
const float LONGITUDE = -16.322899;

// Type of beacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
const byte BEACON_TYPE = 0x01;

// From/To direction that applies
const unsigned int FROM_DIR = 280;   
const unsigned int TO_DIR = 280;

TrafficLightDetectorBasedOn3LDR trafficLightDetector(4, 5, 6);

int currentState, newState;
char deviceNameField[DNSIZE+1];
char advertisementDataField[ADSIZE+1]; 
  
void setup() {

  Serial.begin(9600);
  
  trafficLightDetector.begin();
  currentState = trafficLightDetector.getTrafficLightState();
  
  memset(deviceNameField, 0, sizeof(deviceNameField));
  memcpy(deviceNameField, &DEPHISIT_BEACON, 1);
  memcpy(deviceNameField+1, &LATITUDE, 4);
  memcpy(deviceNameField+5, &LONGITUDE, 4);
  RFduinoBLE.deviceName = deviceNameField;
  //Serial.print("deviceNameField=");for (int i=0;i<sizeof(deviceNameField);i++){Serial.print(deviceNameField[i], HEX); Serial.print(" ");}Serial.println();
  //Serial.print("deviceNameField=");Serial.println(deviceNameField);
  
  memset(advertisementDataField, 0, sizeof(advertisementDataField));
  memcpy(advertisementDataField, &BEACON_TYPE, 1);
  memcpy(advertisementDataField+1, &currentState, 1);
  memcpy(advertisementDataField+2, &FROM_DIR, 2);
  memcpy(advertisementDataField+4, &TO_DIR, 2);
  RFduinoBLE.advertisementData = advertisementDataField;
  //Serial.print("advertisementDataField=");for (int i=0;i<sizeof(advertisementDataField);i++){Serial.print(advertisementDataField[i], HEX); Serial.print(" ");}Serial.println();
  //Serial.print("advertisementDataField=");Serial.println(advertisementDataField);
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  newState = trafficLightDetector.getTrafficLightState();
  
  if (currentState != newState)
  {
    RFduinoBLE.end();
    
    memcpy(advertisementDataField+1, &newState, 1);
    RFduinoBLE.advertisementData = advertisementDataField;
    
    RFduinoBLE.begin();
    
    currentState = newState;
  }
  
  /*switch (newState)
  {
    case GREEN: Serial.println("The traffic light is GREEN"); break;
    case YELLOW: Serial.println("The traffic light is YELLOW"); break;
    case RED: Serial.println("The traffic light is RED"); break;
    case FLASHING_YELLOW: Serial.println("The traffic light is FLASHING-YELLOW"); break;
    case FLASHING_RED: Serial.println("The traffic light is FLASHING-RED"); break;
    case OFF: Serial.println("The traffic light is OFF"); break;
    default: Serial.println("Unknown traffic light's state");
  }*/
  
  //RFduino_ULPDelay(100);
}
