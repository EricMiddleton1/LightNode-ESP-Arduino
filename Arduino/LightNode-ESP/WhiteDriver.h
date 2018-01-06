#pragma once


#include "Driver.h"

class WhiteDriver : public Driver {
public:
  WhiteDriver(uint8_t pin);
  ~WhiteDriver();

  uint16_t size() const override;
  Color getColor(uint16_t index) const override;
  void setColor(uint16_t index, const Color& c) override;
  void display() override;
  
private:
  void setLED(uint8_t pin, uint8_t value);
  
  uint8_t pin;
  Color c;
};

