#pragma once

#include <cstdint>

#include "Effect.h"

#include "Color.h"

class SingleColorEffect : public Effect {
public:
  SingleColorEffect();

  void transitionPeriod(uint8_t period);
  
  void on(bool);
  void brightness(uint8_t brightness);
  void color(uint8_t hue, uint8_t sat);
  void changeBrightness(uint8_t delta);

  void effectStart() override;
  void stop() override;
  void run() override;

private:
  void display();

  bool isOn;
  uint8_t period;

  uint8_t hue, sat, val;
};

