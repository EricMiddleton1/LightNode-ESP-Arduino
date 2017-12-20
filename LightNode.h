#pragma once

#include "Light.h"
#include "EffectManager.h"

#include <cstdint>
#include <ESPAsyncUDP.h>

extern "C" {
  #include "os_type.h"
}

class LightNode {
public:

  LightNode(const String& name, Light* lights[], int lightCount, EffectManager& manager);
  ~LightNode();
  
  void begin();
  void end();

  void run();
  
private:
  const uint16_t PORT = 5492;
  static const int BUFFER_SIZE = 128;
  static const int REMOTE_TIMEOUT = 1000;

  enum class PacketType {
    NodeInfo = 0,
    NodeInfoResponse,
    LightInfo,
    LightInfoResponse,
    TurnOn,
    TurnOff,
    SetBrightness,
    SetColor,
    ChangeBrightness,
    UpdateColor,

    COUNT
  };

  bool parsePacket(AsyncUDPPacket& packet, uint8_t& lightID, PacketType& type, uint8_t** data, int& length);
  void processPacket(AsyncUDPPacket packet);
  void cbRemoteTimer();
  
  AsyncUDP socket;
  uint8_t buffer[BUFFER_SIZE];

  const int requiredDataLength[static_cast<int>(PacketType::COUNT)] = {0, 0, 0, 0, 1, 1, 2, 3, 2, -1};

  String name;
  Light** lights;
  int lightCount;

  EffectManager& manager;
  int singleColorEffect;
  int remoteUpdateEffect;
  int prevEffect;

  //os_timer_t remoteTimer;
};
