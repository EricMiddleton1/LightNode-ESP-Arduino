#pragma once


#include "LightDriver.h"

#include <NeoPixelBus.h>

class WhiteDriver : public LightDriver {
public:
  WhiteDriver(uint8_t pin);
  WhiteDriver(const JsonObject& config);
  ~WhiteDriver();

  uint16_t size() const override;

  uint8_t getBrightness() const override;
  void setBrightness(uint8_t brightness) override;
  
  void setColor(uint16_t index, const Color& c) override;
  
  void display() override;
  
private:
  void serializeThis(JsonObject& jsonConfig) const override;

  void setLED(uint8_t pin, uint8_t value);
  
  uint8_t pin;
  Color c;
  uint8_t brightness;

  NeoGamma<NeoGammaTableMethod> gammaTable;
};

