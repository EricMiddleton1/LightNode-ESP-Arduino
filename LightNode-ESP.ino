#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "AnalogLight.h"
#include "NeoPixelLight.h"
#include "LightNode.h"

AnalogLight analog{"Bedroom", 2, 4, 5};
NeoPixelLight digital{"Test_Digital", 300, 0};
Light* lights[] = {&analog, &digital};

LightNode node("ESP", lights, 2);

void setup() {
  Serial.begin(115200);

  Serial.print("\nConnecting to AP");
  
  WiFi.begin(SSID, PSK);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  analog.begin();
  digital.begin();

  Serial.print("Starting LightNode...");
  node.begin();
  Serial.println("done");
}

void loop() {
  node.run();
}
