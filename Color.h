#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>

class Color {
public:
  Color();
  Color(byte r, byte g, byte b);

  Color& operator=(const Color& c);
  bool operator!=(const Color& rhs) const;
  bool operator==(const Color& rhs) const;
  Color operator*(float rhs) const;

  String toString() const;

  byte getRed() const;
  byte getGreen() const;
  byte getBlue() const;

  void setRed(byte red);
  void setGreen(byte green);
  void setBlue(byte blue);

  float getHue() const;
  float getHSLSaturation() const;
  float getLightness() const;

  float getHSVSaturation() const;
  float getValue() const;

  Color filter(const Color&, float) const;
  Color gammaCorrect(float gamma) const;

  static Color HSL(float hue, float saturation, float lightness);
  static Color HSV(float hue, float saturation, float value);

private:
  byte r, g, b;
};

#endif
