#include "NeoPixelMatrix.h"

#include <cmath>

NeoPixelMatrix::NeoPixelMatrix(const std::string& _name, uint8_t _width, uint8_t _height, PixelMapper _mapper, ColorOrder _colorOrder)
  : Matrix{_name, _width, _height, _mapper}
  , colorOrder{_colorOrder} {

  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(size());
    break;

    case ColorOrder::GRB:
      strip.grb = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(size());
    break;
  }

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i / 255.f, GAMMA) * 255.f + 0.5f;
  }
}

NeoPixelMatrix::~NeoPixelMatrix() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      delete strip.rgb;
    break;

    case ColorOrder::GRB:
      delete strip.grb;
    break;
  }
}

void NeoPixelMatrix::start() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb->Begin();
      strip.rgb->Show();
    break;

    case ColorOrder::GRB:
      strip.grb->Begin();
      strip.grb->Show();
    break;
  }
}

void NeoPixelMatrix::display() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      display(strip.rgb);
    break;

    case ColorOrder::GRB:
      display(strip.grb);
    break;
  }
}

