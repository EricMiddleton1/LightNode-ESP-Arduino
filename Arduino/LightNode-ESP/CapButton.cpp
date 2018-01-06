#include "CapButton.h"

CapButton::CapButton(EffectManager& effectManager, const String& effect, uint8_t sendPin, uint8_t recvPin, Settings settings)
  : Button(effectManager, effect)
  , sensor_{sendPin, recvPin}
  , settings_(settings)
  , pressed{false}
  , nextValidTime{0} {
}

void CapButton::run() {
  unsigned long curTime = millis();
  auto capVal = sensor_.capacitiveSensor(1);

  Serial.print("\t\t\t");
  Serial.println(capVal);
  
  if(!pressed && (capVal >= settings_.pressThreshold) && (curTime >= nextValidTime)) {
    toggle();
    
    pressed = true;
  }
  else if(pressed && (capVal <= settings_.depressThreshold)) {
    pressed = false;
    nextValidTime = curTime + settings_.debounceTime;
  }
}

