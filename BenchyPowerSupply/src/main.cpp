#include <Arduino.h>
#include <Wire.h>
#include "BenchyPowerSupplyPinout.h"
#include "IntConverter.h"

#define recieveBufferSize 16
#define sendBufferSize 8

const int FRONT_ADDR = 42;
const int address = 32;

uint8_t receiveBuffer[recieveBufferSize] = {};
uint8_t sendBuffer[sendBufferSize] = {};
uint8_t receiveCommand = 0;
uint8_t sendCommand = 0;

uint8_t lowSetVoltage = 0;
uint8_t lowSetCurrent = 0;

uint8_t highSet = 0;

uint16_t V30Sense = 0;
uint16_t V12Sense = 0;
uint16_t highPSCurrent = 0;
uint16_t highPSVoltage = 0;
uint16_t lowPSCurrent = 0;
uint16_t lowPSVoltage = 0;

bool overCurrent = false;

bool lowPSEnable = false;
bool highPSEnable = false;

// uint8_t GetLowByte(uint16_t value)
// {
//   return value & 0xFF;
// }

// uint8_t GetHighByte(uint16_t value)
// {
//   return (value & 0xFF00) >> 8;
// }

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

#pragma region Main Controller Command functions
void ResetCMD()
{
  lowPSEnable = false;
  highPSEnable = false;
  lowSetVoltage = 0;
  lowSetCurrent = 0;
  highSet = 0;
}

void LowPSSetVoltageCMD()
{
  lowSetVoltage = receiveBuffer[1];
}

void LowPSSetCurrentCMD()
{
  lowSetCurrent = receiveBuffer[1];
}

void HighPSSetVoltageCMD()
{
  highSet = receiveBuffer[1];
}

void SetPSStatesCMD()
{
  uint8_t val = receiveBuffer[1];
  lowPSEnable = val & 0b01;
  highPSEnable = val & 0b10;
}

int PSMonitorCMD()
{
  sendBuffer[8] = {};
  sendBuffer[0] = GetLowByte(lowPSCurrent);
  sendBuffer[1] = GetHighByte(lowPSCurrent);
  sendBuffer[2] = GetLowByte(lowPSVoltage);
  sendBuffer[3] = GetHighByte(lowPSVoltage);
  sendBuffer[4] = GetLowByte(highPSCurrent);
  sendBuffer[5] = GetHighByte(highPSCurrent);
  sendBuffer[6] = GetLowByte(highPSVoltage);
  sendBuffer[7] = GetHighByte(highPSVoltage);
  
  return 8;
}

int PowerRailMonitorCMD()
{
  sendBuffer[4] = {};
  sendBuffer[0] = GetLowByte(V30Sense);
  sendBuffer[1] = GetHighByte(V30Sense);
  sendBuffer[2] = GetLowByte(V12Sense);
  sendBuffer[3] = GetHighByte(V12Sense);
  return 4;
}

int StatusCMD()
{
  sendBuffer[1] = {};
  uint8_t value = overCurrent;
  value <<= 1;
  value |= highPSEnable;
  value <<= 1;
  value |= lowPSEnable;
  sendBuffer[0] = value;
  return 1;
}

void (*receiveActions[])() = {
  ResetCMD,
  LowPSSetVoltageCMD,
  LowPSSetCurrentCMD,
  HighPSSetVoltageCMD,
};

int (*sendActions[])() = {
  PSMonitorCMD,
  PowerRailMonitorCMD,
  StatusCMD,
};
#pragma endregion

void I2CReceive(int len)
{
  if (len > 0)
  {
    Wire.readBytes(receiveBuffer, len);
    receiveCommand = receiveBuffer[0];
    receiveActions[receiveCommand]();
  }
}

void I2CRequest()
{
  sendCommand = Wire.read();
  int size = sendActions[sendCommand]();
  Wire.write(sendBuffer, size);
}

void setup() {
  Wire.begin(address);
  Wire.onReceive(I2CReceive);
  Wire.onRequest(I2CRequest);

  //Initial Testing
  InitPins();
}

void loop() {
  // Wire.beginTransmission(42);
  // Wire.write("Test");
  // Wire.endTransmission();

  // highSet++;
  // analogWrite(Digitl::PowerPins::HIGH_PWM_PIN, highSet);
  delay(100);
}