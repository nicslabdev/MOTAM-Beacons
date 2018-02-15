/*
  DisplayLightSoundActuatorDemo - Demo code for use Actuator devices.
  Created by Manuel Montenegro, September 01, 2017.
  Developed for MOTAM project. 
*/

//Time between Display Actuator color changing in ms
#define colorChangeMillis 1
//Duration of soundActuator
#define durationTone 25

//Pin where is the LightActuator
#define pinLed 5
//Pin where is the SoundActuator
#define pinBuzzer 2

#include <rgb_lcd.h>

rgb_lcd lcd;

//Light Actuator status: on or off
int ledStatus = HIGH;

void setup() {
  initializeDisplayActuatorTest();
  initializeLightActuatorTest();
  initializeSoundActuatorTest();
}

void loop() {
  displayActuatorTest();
  lightActuatorTest();
  soundActuatorTest();
}


//Initialize functions ---------------------------
void initializeDisplayActuatorTest () {
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  //Write in first row
  lcd.print("MOTAM Proyect");
  //Write in second row
  lcd.setCursor(0,1);
  lcd.print("Actuators Test2");
}

void initializeLightActuatorTest () {
  pinMode(pinLed, OUTPUT);
}

void initializeSoundActuatorTest () {
  pinMode(pinBuzzer, OUTPUT);
}
//------------------------------------------------


//Test functions ---------------------------------
//Change the color of backlight in Display Actuator
void displayActuatorTest() {
  //Array with three components: Red, Green and Blue.
  int color [] = {255,255,255};
  for (int i=0; i<255; i++) {
    color[1]--;
    lcd.setRGB(color[0],color[1],color[2]);
    delay(colorChangeMillis);
  }
  for (int i=0; i<255; i++) {
    color[0]--;
    lcd.setRGB(color[0],color[1],color[2]);
    delay(colorChangeMillis);
  }
  for (int i=0; i<255; i++) {
    color[0]++;
    lcd.setRGB(color[0],color[1],color[2]);
    delay(colorChangeMillis);
  }
  for (int i=0; i<255; i++) {
    color[1]++;
    lcd.setRGB(color[0],color[1],color[2]);
    delay(colorChangeMillis);
  }
}

void lightActuatorTest() {
  ledStatus = !ledStatus;
  digitalWrite(pinLed, ledStatus);
}

void soundActuatorTest() {
  digitalWrite(pinBuzzer, HIGH);
  delay(durationTone);
  digitalWrite(pinBuzzer,LOW);
}
//------------------------------------------------





