#pragma once

#include <cstdint>
#include <Arduino.h>

#include "LED.h"

class Light {
public:
  Light(const String& name, uint16_t count);
  virtual ~Light();

  Light(const Light& other) = delete;

  LED& operator[](uint16_t index);
  const LED& operator[](uint16_t index) const;

  int getCount() const;
  String getName() const;

  virtual void update() = 0;
  
protected:
  String name;
  
  LED* leds;
  uint16_t count;
};
