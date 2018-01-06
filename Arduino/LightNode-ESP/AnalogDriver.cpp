#include "AnalogDriver.h"

#include <Arduino.h>

AnalogDriver::AnalogDriver(uint8_t _pinR, uint8_t _pinG, uint8_t _pinB)
  : Driver{"Analog (PWM)"}
  , pinR{_pinR}
  , pinG{_pinG}
  , pinB{_pinB} {

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  display();
}

AnalogDriver::~AnalogDriver() {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, LOW);
}

uint16_t AnalogDriver::size() const {
  return 1;
}

Color AnalogDriver::getColor(uint16_t index) const {
  return c;
}

void AnalogDriver::setColor(uint16_t index, const Color& _c) {
  c = _c;
}

void AnalogDriver::display() {
  setLED(pinR, c.getRed());
  setLED(pinG, c.getGreen());
  setLED(pinB, c.getBlue());
}

void AnalogDriver::setLED(uint8_t pin, uint8_t value) {
  analogWrite(pin, (int)PWMRANGE * value / 255);
}

