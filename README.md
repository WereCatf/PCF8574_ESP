# PCF8574_ESP

This is a simple library for using the PCF8574/PCF8574A I/O - expanders over I2C. I took the code by Rob Tillaart from http://playground.arduino.cc/Main/PCF8574Class and modified it to allow the use of I2C on non-default pins and/or using a TwoWire - class of your own instead of the default "Wire" as defined in Wire.h.

This library does not supply any special handling for using the interrupt - pin on the PCF8574, just connect the pin to a pin on your ESP, set up an interrupt-handler for it and remember to use pinMode(pin, INPUT_PULLUP) to make use of it.

# Usage
```
class initializer PCF8574(uint8_t address, int sda = SDA, int scl = SCL, TwoWire UseWire = Wire)
uint8_t read8() -- Read all 8 pins' status at once as a bitmask with a pin being HIGH if the corresponding bit is set, and vice versa.
uint8_t read(uint8_t pin) -- Returns a single pin's status.
uint8_t value() -- Returns the cached pinmask used by the library.
void write8(uint8_t value) -- Set all 8 pins' status at once.
void write(uint8_t pin, uint8_t value) -- Set a single pin's status.
void toggle(uint8_t pin) -- Reverses the corresponding pin's status, HIGH to LOW or vice versa.
void shiftRight(uint8_t n=1) -- Shift the pins' values, with pin 1's value going into pin 2 and so on.
void shiftLeft(uint8_t n=1)
int lastError()
```

# Mixing INPUT- and OUTPUT-pins and write8

Due to the way the PCF8574 works you cannot just use read8() to read the pin-states, then change one pin and write8() the new states out because if you are using some pins as INPUT and the pin is being pulled low the moment you read8() its state you'll then be pulling the pin LOW when issuing write8() and it'll stop working as an INPUT. For this reason the library caches written values instead of relying on reading the pin-states when using write() or toggle(), and if you use write8() in your own code you need to remember to pull HIGH any pin you want to use as INPUT regardless of their current state.
