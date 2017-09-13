#ifndef ANALOG_LIGHT_H
#define ANALOG_LIGHT_H

#include <Arduino.h>

#include "Light.h"

class AnalogLight : public Light {
public:
  AnalogLight(byte pinR, byte pinG, byte pinB);

  void begin();
  void end();

  virtual void set(const Color[]) override;
  virtual void setAll(Color) override;

private:
  void setLED(byte pin, byte value);
  
  byte pinR, pinG, pinB;
};

#endif
