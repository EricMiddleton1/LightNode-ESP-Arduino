#pragma once


#include "Driver.h"

#include <NeoPixelBus.h>

class APA102Driver : public Driver {
public:
  APA102Driver(uint16_t pixelDriver);

  uint16_t size() const override;

  Color getColor(uint16_t index) const override;
  void setColor(uint16_t index, const Color& c) override;

  void display() override;
private:
  const float GAMMA = 2.2f;

  NeoPixelBus<DotStarBgrFeature, DotStarSpiMethod> strip;

  uint8_t gammaTable[256];

};

