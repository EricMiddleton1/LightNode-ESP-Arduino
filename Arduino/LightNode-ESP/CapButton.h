#pragma once

#include <CapacitiveSensor.h>

#include "Button.h"

class CapButton : public Button {
public:
  struct Settings {
    int pressThreshold, depressThreshold, maxValue;
    unsigned int changeCount, debounceTime;
  };
  
  CapButton(EffectManager& effectManager, const String& effect, uint8_t sendPin, uint8_t recvPin, Settings settings);

  void run() override;
private:
  CapacitiveSensor sensor_;
  Settings settings_;

  float filtered;
  bool pressed;
  unsigned long changeCount;
  unsigned long nextValidTime;
};

