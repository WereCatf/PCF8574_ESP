/* PCF8574_ESP -- library for using the I2C-driven 8-pin GPIO-expander
   ORIGINAL AUTHOR: Rob Tillaart
   Library modified by WereCatf */

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
}

void PCF8574::begin(uint8_t defaultValues)
{
  _Wire.begin();
  PCF8574::write8(defaultValues);
}

#elif defined (ESP8266)
PCF8574::PCF8574(uint8_t address, int sda, int scl, TwoWire UseWire)
{
  _Wire = UseWire;
  _address = address;
  _sda = sda;
  _scl = scl;
}

void PCF8574::begin(uint8_t defaultValues)
{
  _Wire.begin(_sda, _scl);
  PCF8574::write8(defaultValues);
}

#else
PCF8574::PCF8574(uint8_t address, TwoWire UseWire)
{
  _Wire = UseWire;
  _address = address;
}

void PCF8574::begin(uint8_t defaultValues)
{
  _Wire.begin();
  PCF8574::write8(defaultValues);
}

#endif

uint8_t PCF8574::read8()
{
  _Wire.beginTransmission(_address);
  if(_Wire.requestFrom(_address, (uint8_t) 1) != 1)
  {
    _error = PCF8574_I2C_ERROR;
    return _data;
  }
#if (ARDUINO < 100)
  _data = _Wire.receive();
#else
  _data = _Wire.read();
#endif
  _Wire.endTransmission();
  return _data;
}

void PCF8574::resetInterruptPin()
{
  PCF8574::read8();
}

/*uint8_t PCF8574::value()
{
  return _data;
}*/

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
  if(pin > 7)
    {
      _error = PCF8574_PIN_ERROR;
      return 0;
  }
  PCF8574::read8();
  return (_data & (1<<pin)) > 0;
}

void PCF8574::write(uint8_t pin, uint8_t value)
{
  if(pin > 7)
    {
      _error = PCF8574_PIN_ERROR;
      return;
  }
  uint8_t _val = value & 1;
  if(_val) _pinModeMask |= _val << pin;
  else _pinModeMask &= ~(1 << pin);
  PCF8574::write8(_pinModeMask);
}

void PCF8574::toggle(uint8_t pin)
{
  if(pin > 7)
    {
      _error = PCF8574_PIN_ERROR;
      return;
  }
  _pinModeMask ^=  (1 << pin);
  PCF8574::write8(_pinModeMask);
}

void PCF8574::toggleAll()
{
  _pinModeMask = ~_pinModeMask;
  PCF8574::write8(_pinModeMask);
}

void PCF8574::shiftRight(uint8_t n)
{
  if (n == 0 || n > 7 ) return;
  _pinModeMask >>= n;
  PCF8574::write8(_pinModeMask);
}

void PCF8574::shiftLeft(uint8_t n)
{
  if (n == 0 || n > 7) return;
  _pinModeMask <<= n;
  PCF8574::write8(_pinModeMask);
}

void PCF8574::rotateRight(uint8_t n)
{
  uint8_t r = n & 7;
  _pinModeMask = (_pinModeMask >> r) | (_pinModeMask << (8-r));
  PCF8574::write8(_pinModeMask);
}

void PCF8574::rotateLeft(uint8_t n)
{
  PCF8574::rotateRight(8- (n & 7));
}

int PCF8574::lastError()
{
  int e = _error;
  _error = 0;
  return e;
}
