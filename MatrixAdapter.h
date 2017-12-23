#pragma once

#include <functional>

#include "LightAdapter.h"
#include "PixelMapper.h"

class MatrixAdapter : public LightAdapter {
public:
  MatrixAdapter(Light& light, uint8_t width, uint8_t height, PixelMapper mapper);
  ~MatrixAdapter();

  uint8_t getWidth() const;
  uint8_t getHeight() const;

  Color& get(uint8_t x, uint8_t y);
  const Color& get(uint8_t x, uint8_t y) const;

  Color& operator[](int index) override;
  const Color& operator[](int index) const override;
  
private:
  uint8_t width, height;
  PixelMapper mapper;
};

