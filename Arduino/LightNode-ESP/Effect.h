#pragma once

#include "LightAdapter.h"

#include <vector>

#include <Arduino.h>

class Effect {
public:
  Effect(const String& name, std::vector<LightAdapter::Type> supportedTypes, int updatePeriod);
  virtual ~Effect();

  const String& getName() const;
  const std::vector<LightAdapter::Type>& getSupportedTypes() const;
  int getUpdatePeriod() const;
  
  bool isSupported(const LightAdapter& light) const;

  int size() const;

  //--If child class has runtime parameters, ALL of the following methods must be overridden--//
  virtual std::vector<String> params() const;
  virtual String getParam(const String& param) const;
  virtual void setParam(const String& param, const String& value);
  //------------------------------------------------------------------------------------------//

  void start(LightAdapter& light);
  virtual void stop() = 0;

  virtual void run() = 0;
protected:
  virtual void effectStart() = 0;

  String name;
  std::vector<LightAdapter::Type> supportedTypes;
  int updatePeriod;

  LightAdapter* adapter;
};

