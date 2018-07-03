#include "Device.h"


Device::Device(JsonObject& json)
  : Serializable{SerializedName}
  , m_name{json["name"].as<String>()} {
}

String Device::name() const {
  return m_name;
}

void Device::serializeThis(JsonObject& jsonConfig) const {
  jsonConfig["name"] = m_name;
}
