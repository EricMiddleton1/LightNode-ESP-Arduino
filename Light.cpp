#include "Light.h"

#include "LightAdapter.h"

#include <Arduino.h>

extern "C" {
  #include "mem.h"
}

Light::Light(const std::string& _name, uint16_t _count)
  : name{_name}
  , colors(_count)
  , changed{true} {

  adapter = new LightAdapter(*this, LightAdapter::Type::Linear);
}

Light::~Light() {
  delete adapter;
}

int Light::size() const {
  return colors.size();
}

std::string Light::getName() const {
  return name;
}

LightAdapter* Light::getAdapter() {
  return adapter;
}

void Light::update() {
  if(changed) {
    changed = false;
    display();
  }
}

