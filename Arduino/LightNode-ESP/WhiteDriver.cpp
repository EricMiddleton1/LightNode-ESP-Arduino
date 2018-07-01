#include "WhiteDriver.h"

#include <Arduino.h>

WhiteDriver::WhiteDriver(uint8_t _pin)
  : LightDriver{"white"}
  , pin{_pin}
  , brightness{255} {

  pinMode(pin, OUTPUT);

  display();
}

WhiteDriver::WhiteDriver(const JsonObject& config)
  : WhiteDriver{config["pin"].as<uint8_t>()} {
}

WhiteDriver::~WhiteDriver() {
  digitalWrite(pin, LOW);
}

void WhiteDriver::serialize(JsonObject& jsonConfig) const {
  jsonConfig["type"] = type();
  jsonConfig["pin"] = pin;
}

uint16_t WhiteDriver::size() const {
  return 1;
}

uint8_t WhiteDriver::getBrightness() const {
  return brightness;
}

void WhiteDriver::setBrightness(uint8_t _brightness) {
  brightness = _brightness;
  display();
}

void WhiteDriver::setColor(uint16_t index, const Color& _c) {
  c = _c;
}

void WhiteDriver::display() {
  auto corrected = gammaTable.Correct(RgbColor(c.getVal(), c.getVal(), c.getVal()));
  
  setLED(pin, corrected.R);
}

void WhiteDriver::setLED(uint8_t pin, uint8_t value) {
  analogWrite(pin, (int)PWMRANGE * value * brightness / (255*255));
}

