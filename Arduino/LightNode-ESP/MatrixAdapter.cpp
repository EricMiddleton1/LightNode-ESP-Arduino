#include "MatrixAdapter.h"

#include <Arduino.h>

#include "LightDriver.h"

MatrixAdapter::MatrixAdapter(Light* _light, LightDriver* _driver, uint8_t _width, uint8_t _height, PixelMapper _mapper)
  : LightAdapter(_light, _driver, Type::Matrix)
  , width{_width}
  , height{_height}
  , mapper{_mapper} {
}

MatrixAdapter::~MatrixAdapter() {
}

uint8_t MatrixAdapter::getWidth() const {
  return width;
}

uint8_t MatrixAdapter::getHeight() const {
  return height;
}

void MatrixAdapter::setColor(uint8_t x, uint8_t y, const Color& c) {
  return driver->setColor(mapper.map(width, height, x, y), c);
}

