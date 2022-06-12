#include <Arduino.h>
#include <Wire.h>
#include "I2CHelper.h"
#include "IntConverter.h"

namespace CMD
{
  namespace PS
  {
    const int RESET = 0x0;
    const int LOW_V_SET = 0x1;
    const int LOW_C_SET = 0x2;
    const int HIGH_V_SET = 0x3;
    const int STATE_SET = 0x4;
  } // namespace PSCommands
} // namespace CMD

struct TestConditions
{
  TestConditions() {}
  TestConditions(int *assert, int len)
  {
    this->assert = assert;
    this->len = len;
  }
  int *assert;
  int *result;
  int len;
};


class TestFunctions
{
private:
  int testID;
  I2CData i2c;
  long ReadWriteDelay = 2;
  long TestDelay = 10;

  #pragma region Power Supply
  void RunPowerSupply();

  void PSWriteReset();
  void PSWriteStates();
  void PSWriteLowVoltage();
  void PSWriteLowCurrent();
  void PSWriteHighVoltage();

  void PSReadMonitor();
  void PSReadRailMonitor();
  void PSReadStatus();
  // Not implemented on REV1
  // Will be on REV2
  void PSWriteHighCurrent();
  #pragma endregion

  #pragma region PS Front Panel
  
  #pragma endregion

  #pragma region Electronic Load
  
  #pragma endregion

  #pragma region Function Generator
  
  #pragma endregion

  #pragma region LCR Meter
  
  #pragma endregion

  void RunAll();
public:
  TestFunctions(I2CData &data);
  void SetI2CData(I2CData &data);
  // Sets the test ID to select what IC will be tested.
  // 0: ALL
  // 1: Power Supply
  // 2: Electronic Load
  // 3: Function Generator
  void SetTestID(int id);
  void RunTest(int id);
};

TestFunctions::TestFunctions(I2CData &data)
{
  this->i2c = data;
}

void TestFunctions::SetI2CData(I2CData &data)
{
  i2c = data;
}

void TestFunctions::SetTestID(int id)
{
  testID = id;
}

void TestFunctions::RunTest(int id)
{
  switch (id)
  {
  case 0:
    RunAll();
    break;
  case 1:
    RunPowerSupply();
    break;
  case 2:

    break;
  case 3:

    break;
  case 4:

    break;
  
  default:
    Serial.println("Test ID not found.");
    break;
  }
}

void TestFunctions::RunAll()
{
  Serial.println("Testing All...");
}

void TestFunctions::RunPowerSupply()
{
  Serial.println("Power Supply Test Started...");
  Serial.print("Address: ");
  Serial.println(i2c.address);
  Serial.println("Testing Write Commands");
  Serial.println("Sending PS Reset Command");
  PSWriteReset();
  delay(TestDelay);
  Serial.println("Sending PS State Command");
  PSWriteStates();
  delay(TestDelay);
  Serial.println("Sending PS Low-V Command");
  PSWriteLowVoltage();
  delay(TestDelay);
  Serial.println("Sending PS Low-C Command");
  PSWriteLowCurrent();
  delay(TestDelay);
  Serial.println("Sending PS High-V Command");
  PSWriteHighVoltage();
  delay(TestDelay);
  Serial.println("Sending PS High-C Command");
  PSWriteHighCurrent();
  delay(TestDelay);
  Serial.println("Testing Read Commands");
  Serial.println("Sending PS Monitor Command");
  PSReadMonitor();
  delay(TestDelay);
  Serial.println("Sending PS Rail Monitor Command");
  PSReadRailMonitor();
  delay(TestDelay);
}

#pragma region PS Methods
#pragma region Write
void TestFunctions::PSWriteReset()
{
  Wire.beginTransmission(i2c.address);
  Wire.write(CMD::PS::RESET);
  Wire.endTransmission();
}

void TestFunctions::PSWriteStates()
{
  Serial.println("Starting Test Transmission...");
  Wire.beginTransmission(i2c.address);
  Wire.write(CMD::PS::STATE_SET);
  Wire.write(0b10);
  int error = Wire.getWriteError();
  Serial.print("Error: ");
  Serial.println(error);
  Serial.println("Test Transmission Completed");
}

void TestFunctions::PSWriteLowVoltage()
{
  Wire.beginTransmission(i2c.address);
  Wire.write(CMD::PS::LOW_V_SET);
  Wire.write(200);
  Wire.endTransmission();
}

void TestFunctions::PSWriteLowCurrent()
{
  Wire.beginTransmission(i2c.address);
  Wire.write(CMD::PS::LOW_C_SET);
  Wire.write(100);
  Wire.endTransmission();
}

void TestFunctions::PSWriteHighVoltage()
{
  Wire.beginTransmission(i2c.address);
  Wire.write(CMD::PS::HIGH_V_SET);
  Wire.write(50);
  Wire.endTransmission();
}

  // Not implemented on REV1
  // Will be on REV2
void TestFunctions::PSWriteHighCurrent()
{
  Serial.println("Write-High-Current Not implemented on REV1");
  // Wire.beginTransmission(PSi2c.address);
  // Wire.write(CMD::PS::HIGH_C_SET);
  // Wire.write(50);
  // Wire.endTransmission();
}
#pragma endregion
#pragma region Read
void TestFunctions::PSReadMonitor()
{
  uint8_t buffer[8] = {};
  Wire.beginTransmission(i2c.address);
  Wire.write(5);
  Wire.write(0);
  Wire.endTransmission();
  delay(ReadWriteDelay);
  Wire.requestFrom(i2c.address, 8);
  Wire.readBytes(buffer, 8);
  uint16_t lowCurrent = CombineBytes(buffer[0], buffer[1]);
  uint16_t lowVoltage = CombineBytes(buffer[2], buffer[3]);
  uint16_t highCurrent = CombineBytes(buffer[4], buffer[5]);
  uint16_t highVoltage = CombineBytes(buffer[6], buffer[7]);

  char strBuffer[42] = {};
  sprintf(strBuffer, "Low( V: %u I: %u)\nHigh( V: %u, I: %u)", lowVoltage, lowCurrent, highVoltage, highCurrent);
  Serial.println(strBuffer);
}

void TestFunctions::PSReadRailMonitor()
{
  uint8_t buffer[4] = {};
  Wire.beginTransmission(i2c.address);
  Wire.write(5);
  Wire.write(1);
  Wire.endTransmission();
  delay(ReadWriteDelay);
  Wire.requestFrom(i2c.address, 4);
  Wire.readBytes(buffer, 4);
  uint16_t V30Rail = CombineBytes(buffer[0], buffer[1]);
  uint16_t V12rail = CombineBytes(buffer[2], buffer[3]);
  char strBuffer[24] = {};
  sprintf(strBuffer, "Rails( 30V: %u 12V: %u)", V30Rail, V12rail);
  Serial.println(strBuffer);
}

void TestFunctions::PSReadStatus()
{
  uint8_t status = 0;
  Wire.beginTransmission(i2c.address);
  Wire.write(5);
  Wire.write(2);
  Wire.endTransmission();
  delay(ReadWriteDelay);
  Wire.requestFrom(i2c.address, 1);
  status = Wire.read();
  char buffer[54] = {};
  sprintf(
    buffer,
    "Status: Over-Current: %u High State: %u Low State: %u",
    status & 0b100,
    status & 0b10,
    status & 0b1
  );
  Serial.println(buffer);
}
#pragma endregion
#pragma endregion