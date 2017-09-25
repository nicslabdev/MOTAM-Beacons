/*
  WeatherDetectorDemo - Demo code for use WeatherDetector devices.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/

#include <WeatherDetector.h>
#include <DHT.h>

WeatherDetectorBasedOnGRainAndGHTPro weatherDetector(2, 6);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  weatherDetector.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int res = weatherDetector.getRoadState();
  switch (res)
  {
    case DRY: Serial.println("The road is DRY"); break;
    case WET: Serial.println("The road is WET"); break;
    case ICE: Serial.println("The road has ICE"); break;
    case SNOW: Serial.println("The road has SNOW"); break;
    default: Serial.println("Unknown road's state");
  }
  
  delay(1000);
}