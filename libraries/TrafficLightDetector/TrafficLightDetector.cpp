/*
  TrafficLightDetector.cpp - Implementations of the sensors 
  that want to provide information about traffic light's state.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>

#include "TrafficLightDetector.h"

#define TIME_TO_OFF		5000

volatile unsigned long lastRedOn = 0; 
volatile unsigned long lastYellowOn = 0; 
volatile unsigned long lastGreenOn = 0; 

volatile int state = OFF;

TrafficLightDetectorBasedOn3LDR::TrafficLightDetectorBasedOn3LDR(int ldrRedDigitalPin, int ldrYellowDigitalPin, int ldrGreenDigitalPin)
{
	_ldrRedPin = ldrRedDigitalPin;
	
	_ldrYellowPin = ldrYellowDigitalPin;
	
	_ldrGreenPin = ldrGreenDigitalPin;
}

void TrafficLightDetectorBasedOn3LDR::begin()
{
  pinMode(_ldrRedPin, INPUT);
  
  pinMode(_ldrYellowPin, INPUT);
  
  pinMode(_ldrGreenPin, INPUT);
}
  
int TrafficLightDetectorBasedOn3LDR::getTrafficLightState()
{
  int ldrRedValue = digitalRead(_ldrRedPin);
  int ldrYellowValue = digitalRead(_ldrYellowPin);
  int ldrGreenValue = digitalRead(_ldrGreenPin);
  
  //Serial.print(state);Serial.print(" - ");Serial.print(ldrRedValue);Serial.print(" ");Serial.print(ldrYellowValue);Serial.print(" ");Serial.println(ldrGreenValue);
  
  char hwState = ldrRedValue * 4 + ldrYellowValue * 2 + ldrGreenValue * 1; 
  
  switch (hwState)
  {
    case 0x00:
	  if (state != FLASHING_RED && state != FLASHING_YELLOW)
	  {
	    state = OFF;
	  }
	  else if (state == FLASHING_RED && (millis() - lastRedOn > TIME_TO_OFF))
	  {
		state = OFF;
	  }
	  else if (state == FLASHING_YELLOW && (millis() - lastYellowOn > TIME_TO_OFF))
	  {
		state = OFF;
	  }
	  break;
    case 0x01:
	  lastGreenOn = millis();
      state = GREEN;
	  break;
	case 0x02:
	  if ((state == OFF || state == FLASHING_YELLOW) && lastYellowOn > max(lastRedOn, lastGreenOn))
	  {
	    lastYellowOn = millis();
        state = FLASHING_YELLOW;
	  }
	  else
	  {
	    lastYellowOn = millis();
        state = YELLOW;
	  }
	  break;
	case 0x04:
	  if ((state == OFF || state == FLASHING_RED) && lastRedOn > max(lastYellowOn, lastGreenOn))
	  {
	    lastRedOn = millis();
	    state = FLASHING_RED;
	  }
	  else
	  {
	    lastRedOn = millis();
	    state = RED;
	  }
	  break;
	default: state = UNKNOWN;
  }
  
  return state;
}
