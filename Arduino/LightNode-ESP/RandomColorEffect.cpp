#include "RandomColorEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>


RandomColorEffect::RandomColorEffect()
  : Effect{"Random Color", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0} {
}

void RandomColorEffect::effectStart() {
  if(!seeded) {
    srand(micros());
  }

  uint8_t hue = rand();
  adapter->setAll(Color::HSV(hue, 255, 255));
}

void RandomColorEffect::stop() {
}

void RandomColorEffect::run() {
}

