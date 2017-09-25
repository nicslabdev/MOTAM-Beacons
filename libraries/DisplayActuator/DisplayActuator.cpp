/*
  DisplayActuator.cpp - Implementations of the actuators 
  that want to show text messages to user.
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>

#include "DisplayActuator.h"

#define LCD_COLS	16
#define LCD_ROWS	2

DisplayActuatorBasedOnGroveLCD::DisplayActuatorBasedOnGroveLCD()
{
}

void DisplayActuatorBasedOnGroveLCD::begin()
{
  // set up the number of columns and rows on the LCD 
  _lcd.begin(LCD_COLS, LCD_ROWS);
}

void DisplayActuatorBasedOnGroveLCD::setTextMessage(const char* message)
{
  //TODO Implement the logic in line with the hardware 
  char line1[LCD_COLS+1], line2[LCD_COLS+1];
  int len = strlen(message);
  
  memset(line1, 0, sizeof(line1));
  strncpy(line1, message, min(LCD_COLS, len));
  memset(line2, 0, sizeof(line2));
  strncpy(line2, message+LCD_COLS, min(LCD_COLS, max(0, len-LCD_COLS)));
  
  _lcd.clear();
  _lcd.setCursor(0,0);
  _lcd.print(line1);
  _lcd.setCursor(0,1);
  _lcd.print(line2);
}