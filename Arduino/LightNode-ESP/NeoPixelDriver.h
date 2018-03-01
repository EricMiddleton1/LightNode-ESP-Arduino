#pragma once

#include <memory>
#include <cstdint>

#include "Driver.h"

#include <NeoPixelBrightnessBus.h>

class NeoPixelDriver : public Driver {
public:
  enum class ColorOrder {
    RGB,
    GRB,
    GRBW
  };

  NeoPixelDriver(uint16_t ledCount, ColorOrder colorOrder = ColorOrder::GRB);
  ~NeoPixelDriver();

  uint16_t size() const override;

  uint8_t getBrightness() const override;
  void setBrightness(uint8_t brightness) override;

  void setColor(uint16_t index, const Color& c) override;

  void display() override;

private:
  const float GAMMA = 2.2f;

  template<typename Strip>
  uint16_t size(Strip* strip) const {
    return strip->PixelCount();
  }

  template<typename Strip>
  uint8_t getBrightness(Strip* strip) const {
    return strip->GetBrightness();
  }

  template<typename Strip>
  void setBrightness(Strip* strip, uint8_t brightness) const {
    strip->SetBrightness(brightness);
  }

  template<typename Strip>
  void setColor(Strip* strip, uint16_t index, const Color& c) {
    strip->SetPixelColor(index, gammaTable.Correct(RgbColor(c.getRed(), c.getGreen(), c.getBlue())));
  }

  template<typename Strip>
  void display(Strip* strip) {
    strip->Show();
  }

  union {
    NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod>* rgb;
    NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>* grb;
    NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod>* grbw;
  } strip;
  ColorOrder colorOrder;

  NeoGamma<NeoGammaTableMethod> gammaTable;
};

