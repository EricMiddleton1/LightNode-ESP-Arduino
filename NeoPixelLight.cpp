#include "NeoPixelLight.h"

#include <cmath>

NeoPixelLight::NeoPixelLight(const String& _name, uint16_t _ledCount, uint8_t _pin)
  : Light{_name, _ledCount}
  , strip{_ledCount, _pin, NEO_GRB | NEO_KHZ800} {

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i / 255.f, GAMMA) * 255.f + 0.5f;
  }
}

void NeoPixelLight::begin() {
  strip.begin();
  strip.show();
}

void NeoPixelLight::update() {
  for(int i = 0; i < strip.numPixels(); ++i) {
    auto c = leds[i].getColor();
    
    strip.setPixelColor(i, gammaTable[c.getRed()], gammaTable[c.getGreen()], gammaTable[c.getBlue()]);
  }
  Serial.println();
  
  strip.show();
}

