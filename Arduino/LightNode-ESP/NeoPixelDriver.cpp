#include "NeoPixelDriver.h"

#include <cmath>

NeoPixelDriver::NeoPixelDriver(uint16_t _ledCount, ColorOrder _colorOrder)
  : Driver{String("WS2812B_") + (_colorOrder == ColorOrder::RGB) ? "RGB" : "GRB"}
  , colorOrder{_colorOrder} {

  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb = new NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod>(_ledCount);
      strip.rgb->Begin();
      strip.rgb->Show();
    break;

    case ColorOrder::GRB:
      strip.grb = new NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>(_ledCount);
      strip.grb->Begin();
      strip.grb->Show();
    break;
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

uint8_t NeoPixelDriver::getBrightness() const {
  switch(colorOrder) {
    case ColorOrder::RGB:
      return getBrightness(strip.rgb);
    break;

    case ColorOrder::GRB:
      return getBrightness(strip.grb);
    break;
  }
}

void NeoPixelDriver::setBrightness(uint8_t brightness) {
  switch(colorOrder) {
    case ColorOrder::RGB:
      setBrightness(strip.rgb, brightness);
    break;

    case ColorOrder::GRB:
      setBrightness(strip.grb, brightness);
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

