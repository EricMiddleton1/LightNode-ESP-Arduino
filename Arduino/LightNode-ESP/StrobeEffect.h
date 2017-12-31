#pragma once

#include "Effect.h"

#include <vector>

class StrobeEffect : public Effect {
public:
  StrobeEffect();
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int STROBE_PERIOD = 5;

  unsigned int tick;
};

