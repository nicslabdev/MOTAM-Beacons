#include <Arduino.h>
#include <Wire.h>

#include <Sodaq_HTS221.h>

#define DEBUG_STREAM SerialUSB

Sodaq_HTS221 humiditySensor;

// the setup function runs once when you press reset or power the board
void setup()
{
    delay(5000);
    
    Wire.begin();
    
    DEBUG_STREAM.begin(115200);
    
    DEBUG_STREAM.println();
    
    if (humiditySensor.init()) {
        DEBUG_STREAM.println("Temperature + humidity sensor initialized.");
        humiditySensor.enableSensor();
    }
    else {
        DEBUG_STREAM.println("Temperature + humidity initialization failed!");
    }
    
    DEBUG_STREAM.println("Done with setup!");
}

// the loop function runs over and over again until power down or reset
void loop()
{
    DEBUG_STREAM.print("Humidity: ");
    DEBUG_STREAM.println(humiditySensor.readHumidity());
    DEBUG_STREAM.print("Temperature: ");
    DEBUG_STREAM.println(humiditySensor.readTemperature());
    
    delay(1000);
}
