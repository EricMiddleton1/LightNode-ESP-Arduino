#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>

#include "AnalogLight.h"
#include "NeoPixelLight.h"
#include "NeoPixelMatrix.h"
#include "LightNode.h"

#include "EffectManager.h"
#include "SingleColorEffect.h"
#include "RemoteUpdateEffect.h"
#include "TwinkleEffect.h"

//AnalogLight analog{"Bedroom", 2, 4, 5};
//NeoPixelLight digital("Test", 256, NeoPixelLight::ColorOrder::GRB);
NeoPixelMatrix matrix("Test Matrix", 32, 8,
  {PixelMapper::Stride::Columns, PixelMapper::StrideOrder::ZigZag, PixelMapper::Start::TopLeft});
Light* lights[] = {&matrix};

EffectManager effectManager{*lights[0]->getAdapter()};
SingleColorEffect singleColorEffect;
RemoteUpdateEffect remoteUpdateEffect;
TwinkleEffect twinkleEffect;

LightNode* node;

void setup() {
  Serial.begin(115200);

  effectManager.addEffect(singleColorEffect);
  effectManager.addEffect(remoteUpdateEffect);
  effectManager.addEffect(twinkleEffect);

  node = new LightNode("Outside", lights, 1, effectManager);

  Serial.print("\nConnecting to AP");
  
  //WiFi.begin("108net", "3ricn3t1");
  WiFi.begin("Eric is Awesome", "ericeric");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  Serial.println("Starting Matrix");
  //digital.start();
  matrix.start();
  Serial.println("Matrix started");

  Serial.print("Starting LightNode...");
  node->begin();
  Serial.println("done");

  //effectManager.selectEffect(effectManager.findEffect("Twinkle"));
}

unsigned long nextTime = 0;

void loop() {
  node->run();
  
  auto curTime = millis();
  if(curTime >= nextTime) {
    nextTime = curTime + 1000;

    Serial.print("\t\t\t\t\t");
    Serial.println(ESP.getFreeHeap());
  }
}
