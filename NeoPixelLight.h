#pragma once

#include <cstdint>

#include <Adafruit_NeoPixel.h>

#include "Light.h"

class NeoPixelLight : public Light {
public:
  NeoPixelLight(const String& name, uint16_t ledCount, uint8_t pin);

  void begin();

  void update() override;

private:
  const float GAMMA = 2.2f;

  Adafruit_NeoPixel strip;

  uint8_t gammaTable[256];
};

