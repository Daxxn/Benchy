#pragma once

#include <Arduino.h>

uint8_t GetHighByte(uint16_t value)
{
  return value >> 8;
}

uint8_t GetLowByte(uint16_t value)
{
  return value & 0xFF;
}

uint16_t CombineBytes(uint8_t lowByte, uint8_t highByte)
{
  uint16_t value = lowByte;
  value |= highByte << 8;
  return value;
}