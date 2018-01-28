#include "APA102Driver.h"

#include <SPI.h>
#include <cmath>

APA102Driver::APA102Driver(uint16_t ledCount)
  : Driver{"APA102"}
  , strip{ledCount} {

  strip.Begin();
  SPI.setFrequency(10000000L); //Default SPI frequency (20Mhz - set in strip.Begin()) is too high for my hardware (nodeMCU board + 2n7000 level shifter)
  
  strip.Show();
}

uint16_t APA102Driver::size() const {
  return strip.PixelCount();
}

void APA102Driver::setBrightness(uint8_t brightness) {
  strip.SetBrightness(brightness);
}
uint8_t APA102Driver::getBrightness() const {
  return strip.GetBrightness();
}

void APA102Driver::setColor(uint16_t index, const Color& c) {
  strip.SetPixelColor(index, gammaTable.Correct(RgbColor(c.getRed(), c.getGreen(), c.getBlue())));
}

void APA102Driver::display() {
  strip.Show();
}

