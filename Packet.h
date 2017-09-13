#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include <WiFiUdp.h>

class Packet {
public:
  enum class Type {
    Ping = 0,
    Init,
    Info,
    Update,
    Alive,
    TurnOn,
    TurnOff,
    SetBrightness,
    SetColorAll,

    Ack = 0xFE,
    Nack = 0xFF
  };
  
  Packet();
  Packet(Type type, const byte data[], int length);
  Packet(WiFiUDP&);
  ~Packet();

  void send(WiFiUDP&, const IPAddress&, uint16_t port);

  operator bool() const;

  Type getType() const;
  String getTypeString() const;
  const byte* getData() const;
  int getDataLength() const;
  
private:
  const byte HEADER[2] = {0xAA, 0x55};
  const char* TYPE_NAMES[9] = {"ping", "init", "info", "update", "alive", "turnon", "turnoff", "setbrightness", "setcolorall"};
  
  bool valid;

  Type type;
  byte *data;
  int dataLength;
};

#endif
