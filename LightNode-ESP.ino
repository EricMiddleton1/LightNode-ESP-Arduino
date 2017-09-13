#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "AnalogLight.h"
#include "LightNode.h"

AnalogLight strip(2, 4, 5);
Light* lights[] = {&strip};

LightNode node("Test", lights, 1);


void setup() {
  Serial.begin(115200);

  Serial.print("\nConnecting to AP");
  
  WiFi.begin("108net", "3ricn3t1");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  strip.begin();

  Serial.print("Starting LightNode...");
  node.begin();
  Serial.println("done");
}

void loop() {
  node.run();
}
