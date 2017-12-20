#pragma once

#include "Color.h"

class Light;

class LightAdapter {
public:
  enum class Type {
    Linear,
    Matrix
  };
  
  LightAdapter(Light& light, Type type);
  virtual ~LightAdapter();

  Type type() const;
  
  int size() const;

  Color& operator[](int index);
  const Color& operator[](int index) const;

  void setAll(const Color& c);

  void display();

private:
  Light& light;
  Type lightType;
};

