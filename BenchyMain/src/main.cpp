#include <Arduino.h>
#include "BenchyMainREV1Pinout.h"

void setup() {
  Serial.begin(115200);
  pinMode(Digitl::PORTPins::EN_1_PIN, OUTPUT);
  pinMode(Digitl::PORTPins::EN_2_PIN, OUTPUT);
  pinMode(Digitl::PORTPins::EN_3_PIN, OUTPUT);
  pinMode(Digitl::PORTPins::EN_4_PIN, OUTPUT);
  pinMode(Digitl::PORTPins::EN_5_PIN, OUTPUT);

  pinMode(Digitl::USBPins::RESET_PIN, OUTPUT);
  pinMode(Digitl::USBPins::I2C_EN_PIN, OUTPUT);

  digitalWrite(Digitl::USBPins::RESET_PIN, HIGH);

  digitalWrite(Digitl::PORTPins::EN_1_PIN, HIGH);
  digitalWrite(Digitl::PORTPins::EN_2_PIN, HIGH);
  digitalWrite(Digitl::PORTPins::EN_3_PIN, HIGH);
  digitalWrite(Digitl::PORTPins::EN_4_PIN, HIGH);
  digitalWrite(Digitl::PORTPins::EN_5_PIN, HIGH);
}

void loop() {
  Serial.println("Test");
}