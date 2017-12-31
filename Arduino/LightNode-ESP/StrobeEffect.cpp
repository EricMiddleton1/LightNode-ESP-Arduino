#include "StrobeEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>


StrobeEffect::StrobeEffect()
  : Effect{"Strobe", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD} {
}

void StrobeEffect::effectStart() {
  tick = 0;

  run();
}

void StrobeEffect::stop() {
}

void StrobeEffect::run() {
  uint8_t brightness = (tick == 0) ? 255 : 0;
  adapter->setAll({brightness, brightness, brightness});

  tick = (tick + 1) % STROBE_PERIOD;
}

