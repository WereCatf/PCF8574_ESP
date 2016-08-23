/*  A simple example-sketch on how to toggle an LED on PIN7 on the PCF8574.
    
    You must install TinyWireM-library (https://github.com/SpenceKonde/TinyWireM) for I2C on Attiny */

#include <pcf8574_esp.h>

//Initialize a PCF8574 at I2C-address 0x20
PCF8574 pcf8574(0x20);

void setup() {
}

void loop() {
  delay(1000);
  pcf8574.toggle(7);
}
