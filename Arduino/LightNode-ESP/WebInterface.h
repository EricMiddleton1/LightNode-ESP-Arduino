#pragma once

#include <ESPAsyncWebServer.h>
#include <Arduino.h>

class EffectManager;
class Light;

class WebInterface {
public:
  WebInterface(EffectManager& effectManager, Light& light);

  void begin(const String& hostname);

private:
  void cbWiFiConnect(const WiFiEventStationModeGotIP&);
  void cbWiFiDisconnect(const WiFiEventStationModeDisconnected&);

  String getEffects() const;

  EffectManager& effectManager_;
  Light& light_;
  AsyncWebServer m_server;

  WiFiEventHandler connectHandler, disconnectHandler;

  String m_hostname;
};

