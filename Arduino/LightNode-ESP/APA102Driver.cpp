#include "APA102Driver.h"

#include <SPI.h>
#include <cmath>

APA102Driver::APA102Driver(uint16_t ledCount)
  : Driver{"APA102"}
  , strip{ledCount} {

  strip.Begin();
  SPI.setFrequency(10000000L);
  
  strip.Show();

  for(int i = 0; i < 256; ++i) {
    gammaTable[i] = std::pow(i/255.f, GAMMA) * 255.f + 0.5f;
  }
}

uint16_t APA102Driver::size() const {
  return strip.PixelCount();
}

Color APA102Driver::getColor(uint16_t index) const {
  auto color = strip.GetPixelColor(index);
  return {color.R, color.G, color.B};
}

void APA102Driver::setColor(uint16_t index, const Color& c) {
  strip.SetPixelColor(index, RgbColor(gammaTable[c.getRed()], gammaTable[c.getGreen()], gammaTable[c.getBlue()]));
}

void APA102Driver::display() {
  strip.Show();
}

