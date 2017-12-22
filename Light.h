#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Color.h"

class LightAdapter;

class Light {
public:
  Light(const std::string& name, uint16_t count);
  virtual ~Light();

  Light(const Light& other) = delete;

  int size() const;
  std::string getName() const;

  LightAdapter* getAdapter();

  void update();
  
protected:
  friend class LightAdapter;

  virtual void display() = 0;
  
  std::string name;
  
  std::vector<Color> colors;
  bool changed;

  LightAdapter* adapter;
};
