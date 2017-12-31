#include "WebInterface.h"
#include "EffectManager.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>

#include "html.h"

WebInterface::WebInterface(EffectManager& effectManager)
  : effectManager_(effectManager)
  , server{80} {
}

void WebInterface::begin() {
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
  server.on("/effects", HTTP_GET, [this]() {
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
  server.on("/select_effect", HTTP_GET, [this]() {
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

  const char * headers[] = {"effect"};

  server.begin();
  server.collectHeaders(headers, 1);

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

