#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "IntConverter.h"

enum LimitMode
{
  CURRENT = 0b01,
  POWER = 0b10,
  LOWEST = 0b11,
  OFF = 0
};

union FloatToBytes
{
  float data;
  uint8_t buffer[4];
};

struct ELoadTestState
{
  ELoadTestState() {}
  ELoadTestState(float current, float power, LimitMode mode)
  {
    this->CurrentSet.data = current;
    this->ModeSelect = mode;
  }
  FloatToBytes CurrentSet;
  FloatToBytes PowerSet;
  LimitMode ModeSelect;
};

struct ELoadTestResult
{
  ELoadTestResult() {}
  FloatToBytes Current;
  FloatToBytes Voltage;
  float Power = 0;
  uint8_t SetValue = 0;
  uint8_t Status = 0;
  LimitMode limit = LimitMode::OFF;

  bool LoadConnect = false;
  bool OverVolt = false;
  bool OverCurrent = false;
  bool OverPower = false;
};

class ELoadTests
{
public:
  ELoadTests(int addr);
  ~ELoadTests();

  void Begin(ELoadTestState state);

  void RunTestSuite();
  void RunTestSuite(ELoadTestState state);

  void RunTest(int test);

  // Receive Tests
  void ResetTest();
  void CurrentSetTest();
  void PowerSetTest();

  // Request Tests
  void LoadMonitorsTest();
  void GetSetValueTest();
  void StatusTest();

  void PrintData();
private:
  int Address;

  uint8_t* Buffer;

  int* Monitors;

  int CurrentSet;
  int PowerSet;

  ELoadTestState TestState;

  ELoadTestResult TestResult;
};

ELoadTests::ELoadTests(int addr)
{
  this->Address = addr;

  this->Buffer = new uint8_t[4];
  this->Monitors = new int[2];

  this->CurrentSet = 0;
  this->PowerSet = 0;
}

ELoadTests::~ELoadTests()
{
  delete[] this->Buffer;
  delete[] this->Monitors;
}

void ELoadTests::Begin(ELoadTestState state)
{
  this->TestState = state;
  this->TestResult = ELoadTestResult();
  Wire.begin();
}

void ELoadTests::RunTest(int test)
{
  if (test == 0)
  {
    this->RunTestSuite();
  }
  else if (test == 1)
  {
    ResetTest();
  }
  else if (test == 2)
  {
    CurrentSetTest();
  }
  else if (test == 3)
  {
    PowerSetTest();
  }
  else if (test == 4)
  {
    LoadMonitorsTest();
  }
  else if (test == 5)
  {
    GetSetValueTest();
  }
  else if (test == 6)
  {
    StatusTest();
  }
}

void ELoadTests::RunTestSuite()
{
  this->RunTestSuite(this->TestState);
}

void ELoadTests::RunTestSuite(ELoadTestState state)
{
  this->TestState = state;

  char str[70] = {};
  sprintf(str, "Starting ELoad Test Suite\n Test State:\n Current: %f  Power: %f", (double)state.CurrentSet.data, (double)state.PowerSet.data);
  Serial.println(str);

  this->ResetTest();
  Serial.println("Reset test complete.");
  delay(1000);

  this->CurrentSetTest();
  Serial.println("Current Set test complete.");
  delay(1000);

  // this->PowerSetTest();
  // Serial.println("Power Set test complete.");
  // delay(1000);

  this->LoadMonitorsTest();
  Serial.println("Load Monitors test complete.");
  delay(1000);

  this->GetSetValueTest();
  Serial.println("Current Set Value test complete.");
  delay(1000);

  this->StatusTest();
  Serial.println("Status test complete.");
  delay(1000);
}

void ELoadTests::ResetTest()
{
  Wire.beginTransmission(this->Address);
  Wire.write(0);
  Wire.endTransmission();
  Serial.println("Reset Test Complete.");
}

void ELoadTests::CurrentSetTest()
{
  Wire.beginTransmission(this->Address);
  Wire.write(0x1);
  Wire.write(this->TestState.CurrentSet.buffer, 4);
  Wire.endTransmission();
  Serial.println("Current Test Complete.");
}

void ELoadTests::PowerSetTest()
{
  Wire.beginTransmission(this->Address);
  Wire.write(0x2);
  Wire.write(this->TestState.PowerSet.buffer, 4);
  Wire.endTransmission();
  Serial.println("Power Set Test Complete.");
}

void ELoadTests::LoadMonitorsTest()
{
  // The desired way to structure the packet.
  Wire.beginTransmission(this->Address);
  Wire.write(0x10);
  Wire.endTransmission();
  
  Wire.requestFrom(this->Address, 8);
  Wire.readBytes(this->TestResult.Current.buffer, 4);
  Wire.readBytes(this->TestResult.Voltage.buffer, 4);

  this->TestResult.Power = this->TestResult.Current.data * this->TestResult.Voltage.data;

  // The easy way to structure the packet.
  // Wire.beginTransmission(this->Address);
  // Wire.write(0x3);
  // Wire.endTransmission();

  // Wire.requestFrom(this->Address, 4);
  // Wire.readBytes(this->Buffer, 4);
  // Wire.endTransmission();

  Serial.println("Monitors Test Complete.");
}

void ELoadTests::GetSetValueTest()
{
  Wire.beginTransmission(this->Address);
  Wire.write(0x11);
  Wire.endTransmission();

  Wire.requestFrom(this->Address, 1);
  this->TestResult.SetValue = Wire.read();

  Serial.println("Set Value Test Complete.");

  // Wire.beginTransmission(this->Address);
  // Wire.write(0x3);
  // Wire.endTransmission();

  // Wire.requestFrom(this->Address, 1);
  // this->TestResult.SetValue = Wire.read();
  // Wire.endTransmission();
}

void ELoadTests::StatusTest()
{
  Wire.beginTransmission(this->Address);
  Wire.write(0x12);
  Wire.endTransmission();

  Wire.requestFrom(this->Address, 1);
  this->TestResult.Status = Wire.read();

  this->TestResult.LoadConnect = bitRead(this->TestResult.Status, 3);
  this->TestResult.OverVolt = bitRead(this->TestResult.Status, 2);
  this->TestResult.OverPower = bitRead(this->TestResult.Status, 1);
  this->TestResult.OverCurrent = bitRead(this->TestResult.Status, 0);

  this->TestResult.limit = (LimitMode)((0b00110000 & this->TestResult.Status) >> 4);
  Serial.println("Status Test Complete.");

  // Wire.beginTransmission(this->Address);
  // Wire.write(0x4);
  // Wire.endTransmission();

  // Wire.requestFrom(this->Address, 1);
  // this->TestResult.Status = Wire.read();
  // Wire.endTransmission();
}

void ELoadTests::PrintData()
{
  Serial.println("\nState");
  Serial.print("Current: ");
  Serial.println(this->TestState.CurrentSet.data);
  Serial.print("Power: ");
  Serial.println(this->TestState.PowerSet.data);
  Serial.print("Mode: ");
  Serial.println(this->TestState.ModeSelect);
  Serial.println();
  Serial.println("Results");
  Serial.println("Load Monitors:");
  Serial.print("Current: ");
  Serial.print(this->TestResult.Current.data);
  Serial.println(" A");
  Serial.print("Voltage: ");
  Serial.print(this->TestResult.Voltage.data);
  Serial.println(" V");
  Serial.print("Power: ");
  Serial.print(this->TestResult.Power);
  Serial.println(" W");
  Serial.println("Control Values:");
  Serial.print("Set Value: ");
  Serial.println(this->TestResult.SetValue);
  Serial.print("Mode: ");
  Serial.println(this->TestResult.limit);
  Serial.println("Status:");
  Serial.print("Status Byte: ");
  Serial.println(this->TestResult.Status);
  Serial.print("OC: ");
  Serial.println(this->TestResult.OverCurrent);
  Serial.print("OV: ");
  Serial.println(this->TestResult.OverVolt);
  Serial.print("OP: ");
  Serial.println(this->TestResult.OverPower);
  Serial.print("Conn: ");
  Serial.println(this->TestResult.LoadConnect);
  delay(10 * 1000);
}