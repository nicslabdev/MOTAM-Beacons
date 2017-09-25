/*
  Definitions to use nRF24L01 Arduino library (https://github.com/maniacbug/RF24) in Intel Galileo platform.
  Created by Jesus Rodriguez, May 13, 2015.
  Developed for DEPHISIT project. 
*/

#if defined(__ARDUINO_X86__)

//JESUS - Define _BV and printf_P macros (necessary for Intel Galileo) ----------
//The following macros must be defined in the RF24 library (specifically in the RF24_config.h file) for the
//compilation takes place without errors in the Intel Galileo platform. Copy and paste the following piece of
//code in the RF24_config.h file after the end of "#ifdef ARDUINO" sentence.
/*------------------- Copy and Paste -------------------
#if defined(__ARDUINO_X86__)
#define _BV(bit) (1 << (bit))
#define printf_P printf
#endif
------------------------------------------------------*/
//JESUS -------------------------------------------------------------------------

//JESUS - Define printf_begin() function (necesary for Intel Galileo) -----------
void printf_begin(void)
{
  //Redirect stdout stream to /dev/ttyGS0 (Serial Monitor port)
  stdout = freopen("/dev/ttyGS0", "w", stdout);
}
//JESUS -------------------------------------------------------------------------

#endif
