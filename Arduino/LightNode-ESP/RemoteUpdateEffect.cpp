#include "RemoteUpdateEffect.h"

#include <algorithm>

#include <Arduino.h>

#include "MatrixAdapter.h"
#include "SolidColorEffect.h"
#include "RemoteUpdateEffect.h"

extern "C" {
  #include "mem.h"
}

RemoteUpdateEffect::RemoteUpdateEffect()
  : Effect{"Remote Update", {LightAdapter::Type::Linear, LightAdapter::Type::Matrix}, 0}
  , running{false} {

  os_timer_setfn(&remoteTimer, [](void* lightNode) {
      reinterpret_cast<RemoteUpdateEffect*>(lightNode)->cbRemoteTimer();
    }, this);
}

RemoteUpdateEffect::~RemoteUpdateEffect() {
  stop();
}

void RemoteUpdateEffect::effectStart() {
  running = true;

  if(socket.listen(PORT)) {
    socket.onPacket([this](AsyncUDPPacket p) { processPacket(p); });
  }
  else {
    Serial.println("[Error] Failed to listen on UDP socket");
  }
}

void RemoteUpdateEffect::stop() {
  running = false;

  socket.close();
}

void RemoteUpdateEffect::run() {

}

bool RemoteUpdateEffect::parsePacket(AsyncUDPPacket& packet, uint8_t& lightID,
  PacketType& type, uint8_t** data, int& length) {
  int datagramLength = packet.length();
  if(datagramLength < 2) {
    return false;
  }
  else {
    uint8_t* datagram = packet.data();
    lightID = datagram[0];

    if(datagram[1] >= static_cast<int>(PacketType::COUNT)) {
      return false;
    }
    type = static_cast<PacketType>(datagram[1]);

    if(datagramLength > 2) {
      *data = datagram + 2;
      length = datagramLength - 2;
    }
    else {
      *data = nullptr;
      length = 0;
    }

    auto requiredLength = requiredDataLength[static_cast<int>(type)];
    if(requiredLength != -1 && length != requiredLength) {
      return false;
    }

    return true;
  }
}

void RemoteUpdateEffect::processPacket(AsyncUDPPacket packet) {
  if(!running) {
    return;
  }

  Serial.print(".");
  
  uint8_t lightID;
  PacketType type;
  uint8_t* data;
  int length;
  auto name = adapter->name();

  if(!parsePacket(packet, lightID, type, &data, length)) {
    Serial.println("[Error] Received invalid UDP datagram");
    return;
  }
  
  if(lightID >= 1) {
    Serial.print("RemoteUpdateEffect::processPacket: Invalid light ID: ");
    Serial.println(lightID);
  }
  else {
    switch(type) {
      case PacketType::NodeInfo: {
        buffer[0] = 0;
        buffer[1] = static_cast<uint8_t>(PacketType::NodeInfoResponse);
        buffer[2] = 1;
        memcpy(buffer+3, name.c_str(), name.length());
        packet.write(buffer, name.length() + 3);
      }
      break;
  
      case PacketType::LightInfo: {
        auto count = adapter->size();
        String lightName = LIGHT_NAME;
        
        buffer[0] = 0;
        buffer[1] = static_cast<uint8_t>(PacketType::LightInfoResponse);

        if(adapter->type() == LightAdapter::Type::Linear) {
          buffer[2] = 0;
          buffer[3] = (count >> 8) && 0xFF;
          buffer[4] = count & 0xFF;
        }
        else {
          auto* matrixAdapter = reinterpret_cast<MatrixAdapter*>(adapter);

          buffer[2] = 1;
          buffer[3] = matrixAdapter->getWidth();
          buffer[4] = matrixAdapter->getHeight();
        }
        memcpy(buffer+5, lightName.c_str(), lightName.length());
        packet.write(buffer, lightName.length() + 5);
      }
      break;
      
      case PacketType::UpdateColor: {
        int requiredLength = 3 + 3*adapter->size();
        if(length != requiredLength) {
          Serial.println("[Error] processPacket: Invalid size for UpdateColor");
          return;
        }
        
        auto periodHue = data[0];
        auto periodSat = data[1];
        auto periodVal = data[2];
        
        os_timer_disarm(&remoteTimer);
        os_timer_arm(&remoteTimer, REMOTE_TIMEOUT, 0);

        for(int i = 0; i < adapter->size(); ++i) {
          auto index = 3*i + 3;
          adapter->setColor(i, Color::HSV(data[index], data[index+1], data[index+2]));
        }
      }
      break;

      default:
        Serial.printf("[Error] RemoteUpdateEffect: Received packet of unknown type "
          "(%d)\n", static_cast<int>(type));
      break;
    }
  }
}

void RemoteUpdateEffect::cbRemoteTimer() {
  if(running) {
    adapter->setAll({});
  }
}

