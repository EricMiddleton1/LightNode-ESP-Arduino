#pragma once

#include <cstdint>

class PixelMapper {
public:
  enum class Stride {
    Columns,
    Rows
  };
  enum class StrideOrder {
    Progressive,
    ZigZag
  };
  enum class Start {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };
  
  PixelMapper(Stride, StrideOrder, Start);
  
  uint16_t map(uint8_t width, uint8_t height, uint8_t x, uint8_t y) const;
private:
  Stride stride;
  StrideOrder strideOrder;
  Start start;
};
