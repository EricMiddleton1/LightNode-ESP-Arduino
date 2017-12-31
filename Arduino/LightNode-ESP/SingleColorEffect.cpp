#include "SingleColorEffect.h"

#include <cmath>

#include <Arduino.h>

#include "Color.h"

SingleColorEffect::SingleColorEffect()
  : Effect("Single Color", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0)
  , isOn{false}
  , period{100}
  , hue{0}
  , sat{0}
  , val{128} {
}

void SingleColorEffect::transitionPeriod(uint8_t _period) {
  period = _period;
}

void SingleColorEffect::on(bool _isOn) {
  isOn = _isOn;
  display();
}

void SingleColorEffect::brightness(uint8_t brightness) {
  isOn = true;
  
  if(brightness > 0) {
    val = brightness;
  }
  else {
    isOn = 0;
  }
  display();
}

void SingleColorEffect::color(uint8_t _hue, uint8_t _sat) {
  isOn = true;
  
  hue = _hue;
  sat = _sat;
  display();
}

void SingleColorEffect::changeBrightness(uint8_t delta) {
  isOn = true;
  
  uint8_t newVal = std::min(255, std::max(0, val + delta));
  if(newVal > 0) {
    val = newVal;
  }
  else {
    isOn = false;
  }
  display();
}

void SingleColorEffect::display() {
  if(adapter != nullptr) {
    adapter->setAll(isOn ? Color::HSV(hue, sat, val) : Color{});
  }
}

void SingleColorEffect::effectStart() {
  display();
}

void SingleColorEffect::stop() {
}

void SingleColorEffect::run() {
}

