#pragma once

#include "Color.h"

#include <Arduino.h>

class Driver {
public:
  Driver(const String& name);
  virtual ~Driver();

  String name() const;

  virtual uint16_t size() const = 0;

  virtual void setBrightness(uint8_t brightness) = 0;
  virtual uint8_t getBrightness() const = 0;
  
  virtual void setColor(uint16_t index, const Color& c) = 0;
  
  virtual void display() = 0;
private:
  String name_;
};

