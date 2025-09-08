/*
Instructions:
  Connect Bluetooth module to Arduino - RX to pin 10, TX to pin 11, GND to GND, VCC to 5V.
  Open Serial Monitor in Arduino IDE. Enter AT commands (e.g., AT+NAMENewName) to rename the module.
  Make sure to set line ending to "Both NL & CR".
  Responses will appear in Serial Monitor.
*/ 


#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // TX, RX

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  Serial.println("Enter AT commands:");
}

void loop() {
  if (BT.available()) Serial.write(BT.read());
  if (Serial.available()) {
     char c = Serial.read();
     BT.write(c);
     Serial.write(c);
  }
}
