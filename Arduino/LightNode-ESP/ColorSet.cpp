#include "ColorSet.h"

#include <algorithm>

ColorSet::ColorSet(std::vector<Color>&& colors, bool randomOrder)
  : m_colors{std::move(colors)}
  , m_randomOrder{randomOrder}
  , m_index{0} {
    
  if(m_randomOrder) {
    std::random_shuffle(m_colors.begin(), m_colors.end());
  }
}

Color ColorSet::getColor() {
  auto c = m_colors[m_index++];

  if(m_index >= m_colors.size()) {
    m_index = 0;

    if(m_randomOrder) {
      std::random_shuffle(m_colors.begin(), m_colors.end());
    }
  }

  return c;
}

