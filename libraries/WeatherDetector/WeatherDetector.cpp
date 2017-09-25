/*
  WeatherDetector.cpp - Implementations of the sensors 
  that want to provide information about weather road  
  conditions.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>
#include <DHT.h>

#include "WeatherDetector.h"

#define ICEMIN	0
#define ICEMAX	3

// -> Con la definicion de estas funciones el codigo ocupa el 8% de la capacidad maxima de los RFduino
// -> Sin la definicion de estas funciones el codigo ocupa el 56% de la capacidad maxima de los RFduino
//void* operator new(size_t size) { return malloc(size); } 
//void operator delete(void* ptr) { free(ptr); }

WeatherDetectorBasedOnGRainAndGHTPro::WeatherDetectorBasedOnGRainAndGHTPro(int rainDigitalPin, int htDigitalPin)
{
  _rainPin = rainDigitalPin;
  
  //DHT dht(htDigitalPin, DHT22);
  //_dhtPointer = (DHT *) malloc(sizeof(DHT));
  //_dhtPointer->DHT(htDigitalPin, DHT22); -> 
  _htPin = htDigitalPin;
  _dhtPointer = new DHT(_htPin, DHT22);
}

void WeatherDetectorBasedOnGRainAndGHTPro::begin()
{
  pinMode(_rainPin, INPUT);

  _dhtPointer->begin();
}

int WeatherDetectorBasedOnGRainAndGHTPro::getRoadState()
{
  int rainValue = digitalRead(_rainPin);
  float tempValue = _dhtPointer->readTemperature();
  //float humValue = _dhtPointer->readHumidity();
  
  //Serial.println(_rainPin); //-> El valor sin agua es de 450-500 y el valor con agua <400 (en funcion de la cantidad)
  Serial.print("R: "); Serial.print(rainValue); Serial.print(", T: "); Serial.println(tempValue);
  
  if (rainValue == HIGH)
  {
    return DRY;
  }
  else if (isnan(tempValue))
  {
	return UNKNOWN;
  }
  else if (rainValue == LOW && tempValue > ICEMAX)
  {
    return WET;
  }
  else if (rainValue == LOW && tempValue <= ICEMAX && tempValue >= ICEMIN)
  {
    return ICE;
  }
  else if (rainValue == LOW && tempValue < ICEMIN)
  {
    return SNOW;
  }
  else
  {
    return UNKNOWN;
  }
}