#include <Arduino.h>
#include "I2CHelper.h"

I2CData I2CHelper::GetData(const char *name, int index)
{
  switch (index)
  {
  case 0:
    return I2CData(name, Addresses::PSCtrlADDR, index);
  case 1:
    return I2CData(name, Addresses::FuncGenCtrlADDR, index);
    case 2:
    return I2CData(name, Addresses::LCRMeterCtrlADDR, index);
    case 3:
    return I2CData(name, Addresses::ElectLoadCtrlADDR, index);
    case 4:
    return I2CData(name, Addresses::PSFrontADDR, index);
    case 5:
    return I2CData(name, Addresses::MainFrontADDR, index);
    case 6:
    return I2CData(name, Addresses::RelaysADDR, index);
  default:
    return I2CData("NoNameFound", 0, index);
  }
}