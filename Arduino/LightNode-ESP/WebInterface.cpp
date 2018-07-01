#include "WebInterface.h"
#include "EffectManager.h"
#include "Light.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <FS.h>

WebInterface::WebInterface(EffectManager& effectManager, Light& light)
  : effectManager_(effectManager)
  , light_(light)
  , m_server{80} {
}

void WebInterface::begin(const String& name) {
  SPIFFS.begin();

  m_server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");

  m_server.on("/info", [this](AsyncWebServerRequest *request) {
    Serial.println("/info");

    StaticJsonBuffer<500> response;
    JsonObject& root = response.createObject();
    root["name"] = light_.getName();

    JsonObject& light = root.createNestedObject("light");
    light["driver"] = light_.getDriver()->name();
    light["count"] = light_.getAdapter()->size();

    JsonObject& network = root.createNestedObject("network");
    network["mode"] = "STA";
    JsonObject& station = network.createNestedObject("station");
    station["ssid"] = WiFi.SSID();
    station["connected"] = true;

    String responseStr;
    root.printTo(responseStr);
    request->send(200, "text/plain", responseStr);
  });

  m_server.on("/effects", [this](AsyncWebServerRequest *request) {
    Serial.println("/effects");
    
    StaticJsonBuffer<500> response;
    JsonObject& root = response.createObject();
    JsonArray& effects = root.createNestedArray("effects");
    for(const auto& effect : effectManager_) {
      effects.add(effect->getName().c_str());
    }
    root["active"] = effectManager_.getCurrentEffect() - effectManager_.begin();
    root["brightness"] = light_.getAdapter()->getBrightness();

    String responseStr;
    root.printTo(responseStr);

    Serial.print("Sending effects JSON object: ");
    Serial.println(responseStr);
    request->send(200, "text/plain", responseStr);
  });

  m_server.on("/select_effect", [this](AsyncWebServerRequest* request) {
    Serial.println("/select_effect");
    bool success = false;
    if(request->hasHeader("effect")) {
      auto effect = request->getHeader("effect")->value();
      Serial.print("effect: ");
      Serial.println(effect);
      
      success = effectManager_.selectEffect(effect);
      Serial.println(success ? "Success" : "No success");
    }
    request->send(200, "text/plain", String("{\"success\":") + (success ? "true" : "false") + "}");
  });
  
  m_server.on("/set", [this](AsyncWebServerRequest* request) {
    Serial.println("/set");
    bool success = false;
    if(request->hasHeader("brightness")) {
      auto brightness = request->getHeader("brightness")->value().toInt();
      Serial.print("brightness: ");
      Serial.println(brightness);

      if(brightness > 0 && brightness < 256) {
        light_.getAdapter()->setBrightness(brightness);
        success = true;
      }
    }
    Serial.println(success ? "Success" : "No success");
    request->send(200, "text/plain", String("{\"success\":") + (success ? "true" : "false") + "}");
  });

  m_server.begin();

  if(!MDNS.begin("lightnode")) {
    Serial.println("[Error] Failed to start MDNS service");
  }
  else {
    Serial.println("[Info] MDNS service started");
  }
}

