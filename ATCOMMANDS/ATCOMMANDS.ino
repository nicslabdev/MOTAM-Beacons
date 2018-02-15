#include "Arduino.h"

#define USB Serial 
#define UBLOX Serial1

// Pin to turn on/off the nb-iot module
#define powerPin 7 
unsigned long baud = 9600;  //start at 9600 allow the USB port to change the Baudrate


void setup() 
{
  // Turn the nb-iot module on
  pinMode(powerPin, OUTPUT); 
  digitalWrite(powerPin, HIGH);  

  // Start communication
  USB.begin(baud);
  UBLOX.begin(baud);

}

// Forward every message to the other serial
void loop() 
{
  while (USB.available())
  {
    uint8_t c = USB.read();
    UBLOX.write(c);
  }

  while (UBLOX.available())
  {     
    USB.write(UBLOX.read());
  }

}

