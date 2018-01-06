#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include <Arduino.h>

#include "LightAdapter.h"
#include "Driver.h"
#include "Color.h"

class Light {
public:
  Light(const String& name);
  ~Light();
  Light(const Light& other) = delete;

  operator bool() const;

  String getName() const;

  LightAdapter* getAdapter();
  const Driver* getDriver() const;

  void setDriver(std::unique_ptr<Driver>&&);
  void setAdapter(std::unique_ptr<LightAdapter>&&);

  void run();
  
protected:
  friend class LightAdapter;
  friend class MatrixAdapter;
  
  String name;

  std::unique_ptr<LightAdapter> adapter;
  std::unique_ptr<Driver> driver;
};

