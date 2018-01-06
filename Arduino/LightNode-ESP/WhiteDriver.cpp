#include "WhiteDriver.h"

#include <Arduino.h>

WhiteDriver::WhiteDriver(uint8_t _pin)
  : Driver{"White"}
  , pin{_pin} {

  pinMode(pin, OUTPUT);

  display();
}

WhiteDriver::~WhiteDriver() {
  digitalWrite(pin, LOW);
}

uint16_t WhiteDriver::size() const {
  return 1;
}

Color WhiteDriver::getColor(uint16_t index) const {
  return c;
}

void WhiteDriver::setColor(uint16_t index, const Color& _c) {
  c = _c;
}

void WhiteDriver::display() {
  setLED(pin, c.getVal());
}

void WhiteDriver::setLED(uint8_t pin, uint8_t value) {
  analogWrite(pin, (int)PWMRANGE * value / 255);
}

