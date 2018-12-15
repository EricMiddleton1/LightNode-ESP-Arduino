#include "TwinkleEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>

TwinkleEffect::TwinkleEffect(const String& name, int _twinkleSize, int _gapSize, const std::shared_ptr<IColorPalette>& palette)
  : Effect{name, {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD}
  , twinkleSize{_twinkleSize}
  , gapSize{_gapSize}
  , m_palette{palette} {
}

void TwinkleEffect::effectStart() {
  adapter->setAll({});
  
  int stride = twinkleSize + gapSize;
  twinkles.resize(adapter->size() / stride);

  auto curTime = millis();
  if(curTime < TWINKLE_PERIOD) {
    curTime = TWINKLE_PERIOD;
  }

  for(int i = 0; i < twinkles.size(); ++i) {
    twinkles[i].time = curTime - i*TWINKLE_PERIOD/twinkles.size();
    twinkles[i].color = m_palette->getColor();
  }

  std::random_shuffle(twinkles.begin(), twinkles.end());

  run();
}

void TwinkleEffect::stop() {
  twinkles.resize(0);
}

void TwinkleEffect::run() {
  auto curTime = millis();

  int stride = twinkleSize + gapSize;
  
  for(int i = 0; i < twinkles.size(); ++i) {
    uint8_t value;

    long int elapsed = static_cast<long int>(curTime) - twinkles[i].time;
    
    if(elapsed >= TWINKLE_PERIOD) {
      twinkles[i].time += TWINKLE_PERIOD;
      twinkles[i].color = m_palette->getColor();

      elapsed -= TWINKLE_PERIOD;
    }
    
    if(elapsed > TWINKLE_PERIOD/2) {
      value = 255 - 255*(elapsed - TWINKLE_PERIOD/2) / (TWINKLE_PERIOD/2);
    }
    else {
      value = 255*elapsed / (TWINKLE_PERIOD/2);
    }

    auto c = twinkles[i].color;
    c.setRed((c.getRed() * value) / 255);
    c.setGreen((c.getGreen() * value) / 255);
    c.setBlue((c.getBlue() * value) / 255);

    int offset = i*stride;
    for(int j = 0; j < twinkleSize;  ++j) {
      adapter->setColor(j+offset, c);
    }
  }
}

