#include "ColorFadeEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>


ColorFadeEffect::ColorFadeEffect()
  : Effect{"Color Fade", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD} {
}

void ColorFadeEffect::effectStart() {
  hue = 0;

  run();
}

void ColorFadeEffect::stop() {
}

void ColorFadeEffect::run() {
  adapter->setAll(Color::HSV(hue, 255, 255));

  hue = fmod(hue + FADE_RATE*UPDATE_PERIOD/1000.f, 256.f); 
}

