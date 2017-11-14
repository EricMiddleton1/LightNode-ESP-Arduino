#include "LightNode.h"

#include <Arduino.h>

extern "C" {
  #include "mem.h"
}

bool LightNode::Client::operator==(const Client& other) const {
  return (addr == other.addr) && (port == other.port);
}

LightNode::LightNode(const String& _name, Light* _lights[], int _lightCount)
  : name{_name}
  , lightCount{_lightCount} {

  lights = static_cast<Light**>(os_malloc(sizeof(Light*) * lightCount));
  memcpy(lights, _lights, sizeof(Light*)*lightCount);
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
  auto lightID = p.getLightID();
  if(lightID >= lightCount) {
    Serial.print("LightNode::processPacket: Invalid light ID: ");
    Serial.println(lightID);
  }
  else {
    auto& light = *lights[lightID];
    
    switch(p.getType()) {
      case Packet::Type::NodeInfo: {
        buffer[0] = lightCount;
        memcpy(buffer+1, name.c_str(), name.length());
        
        Packet response{Packet::Type::NodeInfoResponse, 0, buffer, 1 + name.length()};
  
        response.send(socket, from.addr, from.port);
      }
      break;
  
      case Packet::Type::LightInfo: {
        auto count = light.getCount();
        auto lightName = light.getName();
        
        buffer[0] = (count >> 8) && 0xFF;
        buffer[1] = count & 0xFF;
        memcpy(buffer+2, lightName.c_str(), lightName.length());
      
        Packet response{Packet::Type::LightInfoResponse, lightID, buffer, 2 + lightName.length()};

        response.send(socket, from.addr, from.port);
      }
      break;
  
      case Packet::Type::TurnOn:
        for(uint16_t i = 0; i < light.getCount(); ++i) {
          light[i].turnOn();
        }

        light.update();
      break;

      case Packet::Type::TurnOff:
        for(uint16_t i = 0; i < light.getCount(); ++i) {
          light[i].turnOff();
        }

        light.update();
      break;

      case Packet::Type::UpdateColor: {
        auto size = p.getDataLength();
        auto data = p.getData();
        
        auto colorMask = data[0];

        bool useHue = colorMask & 0x4,
          useSat = colorMask & 0x2,
          useVal = colorMask & 0x1;

        int stride = useHue + useSat + useVal;
        if(((size-1) % stride) != 0) {
          Serial.println("[Error] UpdateColor: Invalid size");
        }
        else {
          if(size == (1+stride)) {
            int offset = 1;
            
            if(useHue) {
              for(uint16_t i = 0; i < light.getCount(); ++i) {
                light[i].setHue(data[offset]);
              }
              ++offset;
            }

            if(useSat) {
              for(uint16_t i = 0; i < light.getCount(); ++i) {
                light[i].setSat(data[offset]);
              }
              ++offset;
            }

            if(useVal) {
              for(uint16_t i = 0; i < light.getCount(); ++i) {
                light[i].setVal(data[offset]);
              }
              ++offset;
            }
            else {
              for(uint16_t i = 0; i < light.getCount(); ++i) {
                light[i].turnOn();
              }
            }
          }
          else {
            if(size != (3*light.getCount()+1)) {
              Serial.print("[Error] UpdateColor: Invalid size:");
              Serial.println(size);
            }
            else {
              int offset = 1;
              
              for(uint16_t i = 0; i < light.getCount(); ++i) {
                if(useHue)
                  light[i].setHue(data[offset++]);
                if(useSat)
                  light[i].setSat(data[offset++]);
                if(useVal)
                  light[i].setVal(data[offset++]);

                if(!useVal && !light[i].isOn())
                  light[i].turnOn();
              }
            }
          }
        }

        light.update();
      }
      break;

      case Packet::Type::ChangeBrightness: {
        int delta = 255*static_cast<int>(static_cast<int8_t>(p.getData()[0]))/100;
        uint8_t val = constrain(light[0].getVal() + delta, 0, 255);

        for(uint16_t i = 0; i < light.getCount(); ++i) {
          light[i].setVal(val);
        }

        light.update();
      }
      break;      
    }
  }
}

