#pragma once

#include "Effect.h"

#include <vector>

class ColorWipeEffect : public Effect {
public:
  ColorWipeEffect();
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int FADE_RATE = 64;

  float hue, mult;
};

