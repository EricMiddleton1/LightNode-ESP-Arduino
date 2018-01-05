#pragma once

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

#include "LightAdapter.h"
#include "Driver.h"
#include "Color.h"

class Light {
public:
  Light(const std::string& name);
  ~Light();
  Light(const Light& other) = delete;

  operator bool() const;

  std::string getName() const;

  LightAdapter* getAdapter();

  void setDriver(std::unique_ptr<Driver>&&);
  void setAdapter(std::unique_ptr<LightAdapter>&&);

  void run();
  
protected:
  friend class LightAdapter;
  friend class MatrixAdapter;
  
  std::string name;

  std::unique_ptr<LightAdapter> adapter;
  std::unique_ptr<Driver> driver;
};

