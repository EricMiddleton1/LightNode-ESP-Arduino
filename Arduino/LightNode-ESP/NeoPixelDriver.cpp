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

    case ColorOrder::GRBW:
      strip.grbw = new NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod>(_ledCount);
      strip.grbw->Begin();
      strip.grbw->Show();
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

    case ColorOrder::GRBW:
      delete strip.grbw;
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

        case ColorOrder::GRBW:
      return size(strip.grbw);
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

    case ColorOrder::GRBW:
      return getBrightness(strip.grbw);
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

    case ColorOrder::GRBW:
      setBrightness(strip.grbw, brightness);
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

    case ColorOrder::GRBW:
      RgbwColor color(c.getRed(), c.getGreen(), c.getBlue());
      if( (color.R == color.G) && (color.G == color.B) ) {
        color = RgbwColor(0, 0, 0, c.getRed());
      }
      strip.grbw->SetPixelColor(index, gammaTable.Correct(color));
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

    case ColorOrder::GRBW:
      display(strip.grbw);
    break;
  }
}

