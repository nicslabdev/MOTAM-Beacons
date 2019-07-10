/*********************************************************************************************/
/*
 * NB-IoT Arduino library
 * Created by Manuel Montenegro, February 22, 2018.
 * Developed for MOTAM proyect. 
 * 
 *  This library manages communication with SODAQ NB-IOT SHIELD.
 *
 *	Compatible boards with this library: Arduino/Genuino 101.
*/
/*********************************************************************************************/

#ifndef __UBLOXNBIOT_H__
#define __UBLOXNBIOT_H__


#include "Arduino.h"


#define DEBUG 			Serial			// Serial port for DEBUG
#define UBLOX 			Serial1			// Serial port for comunnication with NB-IoT module
#define powerPin 		7				// Pin to turn on/off the NB-IoT module
#define networkOperator "21401"			// Vodafone network operator code


class UbloxNbIot {
public:
	UbloxNbIot ();
	bool begin ();						// Init serial port and turn module on
	String getIP();						// Return IP of ublox module
	String getIMEI();					// Return IMEI of card inserted in SODAQ module
	int openSocket (int port);			// Open an UDP socket in designated port
	bool sendData (String data, int sock, String ip, String port);	// Send data to ip and port
	String receiveData (int sock, int timeout); // Return received data from network

private:
	String IP;							// IP that network give to ublox module
	String imei;						// IMEI of SIM card inserted in SODAQ module

	bool setUpUblox ( );				// Start sending AT commands for module configuration

	bool isAlive ();					// Check if ublox module responds to commands
	bool resetModule ();				// Reset ublox module and check OK in response
	bool confNbiotConnection ();		// Configure parameters for NB-IoT connection
	bool networkRegistration ();		// Select the operator and register SIM in network
	bool askForIp ();					// Take IP of ublox module from the network
	bool askForImei ();					// Take IMEI's SIM from ublox module

	int checkIfDataReceived (int timeOut, int sock); // Check data in received buffer

	bool checkRespForOk (int timeOut);	// Check if OK is in the response from ublox module 
	bool checkRespForReg (int timeOut);	// Check response looking for network registration
	String checkRespForIp(int timeOut);	// Check response looking for IP given by network
	int checkRespForSocket(int timeOut);// Check response looking for socket opened
	String checkRespForImei(int timeOut);// Check response looking for IMEI
	int checkRespForDataSended(int timeOut, int sock);	// Check # of bytes sent in response
	String checkRespForDataReceived (int timeOut, int sock);// Return data received

	void sendIt (String atCommand);		// Send an AT command to ublox module
	String receiveIt ();				// Receive data sended by ublox module
	void printIt (String text);			// Print a string by debug serial port
	String stringToHexString(String str);	// Conversion from string to hexadecimal string
	String hexToAscii( String hex );	//Conversion from hexadecimal string to ASCII string

};


#endif