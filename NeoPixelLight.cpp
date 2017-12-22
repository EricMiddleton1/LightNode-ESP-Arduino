#include "NeoPixelLight.h"

#include <cmath>

NeoPixelLight::NeoPixelLight(const std::string& _name, uint16_t _ledCount, ColorOrder _colorOrder)
  : Light{_name, _ledCount}
  , colorOrder{_colorOrder} {

  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(_ledCount);
    break;

    case ColorOrder::GRB:
      strip.grb = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(_ledCount);
    break;
  }

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i / 255.f, GAMMA) * 255.f + 0.5f;
  }
}

NeoPixelLight::~NeoPixelLight() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      delete strip.rgb;
    break;

    case ColorOrder::GRB:
      delete strip.grb;
    break;
  }
}

void NeoPixelLight::start() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      Serial.println("start() RGB");
      strip.rgb->Begin();
      strip.rgb->Show();
    break;

    case ColorOrder::GRB:
      Serial.println("start() RGB");
      strip.grb->Begin();
      strip.grb->Show();
    break;
  }

  Serial.println("start() done");
}

void NeoPixelLight::display() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      display(strip.rgb);
    break;

    case ColorOrder::GRB:
      display(strip.grb);
    break;
  }
}

