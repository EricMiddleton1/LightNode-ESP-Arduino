#include "NeoPixelDriver.h"

#include <cmath>

NeoPixelDriver::NeoPixelDriver(uint16_t _ledCount, ColorOrder _colorOrder)
  : Driver{String("WS2812B_") + (_colorOrder == ColorOrder::RGB) ? "RGB" : "GRB"}
  , colorOrder{_colorOrder} {

  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(_ledCount);
      strip.rgb->Begin();
      strip.rgb->Show();
    break;

    case ColorOrder::GRB:
      strip.grb = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(_ledCount);
      strip.grb->Begin();
      strip.grb->Show();
    break;
  }

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i / 255.f, GAMMA) * 255.f + 0.5f;
  }
}

NeoPixelDriver::~NeoPixelDriver() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      delete strip.rgb;
    break;

    case ColorOrder::GRB:
      delete strip.grb;
    break;
  }
}

uint16_t NeoPixelDriver::size() const {
  switch(colorOrder) {
    case ColorOrder::RGB:
      return size(strip.rgb);
    break;

    case ColorOrder::GRB:
      return size(strip.grb);
    break;
  }
}

Color NeoPixelDriver::getColor(uint16_t index) const {
  switch(colorOrder) {
    case ColorOrder::RGB:
      return getColor(strip.rgb, index);
    break;

    case ColorOrder::GRB:
      return getColor(strip.grb, index);
    break;
  }
}

void NeoPixelDriver::setColor(uint16_t index, const Color& c) {
  switch(colorOrder) {
    case ColorOrder::RGB:
      setColor(strip.rgb, index, c);
    break;

    case ColorOrder::GRB:
      setColor(strip.grb, index, c);
    break;
  }
}

void NeoPixelDriver::display() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      display(strip.rgb);
    break;

    case ColorOrder::GRB:
      display(strip.grb);
    break;
  }
}

