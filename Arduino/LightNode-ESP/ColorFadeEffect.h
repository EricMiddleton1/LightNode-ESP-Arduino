#pragma once

#include "Effect.h"

#include <vector>

class ColorFadeEffect : public Effect {
public:
  ColorFadeEffect();
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int FADE_RATE = 32;

  float hue;
};

