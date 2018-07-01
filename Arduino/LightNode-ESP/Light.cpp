#include "Light.h"

#include "LightAdapter.h"

#include <Arduino.h>

extern "C" {
  #include "mem.h"
}

Light::Light(const String& _name)
  : name{_name} {
}

Light::~Light() {
}

Light::operator bool() const {
  return driver && adapter;
}

String Light::getName() const {
  return name;
}

LightAdapter* Light::getAdapter() {
  return adapter.get();
}

const LightDriver* Light::getDriver() const {
  return driver.get();
}

void Light::setDriver(std::unique_ptr<LightDriver>&& _driver) {
  driver = std::move(_driver);
}

void Light::setAdapter(std::unique_ptr<LightAdapter>&& _adapter) {
  adapter = std::move(_adapter);
  adapter->replaceDriver(driver.get());
}

void Light::run() {
  adapter->run();
}

