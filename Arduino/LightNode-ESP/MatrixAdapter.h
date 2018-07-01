#pragma once


#include "LightAdapter.h"
#include "PixelMapper.h"

class MatrixAdapter : public LightAdapter {
public:
  MatrixAdapter(LightDriver* driver, uint8_t width, uint8_t height, PixelMapper mapper);
  ~MatrixAdapter();

  uint8_t getWidth() const;
  uint8_t getHeight() const;

  void setColor(uint8_t x, uint8_t y, const Color& c);
  
private:
  uint8_t width, height;
  PixelMapper mapper;
};

