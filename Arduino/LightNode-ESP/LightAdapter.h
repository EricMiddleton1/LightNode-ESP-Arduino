#pragma once

#include "Color.h"

class Driver;

class LightAdapter {
public:
  enum class Type {
    Linear,
    Matrix
  };
  
  LightAdapter(Driver* driver, Type type = Type::Linear);
  virtual ~LightAdapter();

  Type type() const;
  
  uint16_t size() const;

  Color getColor(uint16_t index) const;
  void setColor(uint16_t index, const Color&);

  void setAll(const Color& c);

  void replaceDriver(Driver* driver);

  void run();

protected:
  Driver* driver;

private:
  Type lightType;
  bool changed;
};

