#pragma once

#include <cstdint>

#include "Effect.h"

#include "Color.h"

class RandomColorEffect : public Effect {
public:
  RandomColorEffect();

  void effectStart() override;
  void stop() override;
  void run() override;

private:
  bool seeded;
};

