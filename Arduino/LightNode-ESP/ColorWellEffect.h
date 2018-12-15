#pragma once

#include "Effect.h"
#include "IColorPalette.h"

#include <vector>
#include <memory>

class ColorWellEffect : public Effect {
public:
  ColorWellEffect(const String& name, const std::shared_ptr<IColorPalette>& palette);
  
  void effectStart() override;
  void stop() override;
  void run() override;

private:
  static const int UPDATE_PERIOD = 20;
  static const int WELL_COUNT = 4;

  struct ColorWell {
    Color color;
    float size, pos, vel;
  };

  std::shared_ptr<IColorPalette> m_palette;
  std::vector<ColorWell> wells;
  uint32_t lastTime;
};

