#include <Arduino.h>
#include <Wire.h>

#include <Sodaq_LPS22HB.h>

#define DEBUG_STREAM SerialUSB

Sodaq_LPS22HB barometricSensor;

// the setup function runs once when you press reset or power the board
void setup()
{
    delay(5000);
    
    Wire.begin();
    
    DEBUG_STREAM.begin(115200);
    
    DEBUG_STREAM.println();
    
    if (barometricSensor.init()) {
        DEBUG_STREAM.println("Barometric sensor initialization succeeded!");
        barometricSensor.enableSensor(Sodaq_LPS22HB::OdrOneShot);
    }
    else {
        DEBUG_STREAM.println("Barometric sensor initialization failed!");
    }
    
    DEBUG_STREAM.println("Done with setup!");
}

// the loop function runs over and over again until power down or reset
void loop()
{
    DEBUG_STREAM.print("Pressure: ");
    DEBUG_STREAM.println(barometricSensor.readPressureHPA());
    DEBUG_STREAM.print("Temperature: ");
    DEBUG_STREAM.println(barometricSensor.readTemperature());
    
    delay(1000);
}

