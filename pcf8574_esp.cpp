//    FILE: PCF8574.H
//  ORIGINAL AUTHOR: Rob Tillaart
//    DATE: 02-febr-2013
// VERSION: 0.1.02
// PURPOSE: I2C PCF8574 library for Arduino
//     URL:
//
// Library modified by WereCatf

#include "pcf8574_esp.h"
#ifdef ARDUINO_AVR_DIGISPARK || ARDUINO_AVR_ATTINYX5
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

#ifdef ARDUINO_AVR_DIGISPARK || ARDUINO_AVR_ATTINYX5
PCF8574::PCF8574(uint8_t address)
{
  _Wire = TinyWireM;
  _address = address;
  _Wire.begin();
  PCF8574::write8(_pinModeMask);
}
#elif defined (ESP8266)
PCF8574::PCF8574(uint8_t address, int sda, int scl, TwoWire UseWire)
{
  _Wire = UseWire;
  _address = address;
  _Wire.begin(sda, scl);
  PCF8574::write8(_pinModeMask);
}
#else
PCF8574::PCF8574(uint8_t address, TwoWire UseWire)
{
  _Wire = UseWire;
  _address = address;
  _Wire.begin();
/*  For some reason Arduino Micro (Atmega32u4) crashes if you
  try to write data over I2C in class-constructor if setup() hasn't ran yet. 

  Commenting this out for now. */
  //PCF8574::write8(_pinModeMask);
}
#endif

uint8_t PCF8574::read8()
{
  _Wire.beginTransmission(_address);
  _Wire.requestFrom(_address, (uint8_t) 1);
  _data = _Wire.read();
  _error = _Wire.endTransmission();
  return _data;
}

void PCF8574::resetInterruptPin()
{
  read8();
}

uint8_t PCF8574::value()
{
  return _data;
}

void PCF8574::write8(uint8_t value)
{
  _Wire.beginTransmission(_address);
  _data = value;
  _pinModeMask = _data;
  _Wire.write(_data);
  _error = _Wire.endTransmission();
}

uint8_t PCF8574::read(uint8_t pin)
{
  PCF8574::read8();
  return (_data & (1<<pin)) > 0;
}

void PCF8574::write(uint8_t pin, uint8_t value)
{
  uint8_t _val = value & 1;
  if(_val) _pinModeMask |= _val << pin;
  else _pinModeMask &= ~(1 << pin);
  PCF8574::write8(_pinModeMask);
}

void PCF8574::toggle(uint8_t pin)
{
  _pinModeMask ^=  (1 << pin);
  PCF8574::write8(_pinModeMask);
}

void PCF8574::shiftRight(uint8_t n)
{
  if (n == 0 || n > 7 ) return;
  PCF8574::read8();
  _data >>= n;
  PCF8574::write8(_data);
}

void PCF8574::shiftLeft(uint8_t n)
{
  if (n == 0 || n > 7) return;
  PCF8574::read8();
  _data <<= n;
  PCF8574::write8(_data);
}

int PCF8574::lastError()
{
  int e = _error;
  _error = 0;
  return e;
}
