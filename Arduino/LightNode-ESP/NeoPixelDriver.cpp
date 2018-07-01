#include "NeoPixelDriver.h"

#include <cmath>

NeoPixelDriver::NeoPixelDriver(uint16_t _ledCount, ColorOrder _colorOrder)
  : LightDriver{"neopixel"}
  , colorOrder{_colorOrder} {

  Serial.printf("[Info] NeoPixelDriver: LED Count=%d, ColorOrder=%s\n", (int)_ledCount,
    GetColorOrderString(_colorOrder).c_str());

  switch(colorOrder) {
    case ColorOrder::RGB:
      strip.rgb = new NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod>(_ledCount);
      strip.rgb->Begin();
      strip.rgb->Show();
    break;

    case ColorOrder::GRB:
      strip.grb = new NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>(_ledCount);
      strip.grb->Begin();
      strip.grb->Show();
    break;

    case ColorOrder::GRBW:
      strip.grbw = new NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod>(_ledCount);
      strip.grbw->Begin();
      strip.grbw->Show();
    break;
  }
}

NeoPixelDriver::NeoPixelDriver(const JsonObject& config)
  : NeoPixelDriver{config["count"], GetColorOrder(config["color_order"])} {
}

NeoPixelDriver::~NeoPixelDriver() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      delete strip.rgb;
    break;

    case ColorOrder::GRB:
      delete strip.grb;
    break;

    case ColorOrder::GRBW:
      delete strip.grbw;
    break;
  }
}

void NeoPixelDriver::serialize(JsonObject& jsonConfig) const {
  jsonConfig["type"] = type();
  jsonConfig["count"] = size();
  jsonConfig["color_order"] = GetColorOrderString(colorOrder);
}

uint16_t NeoPixelDriver::size() const {
  switch(colorOrder) {
    case ColorOrder::RGB:
      return size(strip.rgb);
    break;

    case ColorOrder::GRB:
      return size(strip.grb);
    break;

        case ColorOrder::GRBW:
      return size(strip.grbw);
    break;
  }
}

uint8_t NeoPixelDriver::getBrightness() const {
  switch(colorOrder) {
    case ColorOrder::RGB:
      return getBrightness(strip.rgb);
    break;

    case ColorOrder::GRB:
      return getBrightness(strip.grb);
    break;

    case ColorOrder::GRBW:
      return getBrightness(strip.grbw);
    break;
  }
}

void NeoPixelDriver::setBrightness(uint8_t brightness) {
  switch(colorOrder) {
    case ColorOrder::RGB:
      setBrightness(strip.rgb, brightness);
    break;

    case ColorOrder::GRB:
      setBrightness(strip.grb, brightness);
    break;

    case ColorOrder::GRBW:
      setBrightness(strip.grbw, brightness);
    break;
  }
}

void NeoPixelDriver::setColor(uint16_t index, const Color& c) {
  switch(colorOrder) {
    case ColorOrder::RGB:
      setColor(strip.rgb, index, c);
    break;

    case ColorOrder::GRB:
      setColor(strip.grb, index, c);
    break;

    case ColorOrder::GRBW:
      RgbwColor color(c.getRed(), c.getGreen(), c.getBlue());
      if( (color.R == color.G) && (color.G == color.B) ) {
        color = RgbwColor(0, 0, 0, c.getRed());
      }
      strip.grbw->SetPixelColor(index, gammaTable.Correct(color));
    break;
  }
}

void NeoPixelDriver::display() {
  switch(colorOrder) {
    case ColorOrder::RGB:
      display(strip.rgb);
    break;

    case ColorOrder::GRB:
      display(strip.grb);
    break;

    case ColorOrder::GRBW:
      display(strip.grbw);
    break;
  }
}

String NeoPixelDriver::GetColorOrderString(ColorOrder order) {
  switch(order) {
    case ColorOrder::RGB:
      return "rgb";
    break;

    case ColorOrder::GRB:
      return "grb";
    break;

    case ColorOrder::GRBW:
      return "grbw";
    break;
  }
}
  
NeoPixelDriver::ColorOrder NeoPixelDriver::GetColorOrder(String order) {
  if(order == "rgb") {
    return ColorOrder::RGB;
  }
  else if(order == "grb") {
    return ColorOrder::GRB;
  }
  else if(order == "grbw") {
    return ColorOrder::GRBW;
  }
  else {
    Serial.printf("[Error] Unknown color order: %s\n",
      order.c_str());

    return ColorOrder::RGB;
  }
}
  
