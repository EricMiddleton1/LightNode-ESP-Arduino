#pragma once

#include <memory>
#include <cstdint>

#include "Light.h"

#include <NeoPixelBus.h>

class NeoPixelLight : public Light {
public:
  enum class ColorOrder {
    RGB,
    GRB
  };

  NeoPixelLight(const std::string& name, uint16_t ledCount, ColorOrder colorOrder = ColorOrder::GRB);
  ~NeoPixelLight();

  void start();

  void display() override;

private:
  const float GAMMA = 2.2f;

  template<typename Strip>
  void display(Strip* strip) {
    for(int i = 0; i < colors.size(); ++i) {
      auto c = colors[i];
      
      strip->SetPixelColor(i, RgbColor(gammaTable[c.getRed()], gammaTable[c.getGreen()], gammaTable[c.getBlue()]));
    }
    
    strip->Show();
  }

  union {
    NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>* rgb;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* grb;
  } strip;
  ColorOrder colorOrder;

  uint8_t gammaTable[256];
};

