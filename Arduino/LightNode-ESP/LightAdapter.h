#pragma once

#include "Color.h"

class Light;

class LightAdapter {
public:
  enum class Type {
    Linear,
    Matrix
  };
  
  LightAdapter(Light& light, Type type = Type::Linear);
  virtual ~LightAdapter();

  Type type() const;
  
  int size() const;

  virtual Color& operator[](int index);
  virtual const Color& operator[](int index) const;

  void setAll(const Color& c);

protected:
  Light& light;

private:
  Type lightType;
};

