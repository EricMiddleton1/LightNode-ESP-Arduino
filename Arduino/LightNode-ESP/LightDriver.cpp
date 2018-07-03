#include "LightDriver.h"

#include "NeoPixelDriver.h"
#include "APA102Driver.h"
#include "WhiteDriver.h"
#include "AnalogDriver.h"

LightDriver::LightDriver(const String& type)
  : Serializable{SerializedName} {
}

LightDriver::~LightDriver() {
}

std::unique_ptr<LightDriver> LightDriver::Build(const JsonObject& config) {
  String type = config["driver"];
  type.toLowerCase();

  Serial.printf("[Info] LightDriver::Build: type=%s\n", type.c_str());

  std::unique_ptr<LightDriver> driver;

  if(type == "neopixel") {
    driver.reset(new NeoPixelDriver(config));
  }
  else if(type == "white") {
    driver.reset(new WhiteDriver(config));
  }
  else if(type == "analog") {
    driver.reset(new AnalogDriver(config));
  }
  else if(type == "apa102") {
    driver.reset(new APA102Driver(config));
  }
  else {
    Serial.printf("[Error] LightDriver::Build: invalid type: %s\n",
      type.c_str());
  }

  return std::move(driver);
}

String LightDriver::type() const {
  return type_;
}

