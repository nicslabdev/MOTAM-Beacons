/*********************************************************************************************/
/*
 * NB-IoT
 * Created by Manuel Montenegro, February 15, 2018.
 * Developed for MOTAM project. 
 * 
 *  This sketch uses SODAQ NB-IoT Shield for send some data from sensors: temperature and 
 *  atmospheric pressure.
 *  
 *  Compatible boards: Arduino/Genuino 101.
*/
/*********************************************************************************************/

#include <Wire.h>                   // I2C library
#include <Sodaq_LPS22HB.h>          // Temperature and pressure sensor's library
#include <SodaqNBIoTMotam.h>        // Library for doing MOTAM operations with SODAQ NB-IoT

#define USB Serial                  // Serial port for DEBUG
#define UBLOX Serial1               // Serial port for communication with NB-IoT module
#define powerPin 7                  // Pin to turn on/off the NB-IoT module

#define NIBBLE_TO_HEX_CHAR(i) ((i <= 9) ? ('0' + i) : ('A' - 10 + i))
#define HIGH_NIBBLE(i) ((i >> 4) & 0x0F)
#define LOW_NIBBLE(i) (i & 0x0F)

String imei;                        // Stores IMEI of ublox module
bool ledState;                      // For saves the LED indicator state


Sodaq_LPS22HB barSensor;            // Barometric and temperature sensor



void setUpBarSensor ( ) {

  Wire.begin();
  barSensor.enableSensor(Sodaq_LPS22HB::OdrOneShot);  
  
}



void sendPressTemp() {

  String message, data, len;

  data += stringToHexString ("IMEI: ");
  data += stringToHexString (imei);
  data += stringToHexString (" Press: ");
  data += stringToHexString (String(barSensor.readPressureHPA()));
  data += stringToHexString (" Temp: ");
  data += stringToHexString (String(barSensor.readTemperature()));

  len = String(data.length()/2);

  message += "AT+NSOST=0,79.114.88.15,16666,";
  message += len;
  message += ",";
  message += data;

  sendIt(message,100);
   
  
}


String stringToHexString (String val) {

  String hexString;

  for (uint8_t i = 0; i < val.length(); i++) {
    hexString += String(static_cast<char>(NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(val[i]))));
    hexString += String(static_cast<char>(NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(val[i]))));
  }

  return hexString;
  
}




void setUpUblox() {

  sendIt("AT",100);
  sendIt("AT+NRB",6000);
  sendIt("AT+CEREG=2", 100);
  sendIt("AT+CSCON=1", 100);
  sendIt("AT+CFUN=1", 6000);
  sendIt("AT+CGDCONT=0,\"IP\",\"\"",100);
  sendIt("AT+COPS=1,2,\"21401\"",30000);
  sendIt("AT+CGPADDR",100);
  sendIt("AT+NSOCR=DGRAM,17,10000,1",100);
  imei = getImei();
  
}



String getImei() {

  String received, receivedImei;

  String comm = "AT+CGSN=1";
  UBLOX.print (comm+"\r");

  delay (100);

  while (!UBLOX.available());
  while (UBLOX.available()) {
    received += (char)UBLOX.read ();
  }

  receivedImei = received.substring(8,23);

  printIt(received);
  printIt(receivedImei);

  return receivedImei;
  
}



void sendIt(String atCommand, int atDelay) {

  printIt("-- "+atCommand);
  UBLOX.print (atCommand+"\r");

  delay(atDelay);

  receiveIt();
  
}



String receiveIt ( ) {

  String received;

  while (!UBLOX.available());
  while (UBLOX.available()) {
    received += (char)UBLOX.read ();
  }

  printIt (received);

  return received;
  
}



void printIt ( String text ) {

  USB.println(text);
  
}



// ----------------------------------------------------------------------

void setup() {

  pinMode(powerPin, OUTPUT);        
  digitalWrite(powerPin, HIGH);     // Turn the NB-IoT module on

  pinMode (LED_BUILTIN, OUTPUT);
  
  USB.begin(9600);                  // Start serial port with USB
  UBLOX.begin(9600);                // Start serial port with NB-IoT shield
  while(!USB);

  setUpBarSensor();                 // Set pressure & temperature sensor
  setUpUblox();                     // Set NB-IoT module up

  ledState = HIGH;
  digitalWrite(LED_BUILTIN, ledState);
  
}



void loop() {

  sendPressTemp();

  delay(1000);

  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  
}
