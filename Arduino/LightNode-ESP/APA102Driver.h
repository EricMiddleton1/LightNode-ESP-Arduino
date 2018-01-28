#pragma once


#include "Driver.h"

#include <NeoPixelBrightnessBus.h>

class APA102Driver : public Driver {
public:
  APA102Driver(uint16_t pixelDriver);

  uint16_t size() const override;

  void setBrightness(uint8_t brightness) override;
  uint8_t getBrightness() const override;
  
  void setColor(uint16_t index, const Color& c) override;

  void display() override;
private:
  const float GAMMA = 2.2f;

  NeoPixelBrightnessBus<DotStarBgrFeature, DotStarSpiMethod> strip;

  NeoGamma<NeoGammaTableMethod> gammaTable;
};

