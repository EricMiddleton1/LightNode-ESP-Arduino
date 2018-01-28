#include "RemoteUpdateEffect.h"

#include <Arduino.h>


RemoteUpdateEffect::RemoteUpdateEffect()
  : Effect("Remote Update", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0) {
}

void RemoteUpdateEffect::setColor(int index, const Color& c) {
  return adapter->setColor(index, c);
}

void RemoteUpdateEffect::update(uint8_t huePeriod, uint8_t satPeriod, uint8_t valPeriod) {
  if(adapter != nullptr) {
    //adapter->display();
  }
}

void RemoteUpdateEffect::effectStart() {
  adapter->setAll({});
}

void RemoteUpdateEffect::stop() {
  
}

void RemoteUpdateEffect::run() {
  
}

