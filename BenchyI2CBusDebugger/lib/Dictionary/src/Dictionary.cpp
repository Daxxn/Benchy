#include <Arduino.h>
#include "Dictionary.h"

template <typename T>
Dictionary<T>::Dictionary(String keys, int size)
{
  this->Keys = new String[size];
  memset(this->Keys, "", size);
  this->Values = new T[size];
}

template <typename T>
Dictionary<T>::~Dictionary()
{
  delete[] this->Keys;
  delete[] this->Values;
}

template <typename T>
T Dictionary<T>::GetValue(String key)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == key)
    {
      return this->Values[i];
    }
  }
}

template <typename T>
String Dictionary<T>::GetKey(T value)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Values[i] == value)
    {
      return this->Keys[i];
    }
  }
  return "";
}

template <typename T>
void Dictionary<T>::SetValue(String key, T value)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == key)
    {
      this->Values[i] = value;
    }
  }
}

template <typename T>
void Dictionary<T>::SetKey(String key, String newKey)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == key)
    {
      this->Keys[i] = newKey;
    }
  }
}

template <typename T>
void Dictionary<T>::Add(String key, T value)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == key)
    {
      this->Values[i] == value;
      return;
    }
  }
  
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == "")
    {
      this->Keys[i] = key;
      this->Values[i] = value;
      return;
    }
  }

  this->Resize();
  this->Keys[this->size - 1] = key;
  this->Values[this->size - 1] = value;
}

template <typename T>
void Dictionary<T>::Add(String key)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == key)
    {
      return;
    }
  }
  
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys[i] == "")
    {
      this->Keys[i] = key;
      return;
    }
  }

  this->Resize();
  this->Keys[this->size - 1] = key;
}

template <typename T>
bool Dictionary<T>::Remove(String key)
{
  for (size_t i = 0; i < this->size; i++)
  {
    if (this->Keys == key)
    {
      this->Keys[i] = "";
      this->Values[i] = nullptr;
      return true;
    }
  }
  return false;
}

template <typename T>
void Dictionary<T>::Resize()
{
  // Create new arrays with the increased size
  T *newValues = new T[this->size + 1];
  String *newKeys = new String[this->size + 1];

  // Copy the data from the current arrays
  memcpy(newValues, this->Values, this->size);
  memcpy(newKeys, this->Keys, this->size);

  this->size++;

  // Get the pointers to the old arrays
  T *oldValues = this->Values;
  String *oldKeys = this->Keys;

  // set the pointers to the new arrays
  this->Keys = newKeys;
  this->Values = newValues;

  // delete the old arrays
  delete[] oldValues;
  delete[] oldKeys;
}