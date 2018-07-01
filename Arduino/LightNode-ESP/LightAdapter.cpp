#include "LightAdapter.h"

#include "LightDriver.h"

LightAdapter::LightAdapter(LightDriver* _driver, Type _type)
  : driver(_driver)
  , lightType{_type}
  , changed{false} {
}

LightAdapter::~LightAdapter() {
}

LightAdapter::Type LightAdapter::type() const {
  return lightType;
}

uint16_t LightAdapter::size() const {
  if(driver == nullptr) {
    return 0;
  }
  else {
    return driver->size();
  }
}

uint8_t LightAdapter::getBrightness() const {
  return driver->getBrightness();
}

void LightAdapter::setBrightness(uint8_t brightness) {
  driver->setBrightness(brightness);
  changed = true;
}

void LightAdapter::setColor(uint16_t index, const Color& c) {
  driver->setColor(index, c);
  changed = true;
}

void LightAdapter::setAll(const Color& c) {
  for(int i = 0; i < driver->size(); ++i) {
    driver->setColor(i, c);
  }
  changed = true;
}

void LightAdapter::replaceDriver(LightDriver* _driver) {
  driver = _driver;
}

void LightAdapter::run() {
  if(changed) {
    changed = false;
    driver->display();
  }
}

