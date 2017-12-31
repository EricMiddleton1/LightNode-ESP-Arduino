#include "Light.h"

#include "LightAdapter.h"

#include <Arduino.h>

extern "C" {
  #include "mem.h"
}

Light::Light(const std::string& _name, uint16_t _count, LightAdapter* customAdapter)
  : name{_name}
  , colors(_count)
  , changed{true} {

  externalAdapter = customAdapter != nullptr;
  if(externalAdapter) {
    adapter = customAdapter;
  }
  else {
    adapter = new LightAdapter(*this);
  }
}

Light::~Light() {
  if(!externalAdapter) {
    delete adapter;
  }
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

