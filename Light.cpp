#include "Light.h"

#include "LightAdapter.h"

#include <Arduino.h>

extern "C" {
  #include "mem.h"
}

Light::Light(const std::string& _name, uint16_t _count)
  : name{_name}
  , colors(_count) {

  Serial.print("new [vector] (");
  Serial.print(sizeof(Color)*colors.size());
  Serial.println(" bytes)");

  Serial.print("new (");
  Serial.print(sizeof(LightAdapter));
  Serial.println(" bytes)");
  adapter = new LightAdapter(*this, LightAdapter::Type::Linear);
}

Light::~Light() {
  Serial.print("delete (");
  Serial.print(sizeof(LightAdapter));
  Serial.println(" bytes)");
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

