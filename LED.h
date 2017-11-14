#pragma once

#include <cstdint>

#include "Color.h"

class LED {
public:
  LED();

  bool isOn() const;
  Color getColor() const;

  void turnOn();
  void turnOff();
  
  uint8_t getHue() const;
  uint8_t getSat() const;
  uint8_t getVal() const;

  void setHue(uint8_t hue);
  void setSat(uint8_t sat);
  void setVal(uint8_t val);

private:
  uint8_t h, s, v;
  bool on;
};

