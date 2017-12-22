#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>

#include "AnalogLight.h"
#include "NeoPixelLight.h"
#include "LightNode.h"

#include "EffectManager.h"
#include "SingleColorEffect.h"
#include "RemoteUpdateEffect.h"
#include "TwinkleEffect.h"

//AnalogLight analog{"Bedroom", 2, 4, 5};
NeoPixelLight digital("Test", 256, NeoPixelLight::ColorOrder::GRB);
Light* lights[] = {&digital};

EffectManager effectManager{*lights[0]->getAdapter()};
SingleColorEffect singleColorEffect;
RemoteUpdateEffect remoteUpdateEffect;
TwinkleEffect twinkleEffect;

LightNode node("Outside", lights, 1, effectManager);

void setup() {
  Serial.begin(115200);

  effectManager.addEffect(singleColorEffect);
  effectManager.addEffect(remoteUpdateEffect);
  effectManager.addEffect(twinkleEffect);

  Serial.print("\nConnecting to AP");
  
  //WiFi.begin("108net", "3ricn3t1");
  WiFi.begin("Eric is Awesome", "ericeric");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  //analog.start();
  digital.start();

  Serial.print("Starting LightNode...");
  node.begin();
  Serial.println("done");

  effectManager.selectEffect(effectManager.findEffect("Twinkle"));
}

unsigned long nextTime = 0;

void loop() {
  node.run();
  
  auto curTime = millis();
  if(curTime >= nextTime) {
    nextTime = curTime + 1000;

    Serial.print("\t\t\t\t\t");
    Serial.println(ESP.getFreeHeap());
  }
}
