#include "Light.h"

extern "C" {
  #include "mem.h"
}

Light::Light(const String& _name, uint16_t _count)
  : name{_name}
  , count{_count} {

  leds = static_cast<LED*>(os_malloc(count * sizeof(LED)));
  if(leds == nullptr) {
    count = 0;
  }
}

Light::~Light() {
  os_free(leds);
}

LED& Light::operator[](uint16_t index) {
  return leds[index];
}

const LED& Light::operator[](uint16_t index) const {
  return leds[index];
}

int Light::getCount() const {
  return count;
}

String Light::getName() const {
  return name;
}

