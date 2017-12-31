#pragma once

#include <Arduino.h>

#include "Light.h"

class AnalogLight : public Light {
public:
  AnalogLight(const std::string& name, byte pinR, byte pinG, byte pinB);

  void start();
  void stop();

  virtual void display() override;
private:
  void setLED(byte pin, byte value);
  
  byte pinR, pinG, pinB;
};
