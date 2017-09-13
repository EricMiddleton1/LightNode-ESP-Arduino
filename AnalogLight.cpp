#include "AnalogLight.h"


AnalogLight::AnalogLight(byte _pinR, byte _pinG, byte _pinB)
  : Light(Light::Type::Analog, 1)
  , pinR{_pinR}
  , pinG{_pinG}
  , pinB{_pinB} {
}

void AnalogLight::begin() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  setAll({});
}

void AnalogLight::end() {
  
}

void AnalogLight::set(const Color* c) {
  setAll(*c);
}

void AnalogLight::setAll(Color c) {
  setLED(pinR, c.getRed());
  setLED(pinG, c.getGreen());
  setLED(pinB, c.getBlue());
}

void AnalogLight::setLED(byte pin, byte value) {
  analogWrite(pin, (int)PWMRANGE * value / 255);
}

