#include "Serializable.h"

Serializable::Serializable(String&& name)
  : m_name{std::move(name)} {
}

String Serializable::serializedName() const {
  return m_name;
}

void Serializable::serialize(JsonObject& parentObject) const {
  JsonObject& thisObject = parentObject.createNestedObject(m_name.c_str());

  //Serialize this object first
  serializeThis(thisObject);

  //Serialize child serializable objects next
  for(const auto& child : m_children) {
    child->serialize(thisObject);
  }
}

void Serializable::setSerializableChildren(std::vector<Serializable*>&& children) {
  m_children = std::move(children);
}

JsonObject& Serializable::getSerializedChild(JsonObject& json,
  const std::string& childName) {
  
  return json[childName.c_str()];
}
