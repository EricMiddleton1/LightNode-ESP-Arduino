#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>

class Packet {
public:
  enum class Type {
    NodeInfo = 0,
    NodeInfoResponse,
    LightInfo,
    LightInfoResponse,
    TurnOn,
    TurnOff,
    UpdateColor,
    ChangeBrightness
  };
  
  Packet();
  Packet(Type type, uint8_t lightID, const byte data[], int length);
  Packet(WiFiUDP&);
  ~Packet();

  void send(WiFiUDP&, const IPAddress&, uint16_t port);

  operator bool() const;

  Type getType() const;
  uint8_t getLightID() const;
  String getTypeString() const;
  const byte* getData() const;
  int getDataLength() const;
  
private:
  const char* TYPE_NAMES[8] = {"NodeInfo", "NodeInfoResponse", "LightInfo", "LightInfoResponse", "TurnOn", "TurnOff", "UpdateColor",
    "ChangeBrightness"};
  
  bool valid;

  Type type;
  uint8_t lightID;
  byte *data;
  int dataLength;
};
