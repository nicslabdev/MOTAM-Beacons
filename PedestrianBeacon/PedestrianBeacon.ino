/*
  PedestrianBeacon - Beacon code for stop signs in RFduino devices.
  Created by Manuel Montenegro, September 24, 2019.
  Developed for MOTAM project. 
*/

#include <RFduinoBLE.h>

uint8_t advdata[] =
{
  0x0C,  // Length
  0xFF,  // Manufacturer data type
  
  // DEphisit BEacon identifier
  0xDE,
  0xBE,

  0x0A,  // 1st part of Beacon ID (Type of Beacon)
  
  // 2nd part of Beacon ID
  0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13
  
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
