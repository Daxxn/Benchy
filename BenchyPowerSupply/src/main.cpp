#include <Arduino.h>
#include <Wire.h>
#include "BenchyPowerSupplyPinout.h"

const int FRONT_ADDR = 42;
const int address = 32;

uint8_t lowSetVoltage = 0;
uint8_t lowSetCurrent = 0;

uint8_t highSet = 0;

int V30Sense = 0;
int V12Sense = 0;
int highCCSense = 0;

bool overCurrent = false;

void HandleOverCurrent()
{
  overCurrent = true;
  highSet = 0;
}

void InitPins()
{
  attachInterrupt(
    digitalPinToInterrupt(Digitl::PowerPins::OC_SENSE_INT_PIN),
    HandleOverCurrent,
    RISING
  );
}

void I2CRecieve(int param)
{
  
}

void I2CRequest()
{
  
}

void setup() {
  // Wire.begin(address);
  // Wire.onReceive(I2CRecieve);
  // Wire.onRequest(I2CRequest);

  //Initial Testing
  Wire.begin();
  InitPins();
}

void loop() {
  // Wire.beginTransmission(42);
  // Wire.write("Test");
  // Wire.endTransmission();

  highSet++;
  analogWrite(Digitl::PowerPins::HIGH_PWM_PIN, highSet);
  delay(100);
}