#pragma once

#include <array>

extern "C" {
  #include "os_type.h"
}

class CapTouch {
public:
  CapTouch(int sendPin, int recvPin, int timeout);
  ~CapTouch();

  void begin();
  void end();

private:
  static const int ARRAY_SIZE = 100;

  void trigger();

  int sendPin_, recvPin_, timeout_;
  std::array<int, ARRAY_SIZE> samples_;
  int sampleCount_;

  unsigned long long startTime_;

  os_timer_t triggerTimer_, capTimeout_, debugTimer_;
};

