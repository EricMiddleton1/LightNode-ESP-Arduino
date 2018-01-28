#pragma once

#include <cstdint>

#include "Effect.h"

class RemoteUpdateEffect : public Effect {
public:
  RemoteUpdateEffect();

  void setColor(int index, const Color& c);

  void update(uint8_t huePeriod, uint8_t satPeriod, uint8_t valPeriod);

  void effectStart() override;
  void stop() override;
  void run() override;
};
