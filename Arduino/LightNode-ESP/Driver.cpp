#include "Driver.h"


Driver::Driver(const String& name)
  : name_{name} {
}

Driver::~Driver() {
}

String Driver::name() const {
  return name_;
}

