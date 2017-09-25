/*
  WeatherDetectorBeacon - Beacon code for use WeatherDetector in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>
#include <WeatherDetector.h>
#include <DHT.h>

#define W_STATE_POS  13

uint8_t advdata[] =
{
  0x0D,  // Length
  0xFF,  // Manufacturer data type
  
  // DEphisit BEacon identifier (0xDEBE)
  0xBE,
  0xDE,  
  
  // Latitude of the TrafficLightDetectorBeacon, in LE float format (28.481531)
  0x2D,
  0xDA,
  0xE3,
  0x41,
  
  // Longitude of the TrafficLightDetectorBeacon, in LE float format (-16.322899)
  0x4C,
  0x95,
  0x82,
  0xC1,
  
  0x02,  // Type of DEphisit BEacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
  
  // DEphisit BEacon data (different structure for each beacon type, and dynamic values)
  UNKNOWN  // Default state of the road -> Unknown
};

WeatherDetectorBasedOnGRainAndGHTPro weatherDetector(2, 6);

//uint8_t currentState, newState;
  
void setup() {

  Serial.begin(9600);
  
  weatherDetector.begin();
  
  /*currentState = weatherDetector.getRoadState();
  advdata[W_STATE_POS] = currentState;*/
  advdata[W_STATE_POS] = (uint8_t)weatherDetector.getRoadState();
  
  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);
  //Serial.print("advdata=");for (int i=0;i<sizeof(advdata);i++){Serial.print(advdata[i], HEX); Serial.print(" ");}Serial.println();
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  /*newState = weatherDetector.getRoadState();
  
  if (currentState != newState)
  {
    RFduinoBLE.end();
    
    advdata[W_STATE_POS] = newState;
    //RFduinoBLE_advdata = advdata;
    //RFduinoBLE_advdata_len = sizeof(advdata);
  
    RFduinoBLE.begin();
    
    currentState = newState;
  }*/
  
  RFduinoBLE.end();

  advdata[W_STATE_POS] = (uint8_t)weatherDetector.getRoadState();
  //RFduinoBLE_advdata = advdata;
  //RFduinoBLE_advdata_len = sizeof(advdata);
  
  RFduinoBLE.begin();
  
  /*switch (newState)
  {
    case DRY: Serial.println("The road is DRY"); break;
    case WET: Serial.println("The road is WET"); break;
    case ICE: Serial.println("The road has ICE"); break;
    case SNOW: Serial.println("The road has SNOW"); break;
    default: Serial.println("Unknown road's state");
  }*/
  
  RFduino_ULPDelay(1000);
}
