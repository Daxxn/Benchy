#include <Arduino.h>
#include <Wire.h>
#include "I2CHelper.h"
#include "TestFunctions.h"
#include "ELoadTests.h"

#define DUT 0

#define TEST_CMD 0x4

#define TEST_DEVICE 2

I2CData i2c = I2CHelper::GetData("Power Supply Controller", DUT);
TestFunctions PSTest(i2c);

ELoadTests eload(Addresses::ElectLoadCtrlADDR);
ELoadTestState testState(1, 100, LimitMode::CURRENT);

uint8_t buffer[10] = {};

int testFunc = 6;

// void serialEvent()
// {
//   if (Serial.available() != 0)
//   {
//     for (size_t i = 0; i < 10; i++)
//     {
//       buffer[i] = Serial.readString()
//     }
    
//   }

// }

void RunTest()
{
  Serial.println("Test Started...");
  Serial.print(i2c.name);
  Serial.print("   Address: ");
  Serial.println(i2c.address);
}

void setup() {
  Serial.begin(115200);
  // Wire.begin();

  delay(100);

  Serial.println("Starting Test...");

  eload.Begin(testState);

  delay(100);
}

void loop() {

  if (TEST_DEVICE == 1)
  {
    RunTest();
    PSTest.RunTest(1);
  }
  else if (TEST_DEVICE == 2)
  {
    eload.RunTest(testFunc);
    eload.PrintData();
  }

  Serial.println("All Tests Complete.");
  delay(5000);
}