#pragma once


#include "Driver.h"

#include <NeoPixelBus.h>

class WhiteDriver : public Driver {
public:
  WhiteDriver(uint8_t pin);
  ~WhiteDriver();

  uint16_t size() const override;

  uint8_t getBrightness() const override;
  void setBrightness(uint8_t brightness) override;
  
  void setColor(uint16_t index, const Color& c) override;
  
  void display() override;
  
private:
  void setLED(uint8_t pin, uint8_t value);
  
  uint8_t pin;
  Color c;
  uint8_t brightness;

  NeoGamma<NeoGammaTableMethod> gammaTable;
};

