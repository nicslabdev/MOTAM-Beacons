/*********************************************************************************************/
/*
 * NB-IoT
 * Created by Manuel Montenegro, February 22, 2018.
 * Developed for MOTAM project. 
 * 
 *  This sketch uses SODAQ NB-IoT Shield for send some data.
 *  
 *  Compatible boards: Arduino Leonardo & Arduino/Genuino 101.
*/
/*********************************************************************************************/

#include <Wire.h>                   // I2C library
#include <Sodaq_LPS22HB.h>          // Temperature and pressure sensor's library
#include <SodaqNBIoTMotam.h>        // Library for doing MOTAM operations with SODAQ NB-IoT


String SERVER_IP = "79.114.88.15";  // IP of UDP server
String SERVER_PORT = "16666";       // Port of UDP server


SodaqNBIoT nbiot;

String IP;                          // IP assigned to device from network
String IMEI;                        // IMEI's card inserted in SODAQ module
int socket;                         // Socket for sending & receiving data

void setup() {
  Serial.begin (9600);

  if ( nbiot.begin() ) {            // If device is registered in network
    IP = nbiot.getIP();
    socket = nbiot.openSocket (10000);
    IMEI = nbiot.getIMEI();
  }

}

void loop() {

  nbiot.sendData ("Hola Mundo!" ,socket, SERVER_IP, SERVER_PORT);
 
  delay (5000);

}
