#include "MatrixAdapter.h"

#include <Arduino.h>

#include "Driver.h"

MatrixAdapter::MatrixAdapter(Driver* _driver, uint8_t _width, uint8_t _height, PixelMapper _mapper)
  : LightAdapter(_driver, Type::Matrix)
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

Color MatrixAdapter::getColor(uint8_t x, uint8_t y) const {
  return driver->getColor(mapper.map(width, height, x, y));
}

void MatrixAdapter::setColor(uint8_t x, uint8_t y, const Color& c) {
  return driver->setColor(mapper.map(width, height, x, y), c);
}

