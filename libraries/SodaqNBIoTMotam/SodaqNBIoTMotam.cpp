/*********************************************************************************************/
/*
 * NB-IoT Arduino library
 * Created by Manuel Montenegro, February 16, 2018.
 * Developed for MOTAM proyect. 
 * 
 *  This library manages communication with SODAQ NB-IOT SHIELD.
*/
/*********************************************************************************************/

#include <SodaqNBIoTMotam.h>

SodaqNBIoT::SodaqNBIoT () { 
	IP = "";
}


// Start ublox n211 module
bool SodaqNBIoT::begin () {

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);		// Turn the NB-IoT module on

	UBLOX.begin(9600);					// Start serial port with ublox
	DEBUG.begin(9600);					// Start serial port for debugging

	while(!UBLOX);						// Wait until ublox's serial connection is stablished

	return setUpUblox ( );				// Start the setting up of ublox module

}

// Return the IP that network assigns to the NB-IoT module
String SodaqNBIoT::getIP() {
	return IP;
}



/* Set the NB-IoT module up sending AT commands. Register in the network and saves the IMEI 
	of the device.
	Return true if connection is successful or false otherwise*/
bool SodaqNBIoT::setUpUblox ( ) {

	bool flag;							// Flag for check the connection

	flag = isAlive();					// Check if there are connection with nbiot module

	if (flag) {
		flag = resetModule ();
	}

	if (flag) {
		flag = confNbiotConnection ();
	}
	if (flag) {
		flag = networkRegistration ();
	}
	if (flag) {
		flag = askForIp ();
	}


	return flag;

}



// Return true if ublox module responds to AT commands or false otherwise
bool SodaqNBIoT::isAlive() {

	sendIt ("AT");						// Send AT command

	return checkRespForOk ( 1000 );		// Check the response during time out

}


// Reset the module. Return true if successful
bool SodaqNBIoT::resetModule() {

	sendIt ("AT+NRB");					// Send AT command

	return checkRespForOk ( 10000 );	// Check the response during time out

}


// Configure parameters on the module for NB-IoT Connection
bool SodaqNBIoT::confNbiotConnection () {

	bool flag = true;					// Flag for checking correct working of AT commands

	if (flag) {
		sendIt("AT+CEREG=2");
		flag = checkRespForOk (500);
	}

	if (flag) {
		sendIt("AT+CSCON=0");
		flag = checkRespForOk (500);
	}
	if (flag) {
		sendIt("AT+CFUN=1");
		flag = checkRespForOk (6000);
	}
	if (flag) {
		sendIt("AT+CGDCONT=0,\"IP\",\"\"");
		flag = checkRespForOk (500);
	}

	return flag;

}


// Select the operator and register SIM in the network
bool SodaqNBIoT::networkRegistration () {

	String atCommand = "AT+COPS=1,2,\"";// Building AT Command
	atCommand += networkOperator;		// Network operator is defined in .h
	atCommand += "\"";
	sendIt (atCommand);					// Send AT command

	return checkRespForReg ( 180000 );	// Check the response during time out

}


// Saves the IP that NB-IoT network gives to ublox module
bool SodaqNBIoT::askForIp ( ) {

	String receivedIp;

	sendIt("AT+CGPADDR");				// Send AT command

	receivedIp = checkRespForIp (500);	// Receive and parse the IP

	if ( receivedIp.length() > 0 ) {
		IP = receivedIp;
		return true;
	} else {
		return false;
	}

}



/* Check for OK in the response from ublox module
	return true if OK is in the response or false if time out */
bool SodaqNBIoT::checkRespForOk ( int timeOut ) {

	String response;
	bool ok = false;
	unsigned long startTime = millis();	// Take time at start for time out

	// Check every char string received for "OK" response
	while ( !ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response = receiveIt();
		ok = (response.indexOf("\r\nOK\r\n") >= 0);	// Check if "OK" is in the response
	}

	return ok;
}


/* Check for network registration in the response from ublox module
	return true if registration is done or false if time out */
bool SodaqNBIoT::checkRespForReg ( int timeOut ) {

	String response;
	bool ok = false;
	unsigned long startTime = millis();	// Take time at start for time out

	// Check every char string received for "OK" response
	while ( !ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response = receiveIt();
		ok = (response.indexOf("\r\n+CEREG:5,") >= 0);	// Check if "OK" is in the response
	}

	return ok;
}



/* Check for IP given by network in the response from ublox module
	return the IP in string format */
String SodaqNBIoT::checkRespForIp ( int timeOut ) {

	String response;
	String receivedIp;
	bool ok = false;
	unsigned long startTime = millis();	// Take time at start for time out

	while ( !ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response = receiveIt();
		if (response.indexOf("\r\n+CGPADDR:") >= 0) {
			receivedIp = response.substring((1+response.indexOf(",")));
			for (int i = 0; i < receivedIp.length(); i++) {
				DEBUG.println(receivedIp[i], HEX);
			}
			DEBUG.print("Direccion IP: ");
			DEBUG.println(receivedIp);
		}
		// ok = (response.indexOf("\r\n+CEREG:5,") >= 0);	// Check if "OK" is in the response
	}

	return "";

}
	








// Send an AT command to ublox module
void SodaqNBIoT::sendIt ( String atCommand ) {

	printIt("-- "+atCommand);			// Print AT command sended
	UBLOX.print (atCommand+"\r");		// Send AT command to ublox module

}




// Receive the response from ublox module. Return it and print it by debug serial port
String SodaqNBIoT::receiveIt ( ) {

	String received;

	// Save data received from ublox module
	while (UBLOX.available()) {
		delay(10);						// Neccesary for don't split strings
		received += (char)UBLOX.read ();
	}

	printIt (received);					// DEBUG: Print response from ublox module

	return received;					// Return string received
  
}



// Print something by DEBUG serial port
void SodaqNBIoT::printIt ( String text ) {

	if (text.length() > 0) {			// If string is not empty...
		DEBUG.println(text);			// Send by debug serial port the string
	}
	
  
}