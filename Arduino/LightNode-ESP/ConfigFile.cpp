#include "ConfigFile.h"

#include <FS.h>
#include <ArduinoJson.h>

ConfigFile::ConfigFile()
  : m_jsonRoot(init()) {
}

const JsonObject& ConfigFile::get() const {
  return m_jsonRoot;
}

const JsonObject& ConfigFile::get(String configObject) const {
  configObject.toLowerCase();
  return m_jsonRoot[configObject];
}

void ConfigFile::Write(const JsonObject& config) {
  auto file = SPIFFS.open(FILE_PATH, "w");
  String str;

  config.printTo(str);
  file.print(str);
  file.close();
}

JsonObject& ConfigFile::init() {
  if(!SPIFFS.exists(FILE_PATH)) {
    Serial.printf("[Error] ConfigFile::init(): Config file %s not found\n",
      FILE_PATH);
  }
  auto file = SPIFFS.open(FILE_PATH, "r");
  auto str = file.readString();
  file.close();

  Serial.printf("[Info] ConfigFile::init(): Config file contents: %s\n",
    str.c_str());

  return m_jsonBuffer.parseObject(str);
}
