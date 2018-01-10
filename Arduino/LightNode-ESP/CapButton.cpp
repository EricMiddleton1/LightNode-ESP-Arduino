#include "CapButton.h"

CapButton::CapButton(EffectManager& effectManager, const String& effect, uint8_t sendPin, uint8_t recvPin, Settings settings)
  : Button(effectManager, effect)
  , sensor_{sendPin, recvPin}
  , settings_(settings)
  , filtered{0.f}
  , pressed{false}
  , nextValidTime{0} {

  sensor_.set_CS_Timeout_Millis(10);
}

void CapButton::run() {
  unsigned long curTime = millis();
  auto capVal = sensor_.capacitiveSensor(1);
  filtered = 0.5f*filtered + 0.5f*capVal;

  //Serial.print("\t\t\t\t");
  //Serial.println(filtered);
  
  if(!pressed && (filtered >= settings_.pressThreshold) && (curTime >= nextValidTime)) {
    Serial.print("[Info] Cap touched:\t");
    Serial.println(filtered);
    
    toggle();
    
    pressed = true;
  }
  else if(pressed && (filtered <= settings_.depressThreshold)) {
    pressed = false;
    nextValidTime = curTime + settings_.debounceTime;
  }
}

