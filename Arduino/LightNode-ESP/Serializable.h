#pragma once

#include <vector>
#include <string>
#include <memory>

#include <Arduino.h>
#include <ArduinoJson.h>

class Serializable {
public:
  Serializable(String&& name);
  
  String serializedName() const;

  void serialize(JsonObject& jsonBuffer) const;

protected:
  virtual void serializeThis(JsonObject& jsonBuffer) const = 0;

  void setSerializableChildren(std::vector<Serializable*>&& children);
  
  JsonObject& getSerializedChild(JsonObject& json, const std::string& childName);

  template<typename ChildType>
  std::unique_ptr<ChildType> deserialize(JsonObject& json) {
    return new ChildType(json[ChildType::SerializedName]);
  }

private:
  String m_name;
  std::vector<Serializable*> m_children;
};
