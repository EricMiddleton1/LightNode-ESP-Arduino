#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncUDP.h>
#include <ArduinoJson.h>

#include "AnalogLight.h"
#include "NeoPixelLight.h"
#include "NeoPixelMatrix.h"
#include "LightNode.h"

#include "EffectManager.h"
#include "SingleColorEffect.h"
#include "RemoteUpdateEffect.h"
#include "ColorFadeEffect.h"
#include "TwinkleEffect.h"
#include "StrobeEffect.h"
#include "RandomColorEffect.h"
#include "ColorWipeEffect.h"

#include "WebInterface.h"

char* NAME = "kitchen";

AnalogLight analog{NAME, 14, 12, 13};
//NeoPixelLight digital(NAME, 100, NeoPixelLight::ColorOrder::RGB);
//NeoPixelMatrix matrix(NAME, 32, 8,
  //{PixelMapper::Stride::Columns, PixelMapper::StrideOrder::ZigZag, PixelMapper::Start::TopLeft});
Light* lights[] = {&analog};

EffectManager effectManager{*lights[0]->getAdapter()};
SingleColorEffect singleColorEffect;
RemoteUpdateEffect remoteUpdateEffect;
ColorFadeEffect colorFade;
TwinkleEffect twinkleEffect;
StrobeEffect strobeEffect;
RandomColorEffect randomColorEffect;
ColorWipeEffect colorWipeEffect;

WebInterface interface(effectManager);

LightNode* node;

void setup() {
  wifi_station_set_hostname(NAME);
  
  Serial.begin(115200);

  effectManager.addEffect(singleColorEffect);
  effectManager.addEffect(remoteUpdateEffect);
  effectManager.addEffect(randomColorEffect);
  effectManager.addEffect(colorFade);
  effectManager.addEffect(colorWipeEffect);
  effectManager.addEffect(twinkleEffect);
  effectManager.addEffect(strobeEffect);

  node = new LightNode(NAME, lights, 1, effectManager);

  Serial.print("\nConnecting to AP");
  
  WiFi.begin("108net", "3ricn3t1");
  //WiFi.begin("Eric is Awesome", "ericeric");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  Serial.println("Starting Matrix");
  analog.start();
  //digital.start();
  //matrix.start();
  Serial.println("Matrix started");

  Serial.println("Starting WebInterface");
  interface.begin(NAME);
  Serial.println("WebInterface started");

  Serial.print("Starting LightNode...");
  node->begin();
  Serial.println("done");

  effectManager.selectEffect("Off");
}

unsigned long nextTime = 0;

void loop() {
  interface.run();
  node->run();
  
  auto curTime = millis();
  if(curTime >= nextTime) {
    nextTime = curTime + 1000;

    Serial.print("\t\t\t\t\t");
    Serial.println(ESP.getFreeHeap());
  }
}
