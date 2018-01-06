#pragma once

#include <Arduino.h>

class EffectManager;

class Button {
public:
  Button(EffectManager& effectManager, const String& effect);
  virtual ~Button();

  virtual void run();

  void toggle();
private:
  EffectManager& effectManager_;
  int effect_;
};

