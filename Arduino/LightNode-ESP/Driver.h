#pragma once

#include "Color.h"

class Driver {
public:
  Driver(uint16_t count);
  virtual ~Driver();

  uint16_t size() const;

  virtual const Color& operator[](uint16_t index) const = 0;
  virtual Color& operator[](uint16_t index) = 0;
  virtual void display() = 0;
};

