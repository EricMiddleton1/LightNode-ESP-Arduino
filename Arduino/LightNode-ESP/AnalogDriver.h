#pragma once


#include "Driver.h"

class AnalogDriver : public Driver {
public:
  AnalogDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB);
  ~AnalogDriver();

  uint16_t size() const override;
  Color getColor(uint16_t index) const override;
  void setColor(uint16_t index, const Color& c) override;
  void display() override;
  
private:
  void setLED(uint8_t pin, uint8_t value);
  
  uint8_t pinR, pinG, pinB;
  Color c;
};
