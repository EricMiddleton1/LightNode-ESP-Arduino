#pragma once

#include "Light.h"

#include "MatrixAdapter.h"
#include "PixelMapper.h"

class Matrix : public Light {
public:
  Matrix(const std::string& name, uint8_t width, uint8_t height, PixelMapper mapper);

private:
  MatrixAdapter matrixAdapter;
};
