#pragma once

#include <memory>
#include <cstdint>

#include "Matrix.h"

#include <NeoPixelBus.h>

class NeoPixelMatrix : public Matrix {
public:
  enum class ColorOrder {
    RGB,
    GRB
  };

  NeoPixelMatrix(const std::string& name, uint8_t width, uint8_t height, PixelMapper mapper, ColorOrder colorOrder = ColorOrder::GRB);
  ~NeoPixelMatrix();

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

