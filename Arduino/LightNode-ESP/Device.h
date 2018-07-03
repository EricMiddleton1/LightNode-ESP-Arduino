#pragma once

#include "Serializable.h"

#include <Arduino.h>

class Device : public Serializable {
public:
  Device(JsonObject& json);

  String name() const;

private:
  static constexpr char* SerializedName = "device";

  void serializeThis(JsonObject& jsonConfig) const override;

  String m_name;
};
