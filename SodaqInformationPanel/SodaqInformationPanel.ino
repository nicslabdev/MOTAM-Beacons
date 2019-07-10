/*********************************************************************************************/
/*
 * Sodaq Information Panel
 * Created by Manuel Montenegro, March 14, 2018.
 * Developed for MOTAM project. 
 * 
 *  This sketch uses SODAQ NB-IoT Shield for receiving some data.
 *  
 *  Compatible boards: Arduino Leonardo & Arduino/Genuino 101.
*/
/*********************************************************************************************/

#include <UbloxNbIot.h>             // Library for doing MOTAM operations with SODAQ NB-IoT
#include <rgb_lcd.h>                // Library for Grove LCD RGB

String SERVER_IP = "35.224.71.226"; // IP of UDP server
String SERVER_PORT = "10000";       // Port of UDP server

UbloxNBIoT nbiot;                   // Ublox module
rgb_lcd lcd;                        // RGB display

String ip;                          // IP assigned to device from network
String imei;                        // IMEI's card inserted in SODAQ module
int socket;                         // Socket for sending & receiving data
String messageSended;                     // Message that will be sent
String messageReceived;             // Message received from network


void setup() 
{
  Serial.begin (9600);

  while (!Serial);

  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  lcd.clear();

  // Start ublox modem
  if ( nbiot.begin() ) {            // If device is registered in network
    socket = nbiot.openSocket (10000);
  }  
}

void loop() 
{
  messageSended = "?\n";
  
  nbiot.sendData (messageSended ,socket, SERVER_IP, SERVER_PORT);

  messageReceived = nbiot.receiveData (socket, 5000);

  if (messageReceived != "") {

    messageReceived[messageReceived.length()-1] = ' ';

    lcd.print(messageReceived);
    lcd.setCursor(0,0);
    
  }  
}
