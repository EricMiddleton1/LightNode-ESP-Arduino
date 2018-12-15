#pragma once

#include "IColorPalette.h"

#include <vector>

class ColorSet : public IColorPalette {
public:
  ColorSet(std::vector<Color>&& colors, bool randomOrder = true);

  Color getColor() override;
private:
  std::vector<Color> m_colors;
  bool m_randomOrder;
  int m_index;
};

