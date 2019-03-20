#include "Arduino.h"

#define USB Serial 
#define UBLOX Serial1

// Pin to turn on/off the nb-iot module
unsigned long baud = 115200;  //start at 9600 allow the USB port to change the Baudrate


void setup() 
{
  delay(100);

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
