#include <Arduino.h>
#include <Wire.h>
#include "Benchy-ElectronicLoad-REV2Pinout.h"

// I2C Interface

// Receive:
//   0x0 Reset
//   0x1 Current Set - 2 bytes
//   0x2 Power Set   - 2 bytes

// Request:
//   0x0 Load Monitors - 4 bytes
//   0x1 Set Value     - 1 byte
//   0x2 Status        - 1 byte

namespace LoadConstants
{
  const uint8_t ADDR = 0x21;
  const int MAX_LOAD_AMP = 10;
  const int MAX_LOAD_VOLT = 30;
  const int MAX_LOAD_PWR = MAX_LOAD_AMP * MAX_LOAD_VOLT;

  // Load voltage resistor ratio
  // TOP: 18K
  // BOT: 5.1K
  const float VOLT_DIV_RATIO = 0.2833333333;

  const float VCC = 4.98;
} // namespace LoadConstants

enum LimitMode
{
  OFF = 0,
  CURRENT_SET = 0b01,
  POWER_SET = 0b10,
  LOWEST_SET = 0b11,
} limit;

union FloatConverter
{
  uint8_t buffer[4];
  float data;
} loadCurrent, loadVoltage, loadISet, loadPSet;

// Raw Current Amp
uint16_t i_input = 0;
uint16_t v_input = 0;

uint8_t iSet = 0;
uint8_t pSet = 0;

uint8_t setPoint = 0;

uint8_t buffer[8] = { };

bool loadActive = false;
bool overVolt = false;
bool overCurr = false;
bool overPower = false;

volatile int command = 0;

void SoftReset()
{
  i_input = 0;
  v_input = 0;
  iSet = 0;

  loadActive = false;
  overVolt = false;
  overCurr = false;
  overPower = false;
}

int ConcatBytes(uint8_t upper, uint8_t lower)
{
  return lower | (upper << 8);
}

void ReadInputs()
{
  i_input = analogRead(Anlg::SensePins::I_PIN);
  v_input = analogRead(Anlg::SensePins::V_PIN);
}

void ClampSetPoint()
{
  if (loadISet.data > LoadConstants::MAX_LOAD_AMP)
  {
    loadISet.data = LoadConstants::MAX_LOAD_AMP;
  }
  if (loadPSet.data > LoadConstants::MAX_LOAD_PWR)
  {
    loadPSet.data = LoadConstants::MAX_LOAD_PWR;
  }
}

void CalcSetPoint()
{
  if (loadActive)
  {
    iSet = (loadISet.data / LoadConstants::MAX_LOAD_AMP) * 255;
    pSet = (loadPSet.data / LoadConstants::MAX_LOAD_PWR) * 255;
    if (limit == LimitMode::CURRENT_SET)
    {
      setPoint = iSet;
    }
    else if (limit == LimitMode::POWER_SET)
    {
      setPoint = pSet;
    }
    else if (limit == LimitMode::LOWEST_SET)
    {
      if (iSet < pSet)
      {
        setPoint = iSet;
      }
      else
      {
        setPoint = pSet;
      }
    }
  }
  else
  {
    setPoint = 0;
  }
  analogWrite(Digitl::SetPins::I_PIN, setPoint);
}

void CalcCurrent()
{
  loadCurrent.data = (i_input * (LoadConstants::VCC / 1023.0)) * 2;
}

// Calculate the voltage from the ADC conversion;
void CalcVoltage()
{
  // loadVoltage.data = (v_input * (LoadConstants::VCC / 1023.0)) * LoadConstants::VOLT_DIV_RATIO;
  loadVoltage.data = v_input / 1023.0 * LoadConstants::MAX_LOAD_VOLT;
}

// Checks inputs for values beyond the capacity of the circuit.
void LimitCheck()
{
  loadActive = true;
  if (loadVoltage.data > LoadConstants::MAX_LOAD_VOLT)
  {
    loadActive = false;
    overVolt = true;
  }
  else
  {
    overVolt = false;
  }
  
  if (loadCurrent.data > LoadConstants::MAX_LOAD_AMP)
  {
    loadActive = false;
    overCurr = true;
  }
  else
  {
    overCurr = false;
  }

  if (loadCurrent.data * loadVoltage.data > LoadConstants::MAX_LOAD_PWR)
  {
    loadActive = false;
    overPower = true;
  }
  else
  {
    overPower = false;
  }
}

// Not sure if this method will work...
void Receive(int len)
{
  if (len > 0)
  {
    command = Wire.read();

    if (command == 0x0)
    {
      SoftReset();
    }
    else if (command == 0x1)
    {
      Wire.readBytes(loadISet.buffer, 4);
    }
    else if (command == 0x2)
    {
      Wire.readBytes(loadPSet.buffer, 4);
      if (loadPSet.data >= LoadConstants::MAX_LOAD_PWR)
      {
        loadPSet.data = LoadConstants::MAX_LOAD_PWR;
      }
      iSet = ((loadPSet.data * (LoadConstants::VCC / 1023.0)) / loadVoltage.data) / LoadConstants::MAX_LOAD_AMP / 255;
    }
  }
}

void Request()
{
  if (command == 0x10)
  {
    buffer[0] = i_input & 0xFF;
    buffer[1] = i_input >> 8;
    buffer[2] = v_input & 0xFF;
    buffer[3] = v_input >> 8;

    Wire.write(buffer, 4);
  }
  else if (command == 0x11)
  {
    Wire.write(iSet);
  }
  else if (command == 0x12)
  {
    buffer[0] = (uint8_t)overCurr;
    buffer[0] |= ((uint8_t)overPower << 1);
    buffer[0] |= ((uint8_t)overVolt << 2);
    buffer[0] |= ((uint8_t)loadActive << 3);
    buffer[0] |= ((uint8_t)limit << 4);

    Wire.write(buffer[0]);
  }
}

void setup()
{
  Wire.begin(LoadConstants::ADDR);

  Wire.onReceive(Receive);
  Wire.onRequest(Request);
}

void loop()
{
  ReadInputs();
  CalcCurrent();
  CalcVoltage();
  LimitCheck();
  ClampSetPoint();
  CalcSetPoint();
}