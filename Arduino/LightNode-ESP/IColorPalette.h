#pragma once

#include "Color.h"

class IColorPalette {
public:
  virtual Color getColor() = 0;
};

