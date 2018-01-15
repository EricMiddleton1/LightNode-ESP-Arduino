#include "CapTouch.h"

#include <Arduino.h>

#include "DebugPort.h"

static CapTouch* __instance;

CapTouch::CapTouch(int sendPin, int recvPin, int timeout)
  : sendPin_{sendPin}
  , recvPin_{recvPin}
  , timeout_{timeout}
  , sampleCount_{0} {
}

CapTouch::~CapTouch() {
  end();
}

void CapTouch::begin() {
  __instance = this;

  os_timer_setfn(&triggerTimer_, [](void* arg) { static_cast<CapTouch*>(arg)->trigger(); }, this);
  os_timer_setfn(&capTimeout_, [](void* arg) {
      auto* instance = static_cast<CapTouch*>(arg);
      
      if(instance->sampleCount_ < instance->samples_.size()) {
        instance->samples_[instance->sampleCount_++] = -1;
      }

      pinMode(instance->recvPin_, OUTPUT);
      digitalWrite(instance->recvPin_, LOW);

      os_timer_arm(&instance->triggerTimer_, 1, 0);
    }, this);
  os_timer_setfn(&debugTimer_, [](void* arg) {
      auto* instance = static_cast<CapTouch*>(arg);

      int count;
      noInterrupts();
      count = instance->sampleCount_;
      interrupts();

      String report;
      for(int i = 0; i < count; ++i) {
        report += String(instance->samples_[i]) + "\r\n";
      }

      noInterrupts();
      instance->sampleCount_ = 0;
      interrupts();

      Serial.print(report);
    }, this);

  sampleCount_ = 0;
  
  pinMode(sendPin_, OUTPUT);
  pinMode(recvPin_, OUTPUT);

  digitalWrite(sendPin_, LOW);
  digitalWrite(recvPin_, LOW);
  delayMicroseconds(10);

  attachInterrupt(digitalPinToInterrupt(recvPin_), []() {
    auto curTime = ESP.getCycleCount();

    os_timer_disarm(&__instance->capTimeout_);

    if(__instance->sampleCount_ < __instance->samples_.size()) {
      __instance->samples_[__instance->sampleCount_++] = curTime - __instance->startTime_;
    }

    digitalWrite(__instance->sendPin_, LOW);
    pinMode(__instance->recvPin_, OUTPUT);
    digitalWrite(__instance->recvPin_, LOW);

    os_timer_arm(&__instance->triggerTimer_, 1, 0);
  }, RISING);

  trigger();

  os_timer_arm(&debugTimer_, 10, 1);
}

void CapTouch::end() {
  os_timer_disarm(&triggerTimer_);
  os_timer_disarm(&capTimeout_);
  os_timer_disarm(&debugTimer_);
  
  detachInterrupt(digitalPinToInterrupt(recvPin_));
}

void CapTouch::trigger() {
  pinMode(recvPin_, INPUT);
  digitalWrite(sendPin_, HIGH);
  startTime_ = ESP.getCycleCount();

  os_timer_arm(&capTimeout_, timeout_, 0);
}

