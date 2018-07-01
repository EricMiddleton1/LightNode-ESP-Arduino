#pragma once

#include <ArduinoJson.h>

class ConfigFile {
public:
  ConfigFile();

  const JsonObject& get() const;
  const JsonObject& get(String configObject) const;

  static void Write(const JsonObject& config);
private:
  static constexpr char* FILE_PATH = "/config.json";

  JsonObject& init();

  DynamicJsonBuffer m_jsonBuffer;
  JsonObject& m_jsonRoot;
};

