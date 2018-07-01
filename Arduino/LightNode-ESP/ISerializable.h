#pragma once

#include <ArduinoJson.h>

class ISerializable {
public:
  virtual void serialize(JsonObject& jsonBuffer) const = 0;
};
