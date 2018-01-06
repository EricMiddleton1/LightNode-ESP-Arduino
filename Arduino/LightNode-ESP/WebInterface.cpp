#include "WebInterface.h"
#include "EffectManager.h"
#include "Light.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>

#include "html.h"

WebInterface::WebInterface(EffectManager& effectManager, Light& light)
  : effectManager_(effectManager)
  , light_(light)
  , server{80} {
}

void WebInterface::begin(const String& name) {
  server.on("/", HTTP_GET, [this]() {
    Serial.println("/");

    server.send(200, "text/html", HTML::index);
  });
  /*
  server.on("/style.css", HTTP_GET, [this]() {
    Serial.println("/style.css");

    server.send(200, "text/css", CSS::style);
  }
  */
  server.on("/info", [this]() {
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
    server.send(200, "text/plain", responseStr);
  });
  /*
  server.on("/settings", [this]() {
    Serial.println("/settings");

    DynamicJsonBuffer response;
    JsonObject& root = response.createObject();
    root["name"] = light_.getName();

    JsonObject& light = root.createNestedObject("light");
    JsonObject& driver = root
  }
  */
  server.on("/effects", [this]() {
    Serial.println("/effects");
    
    StaticJsonBuffer<500> response;
    JsonObject& root = response.createObject();
    JsonArray& effects = root.createNestedArray("effects");
    for(const auto& effect : effectManager_) {
      effects.add(effect->getName().c_str());
    }
    root["active"] = effectManager_.getCurrentEffect() - effectManager_.begin();

    String responseStr;
    root.printTo(responseStr);

    Serial.print("Sending effects JSON object: ");
    Serial.println(responseStr);
    server.send(200, "text/plain", responseStr);
  });
  server.on("/select_effect", [this]() {
    Serial.println("/select_effect");
    bool success = false;
    if(server.hasHeader("effect")) {
      auto effect = server.header("effect");
      Serial.print("effect: ");
      Serial.println(effect);
      
      success = effectManager_.selectEffect(effect);
      Serial.println(success ? "Success" : "No success");
    }
    server.send(200, "text/plain", String("{\"success\":") + (success ? "true" : "false") + "}");
  });

  const char * headers[] = {"effect", "name", "light_driver", "light_count", "network_ssid", "network_psk"};

  server.begin();
  server.collectHeaders(headers, sizeof(headers)/sizeof(headers[0]));

  if(!MDNS.begin("lightnode")) {
    Serial.println("[Error] Failed to start MDNS service");
  }
  else {
    Serial.println("[Info] MDNS service started");
  }
}

void WebInterface::run() {
  server.handleClient();
}

