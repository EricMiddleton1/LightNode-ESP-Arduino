#pragma once

#include <ESP8266WebServer.h>
#include <Arduino.h>

class EffectManager;
class Light;

class WebInterface {
public:
  WebInterface(EffectManager& effectManager, Light& light);

  void begin(const String& name);
  
  void run();

private:
  void cbWiFiConnect(const WiFiEventStationModeGotIP&);
  void cbWiFiDisconnect(const WiFiEventStationModeDisconnected&);

  String getEffects() const;

  EffectManager& effectManager_;
  Light& light_;
  ESP8266WebServer server;

  WiFiEventHandler connectHandler, disconnectHandler;
};

