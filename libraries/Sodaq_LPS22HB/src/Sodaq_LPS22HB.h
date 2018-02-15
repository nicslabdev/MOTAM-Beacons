#ifndef LPS22HB_H_
#define LPS22HB_H_

#include <Wire.h>

#define LPS22HB_ADDRESS 0x5D

// REGISTERS
#define LPS22HB_WHO_AM_I        0x0F // Device Who am I
#define LPS22HB_CTRL_REG1       0x10 // Control register 2
#define LPS22HB_CTRL_REG2       0x11 // Control register 2
#define LPS22HB_PRESS_OUT_XL    0x28 // Pressure output value (LSB) 
#define LPS22HB_PRESS_OUT_H     0x2A // Pressure output value (LSB) 
#define LPS22HB_PRESS_OUT_L     0x29 // Pressure output value (LSB) 
#define LPS22HB_TEMP_OUT_L      0X2B // Temperature output value (LSB)
#define LPS22HB_TEMP_OUT_H      0X2C // Temperature output value (MSB)
#define LPS22HB_STATUS          0x27 // Status register

class Sodaq_LPS22HB
{
    public:
        // Configuration Masks
        
        // Output Data Rate
        enum ODR {
            OdrOneShot = 0,
            Odr1Hz,
            Odr10Hz,
            Odr25Hz,
            Odr50Hz,
            Odr75Hz
        };
        
        enum RegisterBits {
            // CTRL_REG1
            ODR_CONTROL = 4,
            BDU_CONTROL = 1,

            // STATUS
            T_DA = 1,
            P_DA = 0,
        };
        
        Sodaq_LPS22HB(TwoWire& wire = Wire, uint8_t address = LPS22HB_ADDRESS);
        bool init();
        void enableSensor(ODR odrSetting);
        void disableSensor();
        float readPressureHPA();
        float readTemperature();
        
    private:
        TwoWire& _wire;
        uint8_t _address;
        uint16_t _data;
        int _error;
        ODR _currentOdr;

        uint16_t readRegister(uint8_t reg, uint8_t bytes = 1);
        void writeRegister(uint8_t reg, uint8_t value);
        uint8_t getWhoami();
};

#endif
