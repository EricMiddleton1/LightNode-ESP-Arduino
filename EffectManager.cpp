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

  Serial.print("new [vector] (");
  Serial.print(sizeof(Effect*)*effects.size());
  Serial.println(" bytes)");

  os_timer_setfn(&runTimer, [](void* manager) { reinterpret_cast<EffectManager*>(manager)->cbRunTimer(); }, this);
}

void EffectManager::updateLight(LightAdapter& _light) {
  light = &_light;

  effects[activeEffect]->stop();
  effects[activeEffect]->start(*light);
}

void EffectManager::addEffect(Effect& effect) {
  Serial.print("new [vector resize] (");
  Serial.print(sizeof(Effect*)*effects.size());
  Serial.println(" bytes)");
  
  effects.push_back(&effect);
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

EffectManager::iterator EffectManager::findEffect(const std::string& name) {
  return std::find_if(effects.begin(), effects.end(), [&name](const Effect* effect) {
    return effect->getName() == name;
    });
}

EffectManager::const_iterator EffectManager::findEffect(const std::string& name) const {
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
}

void Off::stop() {
}

void Off::run() {
}

