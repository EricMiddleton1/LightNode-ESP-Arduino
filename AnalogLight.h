#pragma once

#include <Arduino.h>

#include "Light.h"

class AnalogLight : public Light {
public:
  AnalogLight(const String& name, byte pinR, byte pinG, byte pinB);

  void begin();
  void end();

  virtual void update() override;
private:
  void setLED(byte pin, byte value);
  
  byte pinR, pinG, pinB;
};
