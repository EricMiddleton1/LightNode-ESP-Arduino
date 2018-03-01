#include "SolidColorEffect.h"

#include <cmath>

#include <Arduino.h>

#include "Color.h"

SolidColorEffect::SolidColorEffect()
  : Effect("Solid Color", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0)
  , hue{0}
  , sat{0} {
}

void SolidColorEffect::color(uint8_t _hue, uint8_t _sat) {
  hue = _hue;
  sat = _sat;
  
  display();
}

std::vector<String> SolidColorEffect::params() const {
  return {"hue", "sat"};
}

String SolidColorEffect::getParam(const String& param) const {
  if(param == "hue") {
    return String((int)hue);
  }
  else if(param == "sat") {
    return String((int)sat);
  }
  else {
    return "";
  }
}

void SolidColorEffect::setParam(const String& param, const String& value) {
  if(param == "hue") {
    hue = value.toFloat();
  }
  else if(param == "sat") {
    sat = value.toFloat();
  }
}

void SolidColorEffect::display() {
  if(adapter != nullptr) {
    adapter->setAll(Color::HSV(hue, sat, 255));
  }
}

void SolidColorEffect::effectStart() {
  display();
}

void SolidColorEffect::stop() {
}

void SolidColorEffect::run() {
}

