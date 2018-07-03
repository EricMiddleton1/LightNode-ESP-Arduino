#pragma once

#include <memory>

#include "Serializable.h"
#include "Color.h"

#include <Arduino.h>
#include <ArduinoJson.h>

class LightDriver : public Serializable {
public:
  LightDriver(const String& type);
  virtual ~LightDriver();

  static std::unique_ptr<LightDriver> Build(const JsonObject& config);

  String type() const;

  virtual uint16_t size() const = 0;

  virtual void setBrightness(uint8_t brightness) = 0;
  virtual uint8_t getBrightness() const = 0;
  
  virtual void setColor(uint16_t index, const Color& c) = 0;
  
  virtual void display() = 0;
private:
  static constexpr char* SerializedName = "light";

  String type_;
};

