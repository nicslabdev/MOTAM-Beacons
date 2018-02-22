#include <Arduino.h>
#include "Sodaq_HTS221.h"

#define HTS221_WHOAMI_VALUE 0xBC
#define ONESHOT_CHANGE_TIMEOUT 80 // safety net to avoid hangs, based on the ODR of 12.5 hz

Sodaq_HTS221::Sodaq_HTS221(TwoWire& wire, uint8_t address) : 
    _wire(wire), 
    _address(address), 
    _data(0), 
    _error(0),
    _currentOdr(OdrOneShot)
{

}

bool Sodaq_HTS221::init()
{
    if (getWhoami() != HTS221_WHOAMI_VALUE) {
        return false;
    }
    
    disableHeater();
    
    readCalibrationValues();
    
    return true;
}

void Sodaq_HTS221::readCalibrationValues()
{
    this->T0_OUT = readU16LE(HTS221_T0_OUT_L);
    this->T1_OUT = readU16LE(HTS221_T1_OUT_L);

    this->H0_T0_OUT = readU16LE(HTS221_H0_T0_OUT_L);
    this->H1_T0_OUT = readU16LE(HTS221_H1_T0_OUT_L);

    uint8_t msb = readRegister(HTS221_T1_T0_msb);
    uint8_t T0_msb = (msb & 0b0011);
    uint8_t T1_msb = (msb & 0b1100) >> 2;
    this->T0_degC = readRegister(HTS221_T0_degC_x8) | (T0_msb << 8);
    this->T1_degC = readRegister(HTS221_T1_degC_x8) | (T1_msb << 8);

    this->H0_rH = readU16LE(HTS221_H0_rH_x2);
    this->H1_rH = readU16LE(HTS221_H1_rH_x2);
}

void Sodaq_HTS221::enableSensor(TemperatureAveraging tempAvg, HumidityAveraging humidAvg, ODR odr)
{
    uint8_t confRegValue = (tempAvg << TEMP_OFFSET) | (humidAvg << HUMID_OFFSET);
    writeRegister(HTS221_AV_CONF, confRegValue);
    
    uint8_t ctrlRegValue = (1 << PD_CONTROL) | (1 << BDU_CONTROL) | (odr << ODR_CONTROL);
    writeRegister(HTS221_CTRL_REG1, ctrlRegValue);

    _currentOdr = odr;
}

void Sodaq_HTS221::disableSensor()
{
    uint8_t ctrlRegValue = 0; // device off (PD), BDU off, ODR set to one-shot
    writeRegister(HTS221_CTRL_REG1, ctrlRegValue);

    _currentOdr = OdrOneShot;
}

// Heating speeds up reads, but can't use reading while heater is enabled.
void Sodaq_HTS221::enableHeater()
{
    uint8_t ctrlRegValue = (1 << HEATER); // the remaining bits can be left 0
    writeRegister(HTS221_CTRL_REG2, ctrlRegValue);
}

void Sodaq_HTS221::disableHeater()
{
    uint8_t ctrlRegValue = (0 << HEATER); // the remaining bits can be left 0
    writeRegister(HTS221_CTRL_REG2, ctrlRegValue);
}

uint8_t Sodaq_HTS221::getWhoami()
{
    return readRegister(HTS221_WHO_AM_I);
}

float Sodaq_HTS221::readTemperature()
{
    if (_currentOdr == OdrOneShot) {
        // set one-shot, while forcing the heater off (BOOT bit should be 0 anyway)
        writeRegister(HTS221_CTRL_REG2, 0x01);
    }

    // wait for sync (either one-shot or ODR cycle)
    waitForSensorData(HTS221_STATUS_REG, TEMP_AVAILABLE);
    int16_t T_OUT = readU16LE(HTS221_TEMP_OUT_L);
    
    float dTemp = (T_OUT - this->T0_OUT) / (float)(this->T1_OUT - this->T0_OUT);
    dTemp = this->T0_degC + dTemp * (this->T1_degC - T0_degC);
    dTemp /= 8;
    
    return dTemp;
}

float Sodaq_HTS221::readHumidity()
{
    if (_currentOdr == OdrOneShot) {
        // set one-shot, while forcing the heater off (BOOT bit should be 0 anyway)
        writeRegister(HTS221_CTRL_REG2, 0x01);
    }

    // wait for sync (either one-shot or ODR cycle)
    waitForSensorData(HTS221_STATUS_REG, HUMID_AVAILABLE);
    int16_t H_OUT = readU16LE(HTS221_HUMIDITY_OUT_L);
    
    float dHumidity = (H_OUT - this->H0_T0_OUT) / (float)(this->H1_T0_OUT - this->H0_T0_OUT);
    dHumidity = this->H0_rH + dHumidity * (this->H1_rH - H0_rH);
    dHumidity /= 2;
    
    return dHumidity;
}

uint16_t Sodaq_HTS221::readU16LE(uint8_t reg)
{
    uint8_t lsb = readRegister(reg);
    uint8_t hsb = readRegister(reg + 1);
    
    return ((uint16_t)hsb << 8) | lsb;
}

void Sodaq_HTS221::waitForSensorData(uint8_t reg, uint8_t bit)
{
    uint32_t time = millis();
    
    while ((readRegister(HTS221_STATUS_REG) & bit) 
            && (millis() - time) < ONESHOT_CHANGE_TIMEOUT) {
            // do nothing
    }
}

// bytes is 1 or 2
uint16_t Sodaq_HTS221::readRegister(uint8_t reg, uint8_t bytes)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _error = _wire.endTransmission();
    
    if (_error != 0) {
        return _data; // last value
    }
    
    if (_wire.requestFrom(_address, (uint8_t)bytes) != bytes) {
        _error = 10;
        return _data; // last value
    }
    
    _data = _wire.read();
    
    if (bytes == 2) {
        _data = _data * 256 + _wire.read();
    }
    
    return _data;
}

void Sodaq_HTS221::writeRegister(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.write(value);
    _error = _wire.endTransmission();
}
