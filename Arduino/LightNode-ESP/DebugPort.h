#pragma once

#include <Arduino.h>
#include <ESPAsyncTCP.h>

#include <vector>
#include <memory>

class DebugPortClass {
public:
  DebugPortClass();

  operator bool() const;

  void begin(uint16_t port);
  void end();

  bool print(const String& str);
  bool println(const String& str);
private:
  std::unique_ptr<AsyncServer> server_;
  std::vector<AsyncClient*> clients_;
};

extern DebugPortClass DebugPort;
