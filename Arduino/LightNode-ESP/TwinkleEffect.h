#pragma once

#include "Effect.h"

#include <vector>
#include <memory>

#include "IColorPalette.h"

class TwinkleEffect : public Effect {
public:
  TwinkleEffect(const String& name, int twinkleSize, int gapSize, const std::shared_ptr<IColorPalette>& palette);
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int TWINKLE_PERIOD = 1500;

  struct Twinkle {
    uint32_t time;
    Color color;
  };

  int twinkleSize, gapSize;
  std::shared_ptr<IColorPalette> m_palette;
  std::vector<Twinkle> twinkles;
};

