#include "AnalogLight.h"


AnalogLight::AnalogLight(const String& _name, byte _pinR, byte _pinG, byte _pinB)
  : Light{_name, 1}
  , pinR{_pinR}
  , pinG{_pinG}
  , pinB{_pinB} {
}

void AnalogLight::begin() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  update();
}

void AnalogLight::end() {
  
}

void AnalogLight::update() {
  auto c = leds[0].getColor();
  
  setLED(pinR, c.getRed());
  setLED(pinG, c.getGreen());
  setLED(pinB, c.getBlue());
}

void AnalogLight::setLED(byte pin, byte value) {
  analogWrite(pin, (int)PWMRANGE * value / 255);
}

