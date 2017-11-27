/*
  Display_Light_Sound_Actuators_Genuino101
  Application code for use DisplayActuator, LightActuator and SoundActuator (and send 
  values over SerialPort) on Genuino 101 platform
  Created by Manuel Montenegro, Sep 04, 2017.
  Developed for MOTAM project. 
*/

//                  |   1 byte   |      1 byte       |   x bytes (OPTIONAL)  |
//                   --------------------------------------------------------
// Messages format: | HwDeviceId | HwDeviceActionReq | HwDeviceActionReqData |
//                   --------------------------------------------------------


// Includes for DisplayActuator, LightActuator and SoundActuator libraries
#include <DisplayActuator.h>
#include <rgb_lcd.h>
#include <LightActuator.h>
#include <SoundActuator.h>

//TODO Pasar a las correspondientes librerias?
// Identifiers of devices and messages
#define INVALID_HWDEVICEID        0x00
#define INVALID_HWDEVICEACTION        0x00

#define POSITION_SENSOR_TYPE          0x01
#define SPEED_SENSOR_TYPE         0x02
#define SEAT_SENSOR_TYPE          0x03
#define WEATHER_DETECTOR_TYPE         0x04
#define BICYCLE_DETECTOR_TYPE         0x05
#define TRAFFICLIGHT_DETECTOR_TYPE        0x06
#define DISPLAY_ACTUATOR_TYPE         0x07
#define LIGHT_ACTUATOR_TYPE           0x08
#define SOUND_ACTUATOR_TYPE           0x09

#define POSITION_SENSOR_GETPOSITION     0x01
#define SPEED_SENSOR_GETSPEED       0x02
#define SEAT_SENSOR_GETSEATSTATE      0x03
#define WEATHER_DETECTOR_GETROADSTATE     0x04
#define BICYCLE_DETECTOR_GETBICYCLESTATE    0x05
#define TRAFFICLIGHT_DETECTOR_GETTRAFFICLIGHTSTATE  0x06
#define DISPLAY_ACTUATOR_SETTEXTMESSAGE     0x07
#define LIGHT_ACTUATOR_SETLIGHTON     0x08
#define LIGHT_ACTUATOR_SETLIGHTOFF      0x09
#define SOUND_ACTUATOR_PLAYSOUND      0x0A

#define MAX_HW_DEVICES     10

// Set up display, light and sound actuators
DisplayActuatorBasedOnGroveLCD displayActuator;
LightActuatorBasedOnGroveLED lightActuator(5);
SoundActuatorBasedOnGroveBuzzer soundActuator(2);

void* hwDevicesRef[MAX_HW_DEVICES] = {&displayActuator, &lightActuator, &soundActuator};
byte hwDevicesType[MAX_HW_DEVICES] = {DISPLAY_ACTUATOR_TYPE, LIGHT_ACTUATOR_TYPE, SOUND_ACTUATOR_TYPE};
//char* dephisitDevicesId[MAX_HW_DEVICES] = {"DA:50:WarningDisplayActuator", "LA:50:WarningLightActuator", "SA:50:WarningSoundActuator"};


void setup() {
  // Open the serial port
  Serial.begin(9600);

  // Initialize devices
  displayActuator.begin();
  lightActuator.begin();
  soundActuator.begin();
}

void loop() {
  // number of bytes of the request message
  int numBytesRequestMessage = Serial.available();
  
  // if requestmessage has 2 bytes at least: correct format
  if (numBytesRequestMessage >= 2) {

    // byte array for incoming message
    byte reqMessage [numBytesRequestMessage];
    for (int i = 0; i < numBytesRequestMessage; i++) {
      reqMessage[i] = Serial.read();
    }

    //Process message
    byte hwDeviceId = reqMessage[0];
    byte hwDeviceAction = reqMessage[1];
    int reqDataSize = numBytesRequestMessage - 2;
    // byte array with size + 1 for carriage return
    byte hwDeviceActionReqData [reqDataSize+1];
    for (int i = 0; i < reqDataSize; i++) {
      hwDeviceActionReqData [i] = reqMessage[i+2];
    }
    // last character is carriage return
    hwDeviceActionReqData[reqDataSize] = 0x00;

    switch(hwDeviceId) {
      case DISPLAY_ACTUATOR_TYPE:
        switch(hwDeviceAction){
          // displayActuator show the received message    
            case DISPLAY_ACTUATOR_SETTEXTMESSAGE: 
              // displayActuator show the received message    
              // if there are request data: correct
              if ( reqDataSize > 0) {
                displayActuator.setTextMessage((char*)hwDeviceActionReqData); 
              // if no Request Data: error  
              } else {
                  printf("No text message found");
              }
              break;
              default: printf("Unknown action for DisplayActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
        }
        break;
        
      case LIGHT_ACTUATOR_TYPE:
        switch(hwDeviceAction){
            case LIGHT_ACTUATOR_SETLIGHTON: lightActuator.setLightOn(); break;
            case LIGHT_ACTUATOR_SETLIGHTOFF: lightActuator.setLightOff(); break;
            default: printf("Unknown action for LightActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
        }
        break;
        
      case SOUND_ACTUATOR_TYPE:
        switch(hwDeviceAction){
            case SOUND_ACTUATOR_PLAYSOUND: soundActuator.playSound(); break;
            default: printf("Unknown action for SoundActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
        }
        break;
        
      default:
        printf("Unknown hardware device\n"); hwDeviceId = INVALID_HWDEVICEID;
        break;
    }


  // if request message bytes number is inferior than 2, wrong format
  } else if (numBytesRequestMessage > 0) {
    printf("Wrong format in request message");
    // clear the Serial port buffer
    while (Serial.available() > 0) {
      byte trash;
      trash = Serial.read();
    }
  }
}

