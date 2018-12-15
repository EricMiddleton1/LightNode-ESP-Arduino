#include "ColorWellEffect.h"

#include <algorithm>
#include <cmath>

#include <Arduino.h>

ColorWellEffect::ColorWellEffect(const String& name, const std::shared_ptr<IColorPalette>& palette)
  : Effect{name, {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, UPDATE_PERIOD}
  , m_palette{palette} {
}

void ColorWellEffect::effectStart() {
  for(int i = 0; i < WELL_COUNT; ++i) {
    float vel = 1000.f / ((rand() % 2001) + 2000); //Random [1/2, 2]
    vel = (rand() % 2) ? vel : -vel;
    float size = ((rand() % 71) + 30) / 1000.f;

    wells.push_back({m_palette->getColor(), size, static_cast<float>(i)/(WELL_COUNT-1), vel});
  }

  lastTime = millis();
}

void ColorWellEffect::stop() {
  wells.resize(0);
}

void ColorWellEffect::run() {
  //Serial.println("ColorWellEffect: Running");
  auto curTime = millis();
  float dt = (curTime - lastTime) / 1000.f;
  lastTime = curTime;

  //Serial.println("Clearing strip");
  adapter->setAll({});

  //Serial.println("Getting strip size");
  auto pixCount = adapter->size();

  //Serial.println("Creating strip color vector");
  std::vector<Color> stripColors(pixCount);

  for(int i = 0; i < wells.size(); ++i) {
    //Serial.printf("Loop i=%d (going to %d)\n", i, wells.size());
    //Serial.println("Creating this color");
    auto thisColor = wells[i].color;

    //Serial.println("Creating pixStart, pixEnd");
    int pixStart = pixCount*(wells[i].pos - wells[i].size/2.f), pixEnd = pixCount*(wells[i].pos + wells[i].size/2.f);
    //Serial.println("Comparing pixel range to boundaries");
    if((wells[i].vel > 0 && pixStart >= pixCount) || (wells[i].vel < 0 && pixEnd < 0)) {
      //Serial.println("Reversing color well");
      wells[i].vel *= -1.f;
      wells[i].color = m_palette->getColor();
    }

    //Serial.println("Starting well draw loop");
    for(int p = pixStart; p <= pixEnd; ++p) {
      if(p >= 0 && p < pixCount) {
        //Serial.printf("Getting current strip color at pixel %d\n", p);
        auto curColor = stripColors[p];

        int r = curColor.getRed() + thisColor.getRed(),
          g = curColor.getGreen() + thisColor.getGreen(),
          b = curColor.getBlue() + thisColor.getBlue();

        Color newColor{min(r, 255), min(g, 255), min(b, 255)};
        //Serial.println("Setting strip color");
        adapter->setColor(p, newColor);
        //Serial.println("Storing local strip color");
        stripColors[p] = newColor;
      }
    }

    //Serial.println("Updating well position");
    wells[i].pos += wells[i].vel*dt;
  }
  //Serial.println("ColorWellEffect: Done running");
}

