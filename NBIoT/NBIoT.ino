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
#include <Sodaq_HTS221.h>           // Temperature and humidity sensor's library
#include <SodaqNBIoTMotam.h>        // Library for doing MOTAM operations with SODAQ NB-IoT


String SERVER_IP = "79.114.88.15";  // IP of UDP server
String SERVER_PORT = "16666";       // Port of UDP server


SodaqNBIoT nbiot;                   // Ublox module
Sodaq_LPS22HB barSensor;            // Barometric and temperature sensor
Sodaq_HTS221 humiSensor;            // Humidity and temperature sensor


String ip;                          // IP assigned to device from network
String imei;                        // IMEI's card inserted in SODAQ module
int socket;                         // Socket for sending & receiving data
String message;                     // Message that will be sent

void setup() {
  Serial.begin (9600);

  Wire.begin();                     // I2C connection with sensors started

  // Start barometric & temp sensor
  barSensor.init();
  barSensor.enableSensor(Sodaq_LPS22HB::OdrOneShot);  
  
  // Start humidity & temp sensor
  humiSensor.init();
  humiSensor.enableSensor();    

  // Start ublox modem
  if ( nbiot.begin() ) {            // If device is registered in network
    ip = nbiot.getIP();
    socket = nbiot.openSocket (10000);
    imei = nbiot.getIMEI();
  }

}

void loop() {

  String message;

  message += "IMEI: ";
  message += imei;
  message += " Pressure: ";
  message += String(barSensor.readPressureHPA());
  message += " Temp: ";
  message += String(humiSensor.readTemperature());
  message += " Humidity: ";
  message += String(humiSensor.readHumidity());
    
  nbiot.sendData (message ,socket, SERVER_IP, SERVER_PORT);
 
  delay (5000);

}
