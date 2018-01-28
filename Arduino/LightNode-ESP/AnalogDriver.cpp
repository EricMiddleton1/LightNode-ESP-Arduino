#include "AnalogDriver.h"

#include <Arduino.h>

AnalogDriver::AnalogDriver(uint8_t _pinR, uint8_t _pinG, uint8_t _pinB)
  : Driver{"Analog (PWM)"}
  , pinR{_pinR}
  , pinG{_pinG}
  , pinB{_pinB}
  , brightness{255} {

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

void AnalogDriver::setBrightness(uint8_t _brightness) {
  brightness = _brightness;
  display();
}

uint8_t AnalogDriver::getBrightness() const {
  return brightness;
}

void AnalogDriver::setColor(uint16_t index, const Color& _c) {
  c = _c;
}

void AnalogDriver::display() {
  auto corrected = gammaTable.Correct(RgbColor(c.getRed(), c.getGreen(), c.getBlue()));
  
  setLED(pinR, corrected.R);
  setLED(pinG, corrected.G);
  setLED(pinB, corrected.B);
}

void AnalogDriver::setLED(uint8_t pin, uint8_t value) {
  analogWrite(pin, (int)PWMRANGE * brightness * value / (255*255));
}

