/*  Example sketch for the PCF8574 for the purposes of showing how to use the interrupt-pin.

    Attach the positive lead of an LED to PIN7 on the PCF8574 (use a resistor if your LED can't handle 3.3V!),
    a wire from GPIO2 (Nodemcu D4) to PIN3 so that it alternates the pin between HIGH and LOW once a second,
    and the INT-pin to GPIO14 (Nodemcu D5) on the ESP8266.

    If all goes well you should see the small blue LED on the ESP-module lighting up and the LED connected to
    the PCF going off, and vice versa. */

#include <pcf8574_esp.h>
#include <ESP8266WiFi.h>

/*  Wire.h already defines "Wire" which the PCF8574-class would use by default, but for the sakes of an example let's define our
    own instance of it and use that instead!

    Also, since I2C is emulated on the ESP8266 let's redefine what pins to use as SDA and SCL and instead swap them around!
    Just for the sakes of an example!
    DO NOT FORGET TO WIRE ACCORDINGLY, SDA GOES TO GPIO5, SCL TO GPIO4 (ON NODEMCU GPIO5 IS D1 AND GPIO4 IS D2) */
TwoWire testWire;
PCF8574 pcf8574(0x20, 5, 4, testWire);

bool PCFInterruptFlag = false;

void ICACHE_RAM_ATTR PCFInterrupt() {
  PCFInterruptFlag = true;
}

void setup() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  // The above lines turn WiFi off for lower power-consumption, since we're not using it in this sketch.
  // Remove them if you want to modify the sketch and use WiFi.

  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Most ready-made PCF8574-modules seem to lack an internal pullup-resistor, so you have to use the ESP8266-internal one.
  pinMode(14, INPUT_PULLUP);
  pcf8574.resetInterruptPin();
  attachInterrupt(digitalPinToInterrupt(14), PCFInterrupt, FALLING);

  // Do notice that we aren't initializing the pins on the PCF8574, because they default to INPUT, ie. HIGH, after reset.
  // Initialization is therefore not needed.
}

void loop() {
  if(PCFInterruptFlag){
    Serial.println("Got an interrupt: ");
    if(pcf8574.read(3)==HIGH) Serial.println("Pin 3 is HIGH!");
    else Serial.println("Pin 3 is LOW!");
    // DO NOTE: When you write LOW to a pin on a PCF8574 it becomes an OUTPUT.
    // It wouldn't generate an interrupt if you were to connect a button to it that pulls it HIGH when you press the button.
    // Any pin you wish to use as input must be written HIGH, or, like it says, all the pins already default to HIGH at boot.
    pcf8574.write(7, pcf8574.read(3));
    PCFInterruptFlag=false;
  }
  delay(1000);
  if(digitalRead(2)==HIGH) digitalWrite(2, LOW);
  else digitalWrite(2, HIGH);
}
