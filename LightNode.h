#pragma once

#include "Light.h"
#include "Packet.h"

#include <cstdint>
#include <WiFiUdp.h>

class LightNode {
public:

  LightNode(const String& name, Light* lights[], int lightCount);
  ~LightNode();
  
  void begin();
  void end();

  void run();
  
private:
  const uint16_t PORT = 5492;
  static const int BUFFER_SIZE = 1024;

  struct Client {
    IPAddress addr;
    uint16_t port;

    bool operator==(const Client& other) const;
  };

  void processPacket(Client, const Packet&);
  
  WiFiUDP socket;
  byte buffer[BUFFER_SIZE];

  String name;
  Light** lights;
  int lightCount;
};
