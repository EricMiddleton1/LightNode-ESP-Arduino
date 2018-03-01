#include "Effect.h"

#include <algorithm>

#include <Arduino.h>

Effect::Effect(const String& _name, std::vector<LightAdapter::Type> _supportedTypes, int _updatePeriod)
  : name{_name}
  , supportedTypes{std::move(_supportedTypes)}
  , updatePeriod{_updatePeriod}
  , adapter{nullptr} {
}

Effect::~Effect() {
}

const String& Effect::getName() const {
  return name;
}

const std::vector<LightAdapter::Type>& Effect::getSupportedTypes() const {
  return supportedTypes;
}

int Effect::getUpdatePeriod() const {
  return updatePeriod;
}

bool Effect::isSupported(const LightAdapter& light) const {
  auto found = std::find(supportedTypes.begin(), supportedTypes.end(), light.type());

  return found != supportedTypes.end();
}

std::vector<String> Effect::params() const {
  return {};
}

String Effect::getParam(const String& param) const {
  return "";
}

void Effect::setParam(const String& param, const String& value) {
  
}

int Effect::size() const {
  return (adapter == nullptr) ? 0 : adapter->size();
}

void Effect::start(LightAdapter& _adapter) {
  adapter = &_adapter;
  
  effectStart();
}

