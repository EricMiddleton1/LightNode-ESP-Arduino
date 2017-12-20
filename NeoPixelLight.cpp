#include "NeoPixelLight.h"

#include <cmath>

NeoPixelLight::NeoPixelLight(const std::string& _name, uint16_t _ledCount, uint8_t _pin, neoPixelType _colorOrder)
  : Light{_name, _ledCount}
  , strip{_ledCount, _pin, _colorOrder | NEO_KHZ800} {

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i / 255.f, GAMMA) * 255.f + 0.5f;
  }
}

void NeoPixelLight::start() {
  strip.begin();
  strip.show();
}

void NeoPixelLight::display() {
  for(int i = 0; i < strip.numPixels(); ++i) {
    auto c = colors[i];
    
    strip.setPixelColor(i, gammaTable[c.getRed()], gammaTable[c.getGreen()], gammaTable[c.getBlue()]);
  }
  
  strip.show();
}

