# Sodaq_HTS221

Arduino library for using the HTS221 sensor.

## Usage

Quick example:

```c

#include <Wire.h>
#include <Sodaq_HTS221.h>

#define DEBUG_STREAM SerialUSB

Sodaq_HTS221 humiditySensor;

void setup()
{
    DEBUG_STREAM.begin(115200);
    Wire.begin();
   
    if (humiditySensor.init()) {
        DEBUG_STREAM.println("Temperature + humidity sensor initialized.");
        humiditySensor.enableSensor();
    }
    else {
        DEBUG_STREAM.println("Temperature + humidity initialization failed!");
    }
    
    DEBUG_STREAM.println("Done with setup!");
}

void loop()
{
    DEBUG_STREAM.print("Humidity: ");
    DEBUG_STREAM.println(humiditySensor.readHumidity());
    DEBUG_STREAM.print("Temperature: ");
    DEBUG_STREAM.println(humiditySensor.readTemperature());
    
    delay(1000);
}

```

Method|Description
------|------
**enableSensor(TemperatureAveraging tempAvg = TemperatureAveraging16, HumidityAveraging humidAvg = HumidityAveraging32, ODR odr = OdrOneShot)**|Activates the sensor and optionally configures it with the passed averaging and datarate parameters. If called without parameters, the defaults are 16 samples averaging for temperature, 32 samples averaging for humidity and measurements only on-demand.
**enableHeater()**|Turns on the heater element of the sensor. Please note that the heater should not be on during sampling.
**disableHeater()**|Disables the heater element of the sensor.
**disableSensor()**|Disales the sensor completely and puts it in a low-power state.
**readTemperature()**|Triggers a measurement (if applicable) and returns the temperature (float).
**readHumidity()**|Triggers a measurement (if applicable) and returns the humidity value (float).

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request


## License

Copyright (c) 2017 SODAQ. All rights reserved.

This file is part of Sodaq_HTS221.

Sodaq_HTS221 is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or(at your option) any later version.

Sodaq_HTS221 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Sodaq_HTS221. If not, see
<http://www.gnu.org/licenses/>.
