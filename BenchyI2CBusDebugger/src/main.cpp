#include <Arduino.h>
#include <Wire.h>
#include "I2CHelper.h"
#include "TestFunctions.h"

#define DUT 0

#define TEST_CMD 0x4

I2CData i2c = I2CHelper::GetData("Power Supply Controller", DUT);
TestFunctions PSTest(i2c);

void RunTest()
{
  Serial.println("Test Started...");
  Serial.print(i2c.name);
  Serial.print("   Address: ");
  Serial.println(i2c.address);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  delay(1000);
}

void loop() {
  // RunTest();
  PSTest.RunTest(1);
  Serial.println("Test Complete");
  delay(1000);
}