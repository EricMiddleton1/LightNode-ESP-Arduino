#pragma once

#include <memory>
#include <cstdint>

#include "Driver.h"

#include <NeoPixelBus.h>

class NeoPixelDriver : public Driver {
public:
  enum class ColorOrder {
    RGB,
    GRB
  };

  NeoPixelDriver(uint16_t ledCount, ColorOrder colorOrder = ColorOrder::GRB);
  ~NeoPixelDriver();

  uint16_t size() const override;

  Color getColor(uint16_t index) const override;
  void setColor(uint16_t index, const Color& c) override;

  void display() override;

private:
  const float GAMMA = 2.2f;

  template<typename Strip>
  uint16_t size(Strip* strip) const {
    return strip->PixelCount();
  }

  template<typename Strip>
  Color getColor(Strip* strip, uint16_t index) const {
    auto color =  strip->GetPixelColor(index);
    return {color.R, color.G, color.B};
  }

  template<typename Strip>
  void setColor(Strip* strip, uint16_t index, const Color& c) {
    strip->SetPixelColor(index, RgbColor(gammaTable[c.getRed()], gammaTable[c.getGreen()], gammaTable[c.getBlue()]));
  }

  template<typename Strip>
  void display(Strip* strip) {
    strip->Show();
  }

  union {
    NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>* rgb;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* grb;
  } strip;
  ColorOrder colorOrder;

  uint8_t gammaTable[256];
};

