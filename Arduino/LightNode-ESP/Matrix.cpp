#include "Matrix.h"


Matrix::Matrix(const std::string& _name, uint8_t width, uint8_t height, PixelMapper mapper)
  : Light(_name, static_cast<uint16_t>(width)*height, &matrixAdapter)
  , matrixAdapter(*this, width, height, mapper) {
}
