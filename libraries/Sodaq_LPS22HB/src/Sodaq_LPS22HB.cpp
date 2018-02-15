#include <Arduino.h>

#include "Sodaq_LPS22HB.h"

#define LPS22HB_WHOAMI_VALUE 0xB1
#define ONESHOT_CHANGE_TIMEOUT 50 // safety timeout to avoid hangs, not found in specs

Sodaq_LPS22HB::Sodaq_LPS22HB(TwoWire& wire, uint8_t address) : 
    _wire(wire), 
    _address(address), 
    _data(0), 
    _error(0),
    _currentOdr(OdrOneShot)
{

}

bool Sodaq_LPS22HB::init()
{    
    if (getWhoami() != LPS22HB_WHOAMI_VALUE) {
        return false;
    }
 
    disableSensor();
    
    return true;
}

void Sodaq_LPS22HB::enableSensor(ODR odrSetting)
{
    // low pass filter off
    uint8_t reg1Value = (odrSetting << ODR_CONTROL) | (1 << BDU_CONTROL);
    writeRegister(LPS22HB_CTRL_REG1, reg1Value);

    _currentOdr = odrSetting;
}

void Sodaq_LPS22HB::disableSensor()
{
    enableSensor(OdrOneShot);
    _currentOdr = OdrOneShot;
}

uint8_t Sodaq_LPS22HB::getWhoami()
{
    return readRegister(LPS22HB_WHO_AM_I);
}

float Sodaq_LPS22HB::readPressureHPA()
{
    if (_currentOdr == OdrOneShot) {
        // set one-shot, fifo off, address increase off, boot & reset normal
        writeRegister(LPS22HB_CTRL_REG2, 0x01);

        // wait for sync (one-shot)
        uint32_t time = millis();
        while ((readRegister(LPS22HB_CTRL_REG2) & 0x01)
            && (millis() - time) < ONESHOT_CHANGE_TIMEOUT) {
            // do nothing
        }
    }
    else {
        // wait for sync (odr)
        uint32_t time = millis();
        while ((readRegister(LPS22HB_STATUS) & (1 << P_DA))
            && (millis() - time) < ONESHOT_CHANGE_TIMEOUT) {
            // do nothing
        }
    }

    uint8_t ldata = readRegister(LPS22HB_PRESS_OUT_L);
    uint8_t xldata = readRegister(LPS22HB_PRESS_OUT_XL);
    uint8_t hdata = readRegister(LPS22HB_PRESS_OUT_H);

    uint32_t pressureLSB = ((uint32_t)hdata << 16) | ((uint32_t)ldata << 8) | xldata;

    return pressureLSB / 4096.0f;
}

float Sodaq_LPS22HB::readTemperature()
{
    if (_currentOdr == OdrOneShot) {
        // set one-shot, fifo off, address increase off, boot & reset normal
        writeRegister(LPS22HB_CTRL_REG2, 0x01);

        // wait for sync (one-shot)
        uint32_t time = millis();
        while ((readRegister(LPS22HB_CTRL_REG2) & 0x01)
            && (millis() - time) < ONESHOT_CHANGE_TIMEOUT) {
            // do nothing
        }
    }
    else {
        // wait for sync (odr)
        uint32_t time = millis();
        while ((readRegister(LPS22HB_STATUS) & (1 << T_DA))
            && (millis() - time) < ONESHOT_CHANGE_TIMEOUT) {
            // do nothing
        }
    }

    uint8_t lsb = readRegister(LPS22HB_TEMP_OUT_L);
    uint8_t hsb = readRegister(LPS22HB_TEMP_OUT_H);

    uint16_t temp = ((uint16_t)hsb << 8) | lsb;
    
    return temp / 100.0f;
}

// bytes is 1 or 2
uint16_t Sodaq_LPS22HB::readRegister(uint8_t reg, uint8_t bytes)
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

void Sodaq_LPS22HB::writeRegister(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.write(value);
    _error = _wire.endTransmission();
}
