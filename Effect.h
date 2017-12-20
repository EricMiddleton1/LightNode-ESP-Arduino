#pragma once

#include "LightAdapter.h"

#include <string>
#include <vector>

class Effect {
public:
  Effect(const std::string& name, std::vector<LightAdapter::Type> supportedTypes, int updatePeriod);
  virtual ~Effect();

  const std::string& getName() const;
  const std::vector<LightAdapter::Type>& getSupportedTypes() const;
  int getUpdatePeriod() const;
  
  bool isSupported(const LightAdapter& light) const;

  int size() const;

  void start(LightAdapter& light);
  virtual void stop() = 0;

  virtual void run() = 0;
protected:
  virtual void effectStart() = 0;

  std::string name;
  std::vector<LightAdapter::Type> supportedTypes;
  int updatePeriod;

  LightAdapter* adapter;
};

