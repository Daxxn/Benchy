#pragma once

#include <Arduino.h>

template <typename T>
class Dictionary
{
public:
  Dictionary(String keys, int size);
  ~Dictionary();

  T GetValue(String key);
  String GetKey(T value);

  void SetValue(String key, T value);
  void SetKey(String key, String newKey);

  void Add(String key, T value);
  void Add(String key);

  bool Remove(String key);
private:
  int size;
  String *Keys;
  T *Values;

  void Resize();
};