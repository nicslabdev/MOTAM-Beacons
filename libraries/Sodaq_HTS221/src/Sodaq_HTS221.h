#ifndef HTS221_H_
#define HTS221_H_

#include <Wire.h>

#define HTS221_ADDRESS 0x5F

// Registers
#define HTS221_WHO_AM_I         0x0F
#define HTS221_AV_CONF          0x10

#define HTS221_CTRL_REG1        0x20
#define HTS221_CTRL_REG2        0x21
#define HTS221_CTRL_REG3        0x22

#define HTS221_STATUS_REG       0x27

#define HTS221_HUMIDITY_OUT_L   0x28
#define HTS221_HUMIDITY_OUT_H   0x29

#define HTS221_TEMP_OUT_L       0x2A
#define HTS221_TEMP_OUT_H       0x2B

// Calibration registers
#define HTS221_H0_rH_x2         0x30
#define HTS221_H1_rH_x2         0x31

#define HTS221_T0_degC_x8       0x32
#define HTS221_T1_degC_x8       0x33

#define HTS221_T1_T0_msb        0x35

#define HTS221_H0_T0_OUT_L      0x36
#define HTS221_H0_T0_OUT_H      0x37

#define HTS221_H1_T0_OUT_L      0x3A
#define HTS221_H1_T0_OUT_H      0x3B

#define HTS221_T0_OUT_L         0x3C
#define HTS221_T0_OUT_H         0x3D

#define HTS221_T1_OUT_L         0x3E
#define HTS221_T1_OUT_H         0x3F

class Sodaq_HTS221
{
    public:
        // Configuration masks

        // number of samples when averaging temperature
        enum TemperatureAveraging {
            TemperatureAveraging2 = 0,
            TemperatureAveraging4,
            TemperatureAveraging8,
            TemperatureAveraging16,
            TemperatureAveraging32,
            TemperatureAveraging64,
            TemperatureAveraging128,
            TemperatureAveraging256,
        };
        
        // number of samples when averaging humidity
        enum HumidityAveraging {
            HumidityAveraging4 = 0,
            HumidityAveraging8,
            HumidityAveraging16,
            HumidityAveraging32,
            HumidityAveraging64,
            HumidityAveraging128,
            HumidityAveraging256,
            HumidityAveraging512,
        };
        
        // output data rate
        enum ODR {
            OdrOneShot = 0,
            Odr1Hz,
            Odr7Hz,
            Odr12Hz5,
        };
        
        enum RegisterBits {
            // AV_CONF
            TEMP_OFFSET = 3,
            HUMID_OFFSET = 0,
            
            // CTRL REG 1
            PD_CONTROL = 7,
            BDU_CONTROL = 2,
            ODR_CONTROL = 0,
            
            // CTRL REG 2
            HEATER = 1,
            
            // STATUS REG
            HUMID_AVAILABLE = 1,
            TEMP_AVAILABLE = 0
        };
        
        Sodaq_HTS221(TwoWire& wire = Wire, uint8_t address = HTS221_ADDRESS);
        bool init();
        void enableSensor(TemperatureAveraging tempAvg = TemperatureAveraging16, HumidityAveraging humidAvg = HumidityAveraging32, ODR odr = OdrOneShot);
        void enableHeater();
        void disableHeater();
        void disableSensor();
        float readTemperature();
        float readHumidity();
        
    private:
		TwoWire& _wire;
		uint8_t _address;
		uint16_t _data;
		int _error;
        ODR _currentOdr;

        // fields holding the calibration values
        // (with same notation as the datasheet)
		int16_t T0_OUT;
		int16_t T1_OUT;

		int16_t H0_T0_OUT;
		int16_t H1_T0_OUT;

		uint16_t T0_degC;
		uint16_t T1_degC;

		uint8_t H0_rH;
		uint8_t H1_rH;

        uint16_t readRegister(uint8_t reg, uint8_t bytes = 1);
        void writeRegister(uint8_t reg, uint8_t value);
        uint8_t getWhoami();
        uint16_t readU16LE(uint8_t reg);
        void waitForSensorData(uint8_t reg, uint8_t bit);
        void readCalibrationValues();
};

#endif
