#include "TwinkleEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>

TwinkleEffect::TwinkleEffect()
  : Effect{"Twinkle", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD} {

  Serial.print("new [vector] (");
  Serial.print(sizeof(Twinkle)*twinkles.size());
  Serial.println(" bytes)");
}

void TwinkleEffect::effectStart() {
  twinkles.resize(adapter->size());

  Serial.print("new [vector resize] (");
  Serial.print(sizeof(Twinkle)*twinkles.size());
  Serial.println(" bytes)");

  auto curTime = millis();
  if(curTime < TWINKLE_PERIOD) {
    curTime = TWINKLE_PERIOD;
  }

  for(int i = 0; i < twinkles.size(); ++i) {
    twinkles[i].time = curTime - i*TWINKLE_PERIOD/twinkles.size();
    twinkles[i].hue = 85 * (rand() % 3);
  }

  std::random_shuffle(twinkles.begin(), twinkles.end());

  run();
}

void TwinkleEffect::stop() {
  twinkles.resize(0);
}

void TwinkleEffect::run() {
  auto curTime = millis();
  
  for(int i = 0; i < twinkles.size(); ++i) {
    uint8_t value;

    long int elapsed = static_cast<long int>(curTime) - twinkles[i].time;
    
    if(elapsed >= TWINKLE_PERIOD) {
      twinkles[i].time += TWINKLE_PERIOD;
      twinkles[i].hue = 85 * (rand() % 3);

      elapsed -= TWINKLE_PERIOD;
    }
    
    if(elapsed > TWINKLE_PERIOD/2) {
      value = 255 - 255*(elapsed - TWINKLE_PERIOD/2) / (TWINKLE_PERIOD/2);
    }
    else {
      value = 255*elapsed / (TWINKLE_PERIOD/2);
    }

    (*adapter)[i] = Color::HSV(twinkles[i].hue, 255, value);
  }
}

