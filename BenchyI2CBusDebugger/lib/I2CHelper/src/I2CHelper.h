#pragma once

#include <Arduino.h>

#pragma region I2C Data
namespace Addresses
{
  // Controllers
  const int PSCtrlADDR = 0x11;
  const int FuncGenCtrlADDR = 0x31;
  const int LCRMeterCtrlADDR = 0x33;
  const int ElectLoadCtrlADDR = 0x21;

  //Front Panels
  const int PSFrontADDR = 0x12;
  const int MainFrontADDR = 0x00;

  const int RelaysADDR = 0x75;
} // namespace Addresses
#pragma endregion

struct I2CData
{
  I2CData() {};
  I2CData(const char *name, int address, int id) {
    this->name = name;
    this->address = address;
    this->ID = id;
  };
  const char *name;
  int address;
  int ID;
};


class I2CHelper
{
public:
  static I2CData GetData(const char *name, int index);
private:
};