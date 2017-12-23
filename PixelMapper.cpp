#include "PixelMapper.h"

#include <Arduino.h>

PixelMapper::PixelMapper(Stride _stride, StrideOrder _strideOrder, Start _start)
  : stride{_stride}
  , strideOrder{_strideOrder}
  , start{_start} {
}

uint16_t PixelMapper::map(uint8_t width, uint8_t height, uint8_t x, uint8_t y) const {
  x = (start == Start::TopLeft || start == Start::BottomLeft) ? x : (width-x-1);
  y = (start == Start::TopLeft || start == Start::TopRight) ? y : (height-y-1);
  
  if(stride == Stride::Rows) {
    x = ((strideOrder == StrideOrder::ZigZag) && (y & 0x01)) ? (width-x-1) : x;
    return static_cast<uint16_t>(y)*width + x;
  }
  else {
    y = ((strideOrder == StrideOrder::ZigZag) && (x & 0x01)) ? (height-y-1) : y;
    return static_cast<uint16_t>(x)*height + y;
  }
}

