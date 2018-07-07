#pragma once

#include "Light.h"
#include "EffectManager.h"

#include <cstdint>
#include <ESPAsyncUDP.h>

extern "C" {
  #include "os_type.h"
}

class RemoteUpdateEffect : public Effect {
public:

  RemoteUpdateEffect();
  ~RemoteUpdateEffect();
  
  void begin();
  void end();

  void effectStart() override;
  void stop() override;

  void run() override;

  
private:
  const uint16_t PORT = 5492;
  static const int BUFFER_SIZE = 128;
  static const int REMOTE_TIMEOUT = 1000;
  static constexpr char* LIGHT_NAME = "light";

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

  bool running;
  
  AsyncUDP socket;
  uint8_t buffer[BUFFER_SIZE];

  const int requiredDataLength[static_cast<int>(PacketType::COUNT)] = {0, 0, 0, 0, 1, 1, 2, 3, 2, -1};

  os_timer_t remoteTimer;
};
