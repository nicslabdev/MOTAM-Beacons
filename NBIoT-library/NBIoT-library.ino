/*********************************************************************************************/
/*
 * NB-IoT
 * Created by Manuel Montenegro, February 19, 2018.
 * Developed for MOTAM p. 
 * 
 *  This sketch uses SODAQ NB-IoT Shield for send some data.
*/
/*********************************************************************************************/

#include <Wire.h>                   // I2C library
#include <Sodaq_LPS22HB.h>          // Temperature and pressure sensor's library
#include <SodaqNBIoTMotam.h>        // Library for doing MOTAM operations with SODAQ NB-IoT


SodaqNBIoT nbiot;


void setup() {
  Serial.begin (9600);
  while (!Serial);

  nbiot.getIP(); 

  bool success = nbiot.begin();
  Serial.print("Registro: ");Serial.println(success);

}

void loop() {
  // put your main code here, to run repeatedly:

}
