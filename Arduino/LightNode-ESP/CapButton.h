#pragma once

#include <CapacitiveSensor.h>

#include "Button.h"

class CapButton : public Button {
public:
  struct Settings {
    unsigned int pressThreshold, depressThreshold;
    unsigned int debounceTime;
  };
  
  CapButton(EffectManager& effectManager, const String& effect, uint8_t sendPin, uint8_t recvPin, Settings settings);

  void run() override;
private:
  CapacitiveSensor sensor_;
  Settings settings_;

  bool pressed;
  unsigned long nextValidTime;
};

