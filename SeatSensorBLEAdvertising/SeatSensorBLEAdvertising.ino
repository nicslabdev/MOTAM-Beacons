/*
  SeatSensorBeacon - Beacon code for seat sensors in RFduino devices.
  Created by Manuel Montenegro, September 24, 2018.
  Developed for MOTAM project. 
*/

#include <RFduinoBLE.h>
#include <SeatSensor.h>

#define S_PRESENCE_POS  13
#define S_LOCK_POS      14

uint8_t advdata[] =
{
  0x0E,       // Length
  0xFF,       // Manufacturer data type

  0xDE, 0xBE, // DEphisit BEacon identifier (0xDEBE)

  0x04,       // MOTAM ID 1st part (type of MOTAM Beacon)
  
  0x00, 0x01, 0x00, 0x01, 0x03, 0x11, 0x00, 0xAA, // MOTAM ID 2nd part
 
  0xFF, 0xFF  // Beacon Data
};

SeatSensorBasedOnReedAndFSR seatSensor(5, 6);

uint8_t currentPresenceState, newPresenceState;
uint8_t currentLockState, newLockState;
  
void setup() {

  Serial.begin(9600);
  
  seatSensor.begin();
  
  currentPresenceState = seatSensor.getPresenceState();
  currentLockState = seatSensor.getLockState();
  
  advdata[S_PRESENCE_POS] = currentPresenceState;
  advdata[S_LOCK_POS] = currentLockState;
  
  RFduinoBLE_advdata = advdata;
  RFduinoBLE_advdata_len = sizeof(advdata);
  // Serial.print("advdata=");for (int i=0;i<sizeof(advdata);i++){Serial.print(advdata[i], HEX); Serial.print(" ");}Serial.println();
  
  RFduinoBLE.advertisementInterval = 20;
  RFduinoBLE.txPowerLevel = +4;

  RFduinoBLE.begin();
}

void loop() {
  
  newPresenceState = seatSensor.getPresenceState();
  newLockState = seatSensor.getLockState();

  if (currentPresenceState != newPresenceState || currentLockState != newLockState)
  {
    RFduinoBLE.end();
    
    advdata[S_PRESENCE_POS] = newPresenceState;
    advdata[S_LOCK_POS] = newLockState;
    //RFduinoBLE_advdata = advdata;
    //RFduinoBLE_advdata_len = sizeof(advdata);

    RFduinoBLE.begin();
    
    currentPresenceState = newPresenceState;
    currentLockState = newLockState;
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
