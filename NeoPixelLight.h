#pragma once

#include <cstdint>

#include "Light.h"

#include <Adafruit_NeoPixel.h>

class NeoPixelLight : public Light {
public:
  NeoPixelLight(const std::string& name, uint16_t ledCount, uint8_t pin, neoPixelType colorOrder = NEO_GRB);

  void start();

  void display() override;

private:
  const float GAMMA = 2.2f;

  Adafruit_NeoPixel strip;

  uint8_t gammaTable[256];
};

