/*********************************************************************************************/
/*
 * NB-IoT Arduino library
 * Created by Manuel Montenegro, February 16, 2018.
 * Developed for MOTAM proyect. 
 * 
 *  This library manages communication with SODAQ NB-IOT SHIELD.
 *
 *	Compatible boards with this library: Arduino/Genuino 101.
*/
/*********************************************************************************************/

#ifndef __SODAQNBIOT_H__
#define __SODAQNBIOT_H__


#include "Arduino.h"


#define DEBUG Serial					// Serial port for DEBUG
#define UBLOX Serial1					// Serial port for comunnication with NB-IoT module
#define powerPin 7						// Pin to turn on/off the NB-IoT module
#define networkOperator "21401"			// Vodafone network operator code


class SodaqNBIoT {
public:
	SodaqNBIoT ();
	bool begin ();						// Init serial port and turn module on
	String getIP();						// Return IP of ublox module

private:
	bool setUpUblox ( );				// Start sending AT commands for module configuration

	bool isAlive();						// Check if ublox module responds to commands
	bool resetModule();					// Reset ublox module and check OK in response
	bool confNbiotConnection();			// Configure parameters for NB-IoT connection
	bool networkRegistration ();		// Select the operator and register SIM in network
	bool askForIp();					// Take IP of ublox module from the network

	bool checkRespForOk (int timeOut);	// Check if OK is in the response from ublox module 
	bool checkRespForReg (int timeOut);	// Check response looking for network registration
	String checkRespForIp(int timeOut);	// Check response looking for IP given by network

	void sendIt (String atCommand);		// Send an AT command to ublox module
	String receiveIt ();				// Receive data sended by ublox module
	void printIt (String text);			// Print a string by debug serial port

	String IP;							// IP that network give to ublox module
};


#endif