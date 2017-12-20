#include "LightAdapter.h"

#include "Light.h"

LightAdapter::LightAdapter(Light& _light, Type _type)
  : light(_light)
  , lightType{_type} {
}

LightAdapter::~LightAdapter() {
}

LightAdapter::Type LightAdapter::type() const {
  return lightType;
}

int LightAdapter::size() const {
  return light.size();
}

Color& LightAdapter::operator[](int index) {
  return light.colors[index];
}

const Color& LightAdapter::operator[](int index) const {
  return light.colors[index];
}

void LightAdapter::setAll(const Color& c) {
  for(auto& color : light.colors) {
    color = c;
  }
}

void LightAdapter::display() {
  light.display();
}

