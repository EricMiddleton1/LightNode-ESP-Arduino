#include "Packet.h"

extern "C" {
  #include "mem.h"
}

Packet::Packet()
  : valid{false}
  , data{nullptr} {
}

Packet::Packet(Type _type, const byte _data[], int _length)
  : valid{true}
  , type{_type}
  , dataLength{_length} {

  if(dataLength > 0) {
    data = static_cast<byte*>(os_malloc(dataLength));
    if(data != nullptr) {
      memcpy(data, _data, dataLength);
    }
    else {
      valid = false;
    }
  }
}

Packet::Packet(WiFiUDP& socket)
  : valid{false}
  , data{nullptr} {

  if(socket.available() >= 3) {
    if( (socket.read() == HEADER[0]) && (socket.read() == HEADER[1]) ) {
      valid = true;
      type = static_cast<Type>(socket.read());

      dataLength = socket.available();
      if(dataLength > 0) {
        data = static_cast<byte*>(os_malloc(dataLength));
        if(data != nullptr) {
          socket.read(data, dataLength);
        }
        else {
          valid = false;
        }
      }
    }
  }
}
  

Packet::~Packet() {
  if(data != nullptr) {
    os_free(data);
  }
}

Packet::operator bool() const {
  return valid;
}

void Packet::send(WiFiUDP& socket, const IPAddress& addr, uint16_t port) {
  socket.beginPacket(addr, port);
  socket.write(HEADER[0]);
  socket.write(HEADER[1]);
  socket.write(static_cast<byte>(type));
  socket.write(data, dataLength);
  socket.endPacket();
}

Packet::Type Packet::getType() const {
  return type;
}

String Packet::getTypeString() const {
  int typeInt = static_cast<int>(type);
  if(typeInt < (sizeof(TYPE_NAMES)/sizeof(TYPE_NAMES[0]))) {
    return TYPE_NAMES[typeInt];
  }
  else {
    if(type == Type::Ack) {
      return "ack";
    }
    else if(type == Type::Nack) {
      return "nack";
    }
    else {
      return "unknown";
    }
  }
}

const byte* Packet::getData() const {
  return data;
}

int Packet::getDataLength() const {
  return dataLength;
}

