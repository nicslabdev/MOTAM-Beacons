/*
  StopSignBeacon - Beacon code for stop signs in RFduino devices.
  Created by Manuel Montenegro, September 24, 2019.
  Developed for MOTAM project. 
*/

#include <RFduinoBLE.h>

#define STOP_SIGN  0x10

uint8_t advdata[] =
{
  0x11,  // Length
  0xFF,  // Manufacturer data type
  
  // DEphisit BEacon identifier
  0xDE,
  0xBE,

  0x01,  // Type of DEphisit BEacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
  
  // Latitude of the TrafficLightDetectorBeacon, in LE float format (28.481768)
  0x42,
  0x12,
  0xDD,
  0x82,
  
  // Longitude of the TrafficLightDetectorBeacon, in LE float format (-16.324577)
  0xC0,
  0x8F,
  0xF7,
  0x10,  

  0x23,  // From direction that applies (35)
  0x00,
  
  0x18,  // To direction that applies (280)
  0x01,
  
  // DEphisit BEacon data (different structure for each beacon type, and dynamic values)
  STOP_SIGN,  // Default state of traffic light -> Unknown
  
};
  
void setup() {

  Serial.begin(9600);
  
  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);
  //Serial.print("advdata=");for (int i=0;i<sizeof(advdata);i++){Serial.print(advdata[i], HEX); Serial.print(" ");}Serial.println();
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  RFduino_ULPDelay(INFINITE);
}
