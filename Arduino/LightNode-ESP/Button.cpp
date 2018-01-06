#include "Button.h"

#include "EffectManager.h"

Button::Button(EffectManager& effectManager, const String& effect)
  : effectManager_(effectManager)
  , effect_{effectManager.findEffect(effect) - effectManager.begin()} {
}

Button::~Button() {
}

void Button::run() {
  
}

void Button::toggle() {
  auto curEffect = effectManager_.getCurrentEffect() - effectManager_.begin();
  EffectManager::iterator nextEffect;
  
  if(curEffect == effect_) {
    Serial.println("[Info] Button::toggle(): Off");
    nextEffect = effectManager_.begin();
  }
  else {
    Serial.println("[Info] Button::toggle(): On");
    nextEffect = effectManager_.begin() + effect_;
  }
  effectManager_.selectEffect(nextEffect);
}

