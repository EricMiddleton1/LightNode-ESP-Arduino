#include "CapButton.h"

#include "DebugPort.h"

CapButton::CapButton(EffectManager& effectManager, const String& effect, uint8_t sendPin, uint8_t recvPin, Settings settings)
  : Button(effectManager, effect)
  , sensor_{sendPin, recvPin}
  , settings_(settings)
  , filtered{0.f}
  , pressed{false}
  , nextValidTime{0}
  , changeCount{0} {

  sensor_.set_CS_Timeout_Millis(10);
}

void CapButton::run() {
  unsigned long curTime = millis();
  auto capVal = sensor_.capacitiveSensor(1);
  filtered = 0.5f*filtered + 0.5f*capVal;
  filtered = min(static_cast<float>(settings_.maxValue), filtered);

  //Serial.print("\t\t\t\t");
  //Serial.println(filtered);
  DebugPort.println(String("\t\t\t\t") + String(filtered));

  if(!pressed && (filtered >= settings_.pressThreshold) && (curTime >= nextValidTime)) {
    changeCount++;
    if(changeCount >= settings_.changeCount) {
      DebugPort.println(String("[Info] Cap touched:\t") + String(filtered));

      toggle();
      pressed = true;
      changeCount = 0;
    }
  }
  else if(pressed && (filtered <= settings_.depressThreshold)) {
    changeCount++;
    if(changeCount >= settings_.changeCount) {
      DebugPort.println("[Info] Cap released");

      pressed = false;
      changeCount = 0;
      nextValidTime = curTime + settings_.debounceTime;
    }
  }
  else {
    changeCount = 0;
  }

  /*
  if(!pressed && (filtered >= settings_.pressThreshold) && (curTime >= nextValidTime)) {
    //Serial.print("[Info] Cap touched:\t");
    //Serial.println(filtered);
    DebugPort.println(String("[Info] Cap touched:\t") + String(filtered));
    
    toggle();
    
    pressed = true;
  }
  else if(pressed && (filtered <= settings_.depressThreshold)) {
    DebugPort.println("[Info] Cap released");
    
    pressed = false;
    nextValidTime = curTime + settings_.debounceTime;
  }
  */
}

