#pragma once

#include <cstdint>

#include "Effect.h"

#include "Color.h"

class SolidColorEffect : public Effect {
public:
  SolidColorEffect();

  void color(uint8_t hue, uint8_t sat);

  void effectStart() override;
  void stop() override;
  void run() override;

  std::vector<String> params() const override;
  String getParam(const String& param) const override;
  void setParam(const String& param, const String& value) override;

private:
  void display();

  uint8_t period;

  uint8_t hue, sat;
};

