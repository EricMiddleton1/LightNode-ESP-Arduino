#include "Effect.h"

#include <algorithm>

#include <Arduino.h>

Effect::Effect(const std::string& _name, std::vector<LightAdapter::Type> _supportedTypes, int _updatePeriod)
  : name{_name}
  , supportedTypes{std::move(_supportedTypes)}
  , updatePeriod{_updatePeriod}
  , adapter{nullptr} {

  Serial.print("new [vector] (");
  Serial.print(sizeof(LightAdapter::Type)*supportedTypes.size());
  Serial.println(" bytes)");
}

Effect::~Effect() {
}

const std::string& Effect::getName() const {
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

int Effect::size() const {
  return adapter->size();
}

void Effect::start(LightAdapter& _adapter) {
  adapter = &_adapter;
  
  effectStart();
}

