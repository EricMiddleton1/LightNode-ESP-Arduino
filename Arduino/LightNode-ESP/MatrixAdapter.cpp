#include "MatrixAdapter.h"

#include <Arduino.h>

#include "Light.h"

MatrixAdapter::MatrixAdapter(Light& _light, uint8_t _width, uint8_t _height, PixelMapper _mapper)
  : LightAdapter(_light, Type::Matrix)
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

Color& MatrixAdapter::get(uint8_t x, uint8_t y) {
  light.changed = true;
  return light.colors[mapper.map(width, height, x, y)];
}

const Color& MatrixAdapter::get(uint8_t x, uint8_t y) const {
  return light.colors[mapper.map(width, height, x, y)];
}

Color& MatrixAdapter::operator[](int index) {
  uint8_t x = index % width,
    y = index / width;
  
  return get(x, y);
}

const Color& MatrixAdapter::operator[](int index) const {
  uint8_t x = index % width,
    y = index / width;

  return get(x, y);
}

