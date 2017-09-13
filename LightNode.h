#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include "Light.h"
#include "Packet.h"

#include <cstdint>
#include <WiFiUdp.h>

extern "C" {
  #include <os_type.h>
  #include <osapi.h>
}

class LightNode {
public:

  LightNode(const String& name, Light* lights[], int lightCount);
  ~LightNode();
  
  void begin();
  void end();

  void run();
  
private:
  const uint16_t PORT = 54923;
  static const int BUFFER_SIZE = 1024;

  struct Client {
    IPAddress addr;
    uint16_t port;

    bool operator==(const Client& other) const;
  };

  void processPacket(Client, const Packet&);

  void cbConnectTimer();
  void cbAliveTimer();
  
  WiFiUDP socket;
  byte buffer[BUFFER_SIZE];
  
  os_timer_t connectTimer, aliveTimer;
  bool connected;
  Client client;

  String name;
  Light** lights;
  int lightCount, analogCount, digitalCount, matrixCount;
};

#endif
