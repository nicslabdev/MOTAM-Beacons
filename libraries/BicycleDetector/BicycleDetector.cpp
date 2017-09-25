/*
  BicycleDetector.cpp - Implementations of the sensors 
  that want to provide information about bicycle's state.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>

#include "BicycleDetector.h"

#define DEBOUNCE_DELAY 	   	40
#define TIME_TO_ACCIDENT	5000
#define TIME_TO_STOP		10000
#define SPEED_TO_ACCIDENT	5

#define BICYCLE_DIAMETER	0.56 //Bicycle diameter (in meters)
#define calculateBicycleSpeed(X) (BICYCLE_DIAMETER / (float)(X) * 3600) //Convert m/ms to km/h

volatile unsigned long lastTimeReed = 0;

volatile bool pAccident = false;
volatile unsigned long pAccidentTime = 0; 
volatile unsigned long lastReedDetection = 0; //4294967295; 
volatile unsigned long prevReedDetection = 0;

volatile int state = STOPPED;

#if defined(RFDUINO)	//RFduino
int onReedHigh(uint32_t ulPin)
#elif defined(ARDUINO_LINUX)	//Intel Galileo
void onReedChange()
#elif defined(AVR_UNO)	//Arduino Uno, only pins 2 (interrupt 0) and 3 (interrupt 1) will work correctly
void onReedChange()
#else
#error ARDUINO BOARD NOT SUPPORTED
#endif
{
  unsigned long currentTime = millis();
  if ((currentTime - lastTimeReed) > DEBOUNCE_DELAY)
  {
    switch(state)
    {
      case STOPPED: 
	    state = ONGOING;
		break;
      default: break;
    }
    prevReedDetection = lastReedDetection;
    lastReedDetection = millis();
	
	lastTimeReed = currentTime;
  }
}

BicycleDetectorBasedOn2TiltAndReed::BicycleDetectorBasedOn2TiltAndReed(int tilt1DigitalPin, int tilt2DigitalPin, int reedDigitalPin)
{
  _tilt1Pin = tilt1DigitalPin;
  
  _tilt2Pin = tilt2DigitalPin;

  _reedPin = reedDigitalPin;
}

void BicycleDetectorBasedOn2TiltAndReed::begin()
{
  pinMode(_tilt1Pin, INPUT);
  
  pinMode(_tilt2Pin, INPUT);
  
  pinMode(_reedPin, INPUT);
  #if defined(RFDUINO)	//RFduino
  RFduino_pinWakeCallback(_reedPin, HIGH, onReedHigh);
  #elif defined(ARDUINO_LINUX)	//Intel Galileo
  attachInterrupt(_reedPin, onReedChange, CHANGE);
  #elif defined(AVR_UNO)	//Arduino Uno, only pins 2 (interrupt 0) and 3 (interrupt 1) will work correctly
  attachInterrupt(_reedPin==3?1:0, onReedChange, CHANGE);
  #else
  #error ARDUINO BOARD NOT SUPPORTED
  #endif
}

int BicycleDetectorBasedOn2TiltAndReed::getBicycleState()
{
  int tilt1Value = digitalRead(_tilt1Pin);
  int tilt2Value = digitalRead(_tilt2Pin);
  int reedValue = digitalRead(_reedPin);
  
  unsigned int speedValue = calculateBicycleSpeed(lastReedDetection - prevReedDetection);
  
  Serial.print("T1: ");Serial.print(tilt1Value);Serial.print(", T2: ");Serial.print(tilt2Value);
  Serial.print(", R: ");Serial.print(reedValue);Serial.print(", S: ");Serial.println(speedValue);
  
  //NOTA: Los sensores "tilt" deben estar colocados con los pines hacia abajo
  switch (state)
  {
    case ONGOING:
	  if ((tilt1Value == LOW || tilt2Value == LOW) && !pAccident //Alguno de los sensores "tilt" detecta inclinacion (bicicleta tumbada)
	    && speedValue > SPEED_TO_ACCIDENT) //Y la ultima velocidad conocida era mayor que 5km/h
	  {
	    pAccident = true;
		pAccidentTime = millis();
	  }
	  else if (pAccident && tilt1Value == HIGH && tilt2Value == HIGH) //Ninguno de los sensores "tilt" detecta inclinacion (bicicleta de pie)
	  {
	    pAccident = false;
	  }
	  
      if (pAccident && (millis() - pAccidentTime > TIME_TO_ACCIDENT)) //Aviso de posible accidente hace 5 segundos y desde entonces bicicleta tumbada
      {
        state = ACCIDENT;
      }
      else if (millis() - lastReedDetection > TIME_TO_STOP) //Pasan mas de 10 segundos sin detectar giro de rueda -> parado
      {
        state = STOPPED;
		pAccident = false;
      }
      break;
	  
    case ACCIDENT:
	  if (tilt1Value == HIGH && tilt2Value == HIGH) //Ninguno de los sensores "tilt" detecta inclinacion (bicicleta de pie)
      {
        state = STOPPED;
      }
      break;
	  
    case STOPPED:
      break;
	  
    default: break;
  }
  
  return state;
}