#pragma once

#include "Effect.h"

#include <vector>

class TwinkleEffect : public Effect {
public:
  TwinkleEffect();
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int TWINKLE_PERIOD = 1500;

  struct Twinkle {
    uint32_t time;
    uint8_t hue;
  };

  std::vector<Twinkle> twinkles;
};

