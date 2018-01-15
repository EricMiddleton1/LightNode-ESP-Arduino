#include "DebugPort.h"

#include <algorithm>

DebugPortClass DebugPort;

DebugPortClass::DebugPortClass() {
}

DebugPortClass::operator bool() const {
  return !clients_.empty();
}

void DebugPortClass::begin(uint16_t port) {
  server_ = std::unique_ptr<AsyncServer>(new AsyncServer{port});
  server_->begin();
  
  server_->onClient([this](void* arg, AsyncClient* client) {
    Serial.println("[Info] Client connected");
    
    clients_.push_back(client);
    client->onDisconnect([this](void* arg, AsyncClient* client) {
      Serial.println("[Info] Client disconnected");
      client->free();
      Serial.println("[Info] Client free'd");

      auto found = std::find(clients_.begin(), clients_.end(), client);
      if(found == clients_.end()) {
        Serial.println("[Error] Failed to find client in clients vector");
      }
      else {
        clients_.erase(found);
      }
    }, nullptr);
  }, nullptr);
}

void DebugPortClass::end() {
  server_->end();
  server_.reset();

  for(auto& client : clients_) {
    client->close(true);
    //client->free();
  }
}

bool DebugPortClass::print(const String& str) {
  for(auto& client : clients_) {
    client->add(str.c_str(), str.length());
    client->send();
  }

  return !clients_.empty();
}

bool DebugPortClass::println(const String& str) {
  return print(str + "\r\n");
}

