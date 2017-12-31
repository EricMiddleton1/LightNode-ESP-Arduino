#pragma once

#include <ESP8266WebServer.h>

class EffectManager;

class WebInterface {
public:
  WebInterface(EffectManager& effectManager);

  void begin();
  
  void run();

private:
  void cbWiFiConnect(const WiFiEventStationModeGotIP&);
  void cbWiFiDisconnect(const WiFiEventStationModeDisconnected&);

  String getEffects() const;

  EffectManager& effectManager_;
  ESP8266WebServer server;

  WiFiEventHandler connectHandler, disconnectHandler;
};

