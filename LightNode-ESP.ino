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
NeoPixelLight* digital;
Light* lights[1];

EffectManager* effectManager;//{*lights[0]->getAdapter()};
//SingleColorEffect singleColorEffect;
RemoteUpdateEffect* remoteUpdateEffect;
//TwinkleEffect twinkleEffect;

LightNode* node;

void setup() {
  Serial.begin(115200);

  //effectManager.addEffect(singleColorEffect);
  //effectManager.addEffect(remoteUpdateEffect);
  //effectManager.addEffect(twinkleEffect);

  digital = new NeoPixelLight("Test", 300, 2, NEO_RGB);
  lights[0] = digital;
  remoteUpdateEffect = new RemoteUpdateEffect();
  effectManager = new EffectManager(*lights[0]->getAdapter());
  effectManager->addEffect(*remoteUpdateEffect);
  

  Serial.print("new (");
  Serial.print(sizeof(LightNode));
  Serial.println(" bytes)");
  node = new LightNode("Outside", lights, 1, *effectManager);

  Serial.print("\nConnecting to AP");
  
  WiFi.begin("108net", "3ricn3t1");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  //analog.start();
  digital->start();

  Serial.print("Starting LightNode...");
  node->begin();
  Serial.println("done");

  //effectManager.selectEffect(effectManager.findEffect("Twinkle"));
}

unsigned long nextTime = 0;

void loop() {
  if(Serial.read() != -1) {
    node->end();
    delete node;

    delete effectManager;
    delete remoteUpdateEffect;
    delete digital;

    Serial.println("[Info] Stopped everything and deleted all the things");
  }
  
  auto curTime = millis();
  if(curTime >= nextTime) {
    nextTime = curTime + 1000;

    Serial.print("\t\t\t\t\t");
    Serial.println(ESP.getFreeHeap());
  }
}
