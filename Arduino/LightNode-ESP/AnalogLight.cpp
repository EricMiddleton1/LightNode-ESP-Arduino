#include "AnalogLight.h"


AnalogLight::AnalogLight(const std::string& _name, byte _pinR, byte _pinG, byte _pinB)
  : Light{_name, 1}
  , pinR{_pinR}
  , pinG{_pinG}
  , pinB{_pinB} {
}

void AnalogLight::start() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  display();
}

void AnalogLight::stop() {
  
}

void AnalogLight::display() {
  setLED(pinR, colors[0].getRed());
  setLED(pinG, colors[0].getGreen());
  setLED(pinB, colors[0].getBlue());
}

void AnalogLight::setLED(byte pin, byte value) {
  analogWrite(pin, (int)PWMRANGE * value / 255);
}

