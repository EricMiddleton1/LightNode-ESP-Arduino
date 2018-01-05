#include "ColorWipeEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>


ColorWipeEffect::ColorWipeEffect()
  : Effect{"Color Wipe", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD} {
}

void ColorWipeEffect::effectStart() {
  hue = 0;
  mult = 255.f / adapter->size();

  run();
}

void ColorWipeEffect::stop() {
}

void ColorWipeEffect::run() {
  adapter->setAll(Color::HSV(hue, 255, 255));
  for(int i = 0; i < adapter->size(); ++i) {
    int pixelHue = hue + i*mult;
    adapter->setColor(i, Color::HSV(pixelHue, 255, 255));
  }

  hue = fmod(hue + FADE_RATE*UPDATE_PERIOD/1000.f, 256.f); 
}

