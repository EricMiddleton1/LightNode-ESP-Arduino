#pragma once


#include "LightDriver.h"

#include <NeoPixelBus.h>

class AnalogDriver : public LightDriver {
public:
  AnalogDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB);
  AnalogDriver(const JsonObject& config);
  ~AnalogDriver();

  uint16_t size() const override;

  void setBrightness(uint8_t brightness) override;
  uint8_t getBrightness() const override;
  
  void setColor(uint16_t index, const Color& c) override;
  
  void display() override;
  
private:
  void serializeThis(JsonObject& jsonConfig) const override;
  
  void setLED(uint8_t pin, uint8_t value);

  uint8_t brightness;
  uint8_t pinR, pinG, pinB;
  Color c;

  NeoGamma<NeoGammaTableMethod> gammaTable;
};
