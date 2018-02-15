/*
  BicycleDetectorBeacon - Beacon code for use BicycleDetector in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>
#include <BicycleDetector.h>

#define B_STATE_POS  13

uint8_t advdata[] =
{
  0x0D,  // Length
  0xFF,  // Manufacturer data type
  
  // DEphisit BEacon identifier (0xDEBE)
  0xBE,
  0xDE,  
  
  // Latitude of the TrafficLightDetectorBeacon, in LE float format (not present)
  0xEC,
  0xDB,
  0x12,
  0x42,
  
  // Longitude of the TrafficLightDetectorBeacon, in LE float format (not present)
  0x9D,
  0xE4,
  0x8F,
  0xC0,
  
  0x03,  // Type of DEphisit BEacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
  
  // DEphisit BEacon data (different structure for each beacon type, and dynamic values)
  UNKNOWN  // Default state of the bicycle -> Unknown
};

BicycleDetectorBasedOn2TiltAndReed bicycleDetector(4, 5, 6);

uint8_t currentState, newState;
  
void setup() {

  Serial.begin(9600);
  
  bicycleDetector.begin();
  currentState = bicycleDetector.getBicycleState();
  
  advdata[B_STATE_POS] = currentState;
  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);
  //Serial.print("advdata=");for (int i=0;i<sizeof(advdata);i++){Serial.print(advdata[i], HEX); Serial.print(" ");}Serial.println();
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  newState = bicycleDetector.getBicycleState();
  
  if (currentState != newState)
  {
    RFduinoBLE.end();
    
    advdata[B_STATE_POS] = newState;
    //RFduinoBLE_advdata = advdata;
    //RFduinoBLE_advdata_len = sizeof(advdata);
  
    RFduinoBLE.begin();
    
    currentState = newState;
  }
  
  /*switch (newState)
  {
    case ONGOING: Serial.println("The bicycle's state is ONGOING"); break;
    case STOPPED: Serial.println("The bicycle's state is STOPPED"); break;
    case ACCIDENT: Serial.println("The bicycle's state is ACCIDENT"); break;
    default: Serial.println("Unknown bicycle's state");
  }*/
  
  //RFduino_ULPDelay(100);
}
