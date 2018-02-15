# Sodaq_LPS22HB

Arduino library for using the LPS22HB sensor.

## Usage

Quick example:

```c

#include <Wire.h>
#include <Sodaq_LPS22HB.h>

#define DEBUG_STREAM SerialUSB

Sodaq_LPS22HB barometricSensor;

void setup()
{
    DEBUG_STREAM.begin(115200);
    Wire.begin();
   
    if (barometricSensor.init()) {
        DEBUG_STREAM.println("Barometric sensor initialization succeeded!");
        barometricSensor.enableSensor(Sodaq_LPS22HB::OdrOneShot);
    }
    else {
        DEBUG_STREAM.println("Barometric sensor initialization failed!");
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
**enableSensor(ODR odrSetting)**|Activates the sensor and optionally configures it with the passed datarate parameter.
**disableSensor()**|Disales the sensor completely and puts it in a low-power state.
**readTemperature()**|Triggers a measurement (if applicable) and returns the temperature (float).
**readPressureHPA()**|Triggers a measurement (if applicable) and returns the pressure value in hPa (float).

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## License

Copyright (c) 2017 SODAQ. All rights reserved.

This file is part of Sodaq_LPS22HB.

Sodaq_LPS22HB is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or(at your option) any later version.

Sodaq_LPS22HB is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Sodaq_LPS22HB. If not, see
<http://www.gnu.org/licenses/>.
