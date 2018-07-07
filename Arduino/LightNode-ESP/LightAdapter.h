#pragma once

#include "Color.h"

#include <Arduino.h>

class LightDriver;
class Light;

class LightAdapter {
public:
  enum class Type {
    Linear,
    Matrix
  };
  
  LightAdapter(Light* light, LightDriver* driver, Type type = Type::Linear);
  virtual ~LightAdapter();

  String name() const;

  Type type() const;
  
  uint16_t size() const;

  uint8_t getBrightness() const;
  void setBrightness(uint8_t brightness);

  void setColor(uint16_t index, const Color&);

  void setAll(const Color& c);

  void replaceDriver(LightDriver* driver);

  void run();

protected:
  Light* light;
  LightDriver* driver;

private:
  Type lightType;
  bool changed;
};

