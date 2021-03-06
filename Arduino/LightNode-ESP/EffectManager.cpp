#include "EffectManager.h"

#include <algorithm>

#include <Arduino.h>

extern "C" {
  #include "osapi.h"
}

EffectManager::EffectManager(LightAdapter& _light)
  : effects{&nullEffect}
  , activeEffect{0}
  , light{&_light} {

  os_timer_setfn(&runTimer, [](void* manager) { reinterpret_cast<EffectManager*>(manager)->cbRunTimer(); }, this);
}

void EffectManager::updateLight(LightAdapter& _light) {
  light = &_light;

  effects[activeEffect]->stop();
  effects[activeEffect]->start(*light);
}

void EffectManager::addEffect(Effect& effect) {
  effects.push_back(&effect);
}

bool EffectManager::selectEffect(const String& name) {
  auto found = findEffect(name);
  if(found == end()) {
    return false;
  }
  else {
    selectEffect(found);
    return true;
  }
}

void EffectManager::selectEffect(const iterator& itr) {
  effects[activeEffect]->stop();

  os_timer_disarm(&runTimer);
  
  activeEffect = itr - effects.begin();
  effects[activeEffect]->start(*light);

  auto period = effects[activeEffect]->getUpdatePeriod();

  if(period > 0) {
    os_timer_arm(&runTimer, period, 1);
  }
}

EffectManager::iterator EffectManager::getCurrentEffect() {
  return effects.begin() + activeEffect;
}

EffectManager::const_iterator EffectManager::getCurrentEffect() const {
  return effects.begin() + activeEffect;
}

EffectManager::iterator EffectManager::findEffect(const String& name) {
  return std::find_if(effects.begin(), effects.end(), [&name](const Effect* effect) {
    return effect->getName() == name;
    });
}

EffectManager::const_iterator EffectManager::findEffect(const String& name) const {
  return std::find_if(effects.begin(), effects.end(), [&name](const Effect* effect) {
    return effect->getName() == name;
    });
}

EffectManager::iterator EffectManager::begin() {
  return effects.begin();
}

EffectManager::const_iterator EffectManager::begin() const {
  return effects.begin();
}

EffectManager::iterator EffectManager::end() {
  return effects.end();
}

EffectManager::const_iterator EffectManager::end() const {
  return effects.end();
}

void EffectManager::cbRunTimer() {
  effects[activeEffect]->run();
}

Off::Off()
  : Effect("Off", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0) {
}

void Off::effectStart() {
  if(adapter != nullptr) {
    adapter->setAll({});
  }
}

void Off::stop() {
}

void Off::run() {
}

