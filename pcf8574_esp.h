/* PCF8574_ESP -- library for using the I2C-driven 8-pin GPIO-expander
   ORIGINAL AUTHOR: Rob Tillaart
   Library modified by WereCatf */

#ifndef _PCF8574_ESP_H
#define _PCF8574_ESP_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef ARDUINO_AVR_DIGISPARK
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

#define PCF8574_OK          0x00
#define PCF8574_PIN_ERROR   0x81
#define PCF8574_I2C_ERROR   0x82

class PCF8574
{
  public:
    #ifdef ARDUINO_AVR_DIGISPARK || ARDUINO_AVR_ATTINYX5
    PCF8574(uint8_t address);
    #elif defined (ESP8266)
    PCF8574(uint8_t address, int sda = SDA, int scl = SCL, TwoWire UseWire = Wire);
    #else
    PCF8574(uint8_t address, TwoWire UseWire = Wire);
    #endif

    void begin(uint8_t defaultValues=0xff);
    uint8_t read8();
    uint8_t read(uint8_t pin);

    void write8(uint8_t value);
    void write(uint8_t pin, uint8_t value);

    void toggle(uint8_t pin);
    void toggleAll();
    void shiftRight(uint8_t n=1);
    void shiftLeft(uint8_t n=1);
    void rotateRight(uint8_t n);
    void rotateLeft(uint8_t n);
    void resetInterruptPin();

    int lastError();

  private:
    #ifdef ARDUINO_AVR_DIGISPARK || ARDUINO_AVR_ATTINYX5
    USI_TWI _Wire;
    #else
    TwoWire _Wire;
    #endif
    uint8_t _address;
    uint8_t _data;
    uint8_t _pinModeMask;
    int _error;
    #if defined (ESP8266)
    int _sda;
    int _scl;
    #endif
};

#endif
