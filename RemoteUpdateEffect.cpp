#include "RemoteUpdateEffect.h"


RemoteUpdateEffect::RemoteUpdateEffect()
  : Effect("Remote Update", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0) {
}

Color& RemoteUpdateEffect::operator[](int index) {
  return (*adapter)[index];
}

const Color& RemoteUpdateEffect::operator[](int index) const {
  return (*adapter)[index];
}

void RemoteUpdateEffect::update(uint8_t huePeriod, uint8_t satPeriod, uint8_t valPeriod) {
  if(adapter != nullptr) {
    adapter->display();
  }
}

void RemoteUpdateEffect::effectStart() {
  
}

void RemoteUpdateEffect::stop() {
  
}

void RemoteUpdateEffect::run() {
  
}

