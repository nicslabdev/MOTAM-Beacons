/*
  TrafficLightDetectorBeacon - Beacon code for use TrafficLightDetector in RFduino devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <RFduinoBLE.h>
#include <TrafficLightDetector.h>

#define TS_STATE_POS  13

uint8_t advdata[] =
{
  0x11,  // Length
  0xFF,  // Manufacturer data type
  
  // DEphisit BEacon identifier (0xDEBE)
  0xBE,
  0xDE,  

  
  // Latitude of the TrafficLightDetectorBeacon, in LE float format (28.481531)
  0x45,
  0xDD,
  0x12,
  0x42,


  // Longitude of the TrafficLightDetectorBeacon, in LE float format (-16.322899)
  0x5A,
  0xD7,
  0x8F,
  0xC0,
  
  0x01,  // Type of DEphisit BEacon (0x01 -> Traffic Sign Beacon, 0x02 -> Weather Beacon, 0x03 -> Bicycle Beacon)
  
  // DEphisit BEacon data (different structure for each beacon type, and dynamic values)
  UNKNOWN,  // Default state of traffic light -> Unknown
  
  0x18,  // From direction that applies (280)
  0x01,
  
  0x18,  // To direction that applies (280)
  0x01
};

TrafficLightDetectorBasedOn3LDR trafficLightDetector(4, 5, 6);

uint8_t currentState, newState;
  
void setup() {

  Serial.begin(9600);
  
  trafficLightDetector.begin();
  currentState = trafficLightDetector.getTrafficLightState();
  
  advdata[TS_STATE_POS] = currentState;
  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);
  //Serial.print("advdata=");for (int i=0;i<sizeof(advdata);i++){Serial.print(advdata[i], HEX); Serial.print(" ");}Serial.println();
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;
  
  RFduinoBLE.begin();
}

void loop() {
  
  newState = trafficLightDetector.getTrafficLightState();
  
  if (currentState != newState)
  {
    RFduinoBLE.end();
    
    advdata[TS_STATE_POS] = newState;
    //RFduinoBLE_advdata = advdata;
    //RFduinoBLE_advdata_len = sizeof(advdata);
  
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
