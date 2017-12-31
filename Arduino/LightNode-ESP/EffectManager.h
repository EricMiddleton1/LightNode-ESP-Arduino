#pragma once

#include "Effect.h"
#include "LightAdapter.h"

#include <Arduino.h>

extern "C" {
  #include "os_type.h"
}

class Off : public Effect {
public:
  Off();
  
  void effectStart() override;
  void stop() override;
  void run() override;
};

class EffectManager {
public:
  using iterator = std::vector<Effect*>::iterator;
  using const_iterator = std::vector<Effect*>::const_iterator;

  EffectManager(LightAdapter& light);

  void updateLight(LightAdapter& light);
  
  void addEffect(Effect& effect);

  bool selectEffect(const String& name);
  void selectEffect(const iterator& itr);

  iterator getCurrentEffect();
  const_iterator getCurrentEffect() const;

  iterator findEffect(const String& name);
  const_iterator findEffect(const String& name) const;

  //-------------------------------------//
  //-------------Iterators---------------//
  //--Note: the returned iterators are---//
  //-invalidated after calls to addEffect//
  //-------------------------------------//
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

private:
  void cbRunTimer();

  Off nullEffect;
  std::vector<Effect*> effects;
  int activeEffect;

  LightAdapter* light;

  os_timer_t runTimer;
};

