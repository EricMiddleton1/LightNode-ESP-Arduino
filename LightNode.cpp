#include "LightNode.h"

extern "C" {
  #include "mem.h"
}

bool LightNode::Client::operator==(const Client& other) const {
  return (addr == other.addr) && (port == other.port);
}

LightNode::LightNode(const String& _name, Light* _lights[], int _lightCount)
  : connected{false}
  , name{_name}
  , lightCount{_lightCount}
  , analogCount{0}
  , digitalCount{0}
  , matrixCount{0} {

  lights = static_cast<Light**>(os_malloc(sizeof(Light*) * lightCount));
  memcpy(lights, _lights, sizeof(Light*)*lightCount);
  
  for(int i = 0; i < lightCount; ++i) {
    switch(lights[i]->getType()) {
      case Light::Type::Analog:
        analogCount++;
      break;

      case Light::Type::Digital:
        digitalCount++;
      break;

      case Light::Type::Matrix:
        matrixCount++;
      break;
    }
  }

  os_timer_disarm(&connectTimer);
  os_timer_setfn(&connectTimer, [](void* self) { reinterpret_cast<LightNode*>(self)->cbConnectTimer(); }, this);

  os_timer_disarm(&aliveTimer);
  os_timer_setfn(&aliveTimer, [](void* self) { reinterpret_cast<LightNode*>(self)->cbAliveTimer(); }, this);
}

LightNode::~LightNode() {
  end();
}

void LightNode::begin() {
  socket.begin(PORT);
}

void LightNode::end() {
  socket.stop();
}

void LightNode::run() {
  int length;
  
  while( (length = socket.parsePacket()) > 0) {
    Packet p(socket);

    if(p) {
      processPacket({socket.remoteIP(), socket.remotePort()}, p);
    }
    else {
      Serial.println("[Error] LightNode: Failed to parse packet");
    }
  }
}

void LightNode::processPacket(Client from, const Packet& p) {
  bool fromClient = connected && (from == client);

  Serial.print("Received packet of type '"); Serial.print(p.getTypeString()); Serial.println("'");

  switch(p.getType()) {
    case Packet::Type::Ping: {
      //TODO: Support digital and matrix lights

      buffer[0] = analogCount;
      buffer[1] = digitalCount;
      buffer[2] = matrixCount;
      memcpy(buffer+3, name.c_str(), name.length());
      
      Packet response{Packet::Type::Info, buffer, 3 + name.length()};

      response.send(socket, from.addr, from.port);
    }
    break;

    case Packet::Type::Update:
      if( !connected || (connected && fromClient) ) {
        auto* data = p.getData();
        
        for(int i = 0; i < lightCount; ++i) {
          auto light = lights[i];
          
          auto* colors = static_cast<Color*>(os_malloc(sizeof(Color) * light->getCount()));

          for(int j = 0; j < light->getCount(); ++j) {
            int index = 3*j;
            colors[j] = {data[index], data[index+1], data[index+2]};
          }

          light->set(colors);

          os_free(colors);
        }
      }
     break;
  }
}

void LightNode::cbConnectTimer() {
  
}

void LightNode::cbAliveTimer() {
  
}

