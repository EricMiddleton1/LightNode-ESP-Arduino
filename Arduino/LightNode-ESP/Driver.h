#pragma once

#include "Color.h"

class Driver {
public:
  virtual ~Driver();

  virtual uint16_t size() const = 0;
  
  virtual Color getColor(uint16_t index) const = 0;
  virtual void setColor(uint16_t index, const Color& c) = 0;
  
  virtual void display() = 0;
};

