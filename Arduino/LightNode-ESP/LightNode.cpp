#include "LightNode.h"

#include <algorithm>

#include <Arduino.h>

#include "MatrixAdapter.h"
#include "SingleColorEffect.h"
#include "RemoteUpdateEffect.h"

extern "C" {
  #include "mem.h"
}

LightNode::LightNode(const String& _name, Light* _lights[], int _lightCount, EffectManager& _manager)
  : name{_name}
  , lightCount{_lightCount}
  , manager(_manager)
  , prevEffect{manager.getCurrentEffect() - manager.begin()} {

  lights = new Light*[lightCount];
  memcpy(lights, _lights, sizeof(Light*)*lightCount);

  lightUpdate = new bool[lightCount];
  for(int i = 0; i < lightCount; ++i) {
    lightUpdate[i] = false;
  }

  auto found = manager.findEffect("Single Color");
  if(found == manager.end()) {
    Serial.println("[Error] LightNode: Failed to find 'Single Color' effect");
    singleColorEffect = 0;
  }
  else {
    singleColorEffect = found - manager.begin();
  }

  found = manager.findEffect("Remote Update");
  if(found == manager.end()) {
    Serial.println("[Error] LightNode: Failed to find 'Remote Update' effect");
    remoteUpdateEffect = 0;
  }
  else {
    remoteUpdateEffect = found - manager.begin();
  }

  os_timer_setfn(&remoteTimer, [](void* lightNode) { reinterpret_cast<LightNode*>(lightNode)->cbRemoteTimer(); }, this);
}

LightNode::~LightNode() {
  end();

  delete[] lights;
  delete[] lightUpdate;
}

void LightNode::begin() {
  if(socket.listen(PORT)) {
    socket.onPacket([this](AsyncUDPPacket p) { processPacket(p); });
  }
  else {
    Serial.println("[Error] Failed to listen on UDP socket");
  }
}

void LightNode::end() {
  socket.close();
}

void LightNode::run() {
  for(int i = 0; i < lightCount; ++i) {
    lights[i]->update();
  }
}

bool LightNode::parsePacket(AsyncUDPPacket& packet, uint8_t& lightID, PacketType& type, uint8_t** data, int& length) {
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

void LightNode::processPacket(AsyncUDPPacket packet) {
  Serial.print(".");
  
  uint8_t lightID;
  PacketType type;
  uint8_t* data;
  int length;

  if(!parsePacket(packet, lightID, type, &data, length)) {
    Serial.println("[Error] Received invalid UDP datagram");
    return;
  }
  
  if(lightID >= lightCount) {
    Serial.print("LightNode::processPacket: Invalid light ID: ");
    Serial.println(lightID);
  }
  else {
    auto& light = *lights[lightID];
    auto& adapter = *(light.getAdapter());
    
    switch(type) {
      case PacketType::NodeInfo: {
        buffer[0] = 0;
        buffer[1] = static_cast<uint8_t>(PacketType::NodeInfoResponse);
        buffer[2] = lightCount;
        memcpy(buffer+3, name.c_str(), name.length());
        packet.write(buffer, name.length() + 3);
      }
      break;
  
      case PacketType::LightInfo: {
        auto count = light.size();
        auto lightName = light.getName();
        
        buffer[0] = 0;
        buffer[1] = static_cast<uint8_t>(PacketType::LightInfoResponse);

        if(adapter.type() == LightAdapter::Type::Linear) {
          buffer[2] = 0;
          buffer[3] = (count >> 8) && 0xFF;
          buffer[4] = count & 0xFF;
        }
        else {
          auto& matrixAdapter = reinterpret_cast<MatrixAdapter&>(adapter);

          buffer[2] = 1;
          buffer[3] = matrixAdapter.getWidth();
          buffer[4] = matrixAdapter.getHeight();
        }
        memcpy(buffer+5, lightName.c_str(), lightName.length());
        packet.write(buffer, lightName.length() + 5);
      }
      break;
  
      case PacketType::TurnOn: {
        manager.selectEffect(manager.begin() + singleColorEffect);
        auto* effect = static_cast<SingleColorEffect*>(*manager.getCurrentEffect());
        effect->transitionPeriod(data[0]);
        effect->on(true);
      }
      break;

      case PacketType::TurnOff: {
        manager.selectEffect(manager.begin() + singleColorEffect);
        auto* effect = static_cast<SingleColorEffect*>(*manager.getCurrentEffect());
        effect->transitionPeriod(data[0]);
        effect->on(false);
      }
      break;

      case PacketType::SetBrightness: {
        manager.selectEffect(manager.begin() + singleColorEffect);
        auto* effect = static_cast<SingleColorEffect*>(*manager.getCurrentEffect());
        effect->transitionPeriod(data[0]);
        effect->brightness(data[1]);
      }
      break;

      case PacketType::SetColor: {
        manager.selectEffect(manager.begin() + singleColorEffect);
        auto* effect = static_cast<SingleColorEffect*>(*manager.getCurrentEffect());
        effect->transitionPeriod(data[0]);
        effect->color(data[1], data[2]);
      }
      break;

      case PacketType::ChangeBrightness: {
        int delta = 255*static_cast<int>(static_cast<int8_t>(data[0]))/100;
        
        manager.selectEffect(manager.begin() + singleColorEffect);
        auto* effect = static_cast<SingleColorEffect*>(*manager.getCurrentEffect());
        effect->transitionPeriod(data[0]);
        effect->changeBrightness(delta);
      }
      break;

      case PacketType::UpdateColor: {
        int requiredLength = 3 + 3*light.size();
        if(length != requiredLength) {
          Serial.println("[Error] processPacket: Invalid size for UpdateColor");
          return;
        }
        
        auto periodHue = data[0];
        auto periodSat = data[1];
        auto periodVal = data[2];
        
        auto curEffect = manager.getCurrentEffect() - manager.begin();
        if(curEffect != remoteUpdateEffect) {
          prevEffect = curEffect;
          manager.selectEffect(manager.begin() + remoteUpdateEffect);
        }
        os_timer_disarm(&remoteTimer);
        os_timer_arm(&remoteTimer, REMOTE_TIMEOUT, 0);
        auto& effect = *static_cast<RemoteUpdateEffect*>(*manager.getCurrentEffect());

        for(int i = 0; i < effect.size(); ++i) {
          auto index = 3*i + 3;
          effect[i] = Color::HSV(data[index], data[index+1], data[index+2]);
        }
        effect.update(periodHue, periodSat, periodVal);
      }
      break;
    }
  }
}

void LightNode::cbRemoteTimer() {
  manager.selectEffect(manager.begin() + prevEffect);
}

