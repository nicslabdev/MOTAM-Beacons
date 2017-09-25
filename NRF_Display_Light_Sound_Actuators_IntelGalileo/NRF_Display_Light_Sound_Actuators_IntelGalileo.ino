/*
  NRF_Display_Light_Sound_Actuators_IntelGalileo - Application code for use DisplayActuator, LightActuator and SoundActuator (and send values over NRF24) 
  on Intel Galileo platform.
  Created by Jesus Rodriguez, May 13, 2015.
  Developed for DEPHISIT project. 
*/

// Includes for DisplayActuator, LightActuator and SoundActuator libraries
#include <DisplayActuator.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <LightActuator.h>
#include <SoundActuator.h>

// Includes for NRF24 library
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "RF24_for_intel_galileo.h"

// Includes for AES and HMAC-MD5 libraries
#include <AES.h>
#include <MD5.h>

//TODO Pasar a las correspondientes librerias?
#define INVALID_HWDEVICEID				0x00
#define INVALID_HWDEVICEACTION				0x00

#define POSITION_SENSOR_TYPE  				0x01
#define SPEED_SENSOR_TYPE  				0x02
#define SEAT_SENSOR_TYPE  				0x03
#define WEATHER_DETECTOR_TYPE  				0x04
#define BICYCLE_DETECTOR_TYPE  				0x05
#define TRAFFICLIGHT_DETECTOR_TYPE  			0x06
#define DISPLAY_ACTUATOR_TYPE  				0x07
#define LIGHT_ACTUATOR_TYPE    				0x08
#define SOUND_ACTUATOR_TYPE    				0x09

#define POSITION_SENSOR_GETPOSITION			0x01
#define SPEED_SENSOR_GETSPEED				0x02
#define SEAT_SENSOR_GETSEATSTATE			0x03
#define WEATHER_DETECTOR_GETROADSTATE			0x04
#define BICYCLE_DETECTOR_GETBICYCLESTATE		0x05
#define TRAFFICLIGHT_DETECTOR_GETTRAFFICLIGHTSTATE	0x06
#define DISPLAY_ACTUATOR_SETTEXTMESSAGE			0x07
#define LIGHT_ACTUATOR_SETLIGHTON			0x08
#define LIGHT_ACTUATOR_SETLIGHTOFF			0x09
#define SOUND_ACTUATOR_PLAYSOUND			0x0A


//TODO Crear una libreria (o fichero .h) general para procesar y manejar los dispositivos? 
#define MAX_NRF24_MSG_SIZE 32
#define MAX_MESSAGE_SIZE   23 //MAX_NRF24_MSG_SIZE - REDUCED_HMAC_SIZE - COUNTER_SIZE
#define MSG_HEADER_SIZE    3
#define MAX_DATA_SIZE      20 //MAX_MESSAGE_SIZE - MSG_HEADER_SIZE
#define MAX_HW_DEVICES     10

//Define NRF24 NODE ID
#define NRFNODEID        0x01

//Define security constans
#define KEY_SIZE           16
#define BLOCK_SIZE         16
#define HMAC_SIZE          16
#define REDUCED_HMAC_SIZE  8
#define COUNTER_SIZE       1

// Set up display, light and sound actuators
DisplayActuatorBasedOnGroveLCD displayActuator;
LightActuatorBasedOnGroveLED lightActuator(5);
SoundActuatorBasedOnGroveBuzzer soundActuator(2);

void* hwDevicesRef[MAX_HW_DEVICES] = {&displayActuator, &lightActuator, &soundActuator};
byte hwDevicesType[MAX_HW_DEVICES] = {DISPLAY_ACTUATOR_TYPE, LIGHT_ACTUATOR_TYPE, SOUND_ACTUATOR_TYPE};
char* dephisitDevicesId[MAX_HW_DEVICES] = {"DA:50:WarningDisplayActuator", "LA:50:WarningLightActuator", "SA:50:WarningSoundActuator"};

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

byte nrfMessage[MAX_NRF24_MSG_SIZE];
int bytesReceived = 0;
int bytesToSend = 0;
byte reqMessage[MAX_MESSAGE_SIZE];
byte resMessage[MAX_MESSAGE_SIZE];

// Set up AES and HMAC-MD5 objects
AES aesCipher;
MD5 md5Hash;

byte key[KEY_SIZE] = {0x6E,0x55,0x2A,0xA3,0x10,0x14,0xB4,0xD5,0x5E,0x1D,0x05,0xA6,0xB6,0xD8,0xBB,0xF5}; //Secret key generated randomly
byte lastReceivedCounter = 0;
byte lastSendedCounter = 0;

//                  |  1 byte   |   1 byte   |      1 byte       |                    20 bytes                    | 1 byte  |                    8 bytes                     |
//                   --------------------------------------------------------------------------------------------------------------------------------------------------------
// Messages format: | NrfNodeId | HwDeviceId | HwDeviceActionReq | HwDeviceActionReqData + Padding (random bytes) | Counter |  Reduced HMAC (First 8 bytes XOR Last 8 bytes) |
//                   --------------------------------------------------------------------------------------------------------------------------------------------------------

int generateReducedHmac(byte *data, int size, byte* reducedHmac) {
  
  //Calculate HMAC (hash message authentication code)
  byte hmac[HMAC_SIZE];
  //memset(hmac, 0, sizeof(hmac));
  md5Hash.hmac_md5(data, size, key, sizeof(key), hmac);
  printf("hmac[%d]: ", sizeof(hmac)); for (int i = 0; i < sizeof(hmac); i++){printf("%02x", hmac[i]);} printf("\n");
  
  //Generate reduced HMAC
  for(int i=0; i<REDUCED_HMAC_SIZE; i++) {
    reducedHmac[i] = hmac[0+i] ^ hmac[(HMAC_SIZE/2)+i];
  }
  printf("reducedHmac[%d]: ", REDUCED_HMAC_SIZE); for (int i = 0; i < REDUCED_HMAC_SIZE; i++){printf("%02x", reducedHmac[i]);} printf("\n");
}

int generateRandomBytes(byte *randomBytes, int numberOfBytes) {
  FILE *fp;
  //fp = fopen("/dev/random", "r"); //-> Only contains a few random bytes
  fp = fopen("/dev/urandom", "r");
  fread(randomBytes, 1, numberOfBytes, fp);
  fclose(fp);
  printf("randomBytes[%d]: ", numberOfBytes); for (int i = 0; i < numberOfBytes; i++){printf("%02x", randomBytes[i]);} printf("\n");
}

int responseMessageToSecureMessage(int resMessageSize) {
  
  byte plainData[BLOCK_SIZE*2];
  
  //memset(plainData, 0, sizeof(plainData));
  memcpy(&plainData[0], &resMessage[0], resMessageSize);
  
  //If needed, generate and add random bytes for padding
  if (resMessageSize < MAX_MESSAGE_SIZE) {
    generateRandomBytes(&plainData[resMessageSize], MAX_MESSAGE_SIZE-resMessageSize);
  }
  
  //Generate and add counter
  plainData[23] = ++lastSendedCounter;
  
  //Generate and add reduced HMAC
  generateReducedHmac(&plainData[0], 24, &plainData[24]);

  printf("responseMessageToSecureMessage - plainData[%d]: ", sizeof(plainData)); for (int i = 0; i < sizeof(plainData); i++){printf("%02x", plainData[i]);} printf("\n");
  
  //Encrypt the message to send using AES cipher in ECB mode (two blocks of 16 bytes).
  memset(nrfMessage, 0, sizeof(nrfMessage));
  aesCipher.encrypt(&plainData[0], &nrfMessage[0]);
  aesCipher.encrypt(&plainData[BLOCK_SIZE], &nrfMessage[BLOCK_SIZE]);
  
  printf("responseMessageToSecureMessage - nrfMessage[%d]: ", sizeof(nrfMessage)); for (int i = 0; i < sizeof(nrfMessage); i++){printf("%02x", nrfMessage[i]);} printf("\n");
  
  return 0;
}

int secureMessageToRequestMessage() {

  byte plainData[BLOCK_SIZE*2];

  printf("secureMessageToRequestMessage - nrfMessage[%d]: ", sizeof(nrfMessage)); for (int i = 0; i < sizeof(nrfMessage); i++){printf("%02x", nrfMessage[i]);} printf("\n");

  //Decrypt the message received using AES cipher in ECB mode (two blocks of 16 bytes).
  memset(plainData, 0, sizeof(plainData));
  aesCipher.decrypt(&nrfMessage[0], &plainData[0]);
  aesCipher.decrypt(&nrfMessage[BLOCK_SIZE], &plainData[BLOCK_SIZE]);
  printf("secureMessageToRequestMessage - plainData[%d]: ", sizeof(plainData)); for (int i = 0; i < sizeof(plainData); i++){printf("%02x", plainData[i]);} printf("\n");
  
  //Generate reduced HMAC
  byte reducedHmac[REDUCED_HMAC_SIZE];
  generateReducedHmac(&plainData[0], 24, reducedHmac);
  
  //Check reduced HMAC. This verify both the data integrity and the authentication of a message.
  if (memcmp(&plainData[24], reducedHmac, REDUCED_HMAC_SIZE) == 0) {
 
    //Get request message content
    memcpy(&reqMessage[0], &plainData[0], MAX_MESSAGE_SIZE);
    
    //Get received counter. Currently we do nothing with this counter.
    lastReceivedCounter = plainData[MAX_MESSAGE_SIZE];
    
    return 0;
    
  } else {
    return -1;
  }
}

int sendResponseMessage(byte hwDeviceId, byte hwDeviceAction, byte *hwDeviceActionResData, int hwDeviceActionResDataSize)
{
  memset(resMessage, 0, sizeof(resMessage));
  
  resMessage[0] = NRFNODEID;
  resMessage[1] = hwDeviceId;
  resMessage[2] = hwDeviceAction;
  
  int dataBytesToSend = min(hwDeviceActionResDataSize, MAX_DATA_SIZE);
  memcpy(&resMessage[3], hwDeviceActionResData, dataBytesToSend);
 
  //Encrypt response message (from resMessage array to nrfMessage array)
  responseMessageToSecureMessage(3+dataBytesToSend);
  
  //bytesToSend = MSG_HEADER_SIZE+dataBytesToSend;
  //printf("Sending %u bytes. Content ", bytesToSend); for (int i = 0; i < bytesToSend; i++){printf("%02x", resMessage[i]);} printf("\n");
  bytesToSend = BLOCK_SIZE*2; //All messages have 32 bytes size (AES block size * 2).
  printf("Sending %u bytes. Content ", bytesToSend); for (int i = 0; i < bytesToSend; i++){printf("%02x", nrfMessage[i]);} printf("\n");
  
  radio.stopListening();
  //boolean ok = radio.write(&resMessage, bytesToSend);
  boolean ok = radio.write(&nrfMessage, bytesToSend);
  radio.startListening();
  
  if (ok)
    printf("Response message sended correctly\n");
  else
    printf("Error sending response message\n");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  printf_begin();
  
  // Initialize devices
  displayActuator.begin();
  lightActuator.begin();
  soundActuator.begin();
  
  // Initialize AES key
  aesCipher.set_key(key, 16);
  
  // Initialize readio
  radio.begin();
  // Optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15); 
  // Set data rate
  radio.setDataRate(RF24_250KBPS);
  // Enable dynamic payloads
  radio.enableDynamicPayloads();
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  // Start listening over radio
  radio.startListening();
}

void loop() {

  // if there is data ready
  if (radio.available())
  {
    //Read message
    //memset(reqMessage, 0, sizeof(reqMessage));
    memset(nrfMessage, 0, sizeof(nrfMessage));
    bytesReceived = (byte) radio.getDynamicPayloadSize();
    
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      //done = radio.read(reqMessage, bytesReceived);
      done = radio.read(nrfMessage, bytesReceived);
    }
    //printf("Received %u bytes. Content: ", bytesReceived); for (int i = 0; i < bytesReceived; i++){printf("%02x", reqMessage[i]);} printf("\n");
    printf("Received %u bytes. Content: ", bytesReceived); for (int i = 0; i < bytesReceived; i++){printf("%02x", nrfMessage[i]);} printf("\n");
    
    if (bytesReceived == BLOCK_SIZE*2) { //If message size is correct. All messages have 32 bytes size (AES block size * 2).
      
      //Decrypt request message (from nrfMessage array to reqMessage array)
      if (secureMessageToRequestMessage() == 0) {
      
        //Process message
        byte nrfNodeId = reqMessage[0];
        byte hwDeviceId = reqMessage[1]-1;
        byte hwDeviceAction = reqMessage[2];
        byte *hwDeviceActionReqData = &reqMessage[3];
        byte *hwDeviceActionResData = NULL;
        int hwDeviceActionResDataSize = 0;
        
        if (nrfNodeId == NRFNODEID) //If this node is the target of the message 
        {
          switch(hwDevicesType[hwDeviceId])
          {
            /*case POSITION_SENSOR_TYPE:
              switch(hwDeviceAction){
                  //The result returned by getPosition must be a static variable
                  case POSITION_SENSOR_GETPOSITION: hwDeviceActionResData = (byte *)((PositionSensor*)hwDevicesRef[hwDeviceId])->getPosition(); hwDeviceActionResDataSize = 8; break;
                  default: printf("Unknown action for PositionSensor hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
              
            case SPEED_SENSOR_TYPE:
              switch(hwDeviceAction){
                  case SPEED_SENSOR_GETSPEED: hwDeviceActionResData = (byte *)((SpeedSensor*)hwDevicesRef[hwDeviceId])->getSpeed(); hwDeviceActionResDataSize = 2; break;
                  default: printf("Unknown action for SpeedSensor hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
            
            case SEAT_SENSOR_TYPE:
              switch(hwDeviceAction){
                  case SEAT_SENSOR_GETSEATSTATE: hwDeviceActionResData = (byte *)((SeatSensor*)hwDevicesRef[hwDeviceId])->getSeatState(); hwDeviceActionResDataSize = 1; break;
                  default: printf("Unknown action for SeatSensor hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
            
            case WEATHER_DETECTOR_TYPE:
              switch(hwDeviceAction){
                  case WEATHER_DETECTOR_GETROADSTATE: hwDeviceActionResData = (byte *)((WeatherDetector*)hwDevicesRef[hwDeviceId])->getRoadState(); hwDeviceActionResDataSize = 1; break;
                  default: printf("Unknown action for WeatherDetector hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
            
            case BICYCLE_DETECTOR_TYPE:
              switch(hwDeviceAction){
                  case BICYCLE_DETECTOR_GETBICYCLESTATE: hwDeviceActionResData = (byte *)((BicycleDetector*)hwDevicesRef[hwDeviceId])->getBicycleState(); hwDeviceActionResDataSize = 1; break;
                  default: printf("Unknown action for BicycleDetector hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
            
            case TRAFFICLIGHT_DETECTOR_TYPE:
              switch(hwDeviceAction){
                  case TRAFFICLIGHT_DETECTOR_GETTRAFFICLIGHTSTATE: hwDeviceActionResData = (byte *)((TrafficLightDetector*)hwDevicesRef[hwDeviceId])->getTrafficLightState(); hwDeviceActionResDataSize = 1; break;
                  default: printf("Unknown action for TrafficLightDetector hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;*/
            
            case DISPLAY_ACTUATOR_TYPE:
              switch(hwDeviceAction){
                  case DISPLAY_ACTUATOR_SETTEXTMESSAGE: ((DisplayActuator*)hwDevicesRef[hwDeviceId])->setTextMessage((char*)hwDeviceActionReqData); break;
                  default: printf("Unknown action for DisplayActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
              
            case LIGHT_ACTUATOR_TYPE:
              switch(hwDeviceAction){
                  case LIGHT_ACTUATOR_SETLIGHTON: ((LightActuator*)hwDevicesRef[hwDeviceId])->setLightOn(); break;
                  case LIGHT_ACTUATOR_SETLIGHTOFF: ((LightActuator*)hwDevicesRef[hwDeviceId])->setLightOff(); break;
                  default: printf("Unknown action for LightActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
              
            case SOUND_ACTUATOR_TYPE:
              switch(hwDeviceAction){
                  case SOUND_ACTUATOR_PLAYSOUND: ((SoundActuator*)hwDevicesRef[hwDeviceId])->playSound(); break;
                  default: printf("Unknown action for SoundActuator hardware device\n"); hwDeviceAction = INVALID_HWDEVICEACTION; break;
              }
              break;
              
            default:
              printf("Unknown hardware device\n"); hwDeviceId = INVALID_HWDEVICEID;
              break;
          }
          
          sendResponseMessage(hwDeviceId+1, hwDeviceAction, hwDeviceActionResData, hwDeviceActionResDataSize);
        }
        
      } else {
        printf("Incorrect (reduced) HMAC\n");
      }
      
    } else {
      printf("Incorrect message size\n");
    }
  }
}
