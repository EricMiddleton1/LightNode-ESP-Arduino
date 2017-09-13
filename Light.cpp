#include "Light.h"


Light::Light(Type _type, int _count)
  : type{_type}
  , count{_count} {
}

Light::Type Light::getType() const {
  return type;
}

int Light::getCount() const {
  return count;
}

