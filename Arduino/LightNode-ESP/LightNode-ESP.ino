#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncUDP.h>
#include <ArduinoJson.h>
#include <CapacitiveSensor.h>

#include <memory>

#include "DebugPort.h"

#include "Light.h"
#include "WhiteDriver.h"
#include "AnalogDriver.h"
#include "NeoPixelDriver.h"
#include "LightAdapter.h"
#include "MatrixAdapter.h"
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
#include "Button.h"
#include "CapButton.h"

#include "CapTouch.h"

char* NAME = "Kitchen";
const uint16_t DEBUG_PORT = 1234;

/*
Light light{NAME};
AnalogDriver driver{14, 12, 13};
LightAdapter adapter{&driver, LightAdapter::Type::Linear};
Light* lights[] = {&light};


//NeoPixelLight digital(NAME, 100, NeoPixelLight::ColorOrder::RGB);
//NeoPixelMatrix matrix(NAME, 32, 8,
  //{PixelMapper::Stride::Columns, PixelMapper::StrideOrder::ZigZag, PixelMapper::Start::TopLeft});
*/

//EffectManager effectManager{*lights[0]->getAdapter()};
SingleColorEffect singleColorEffect;
RemoteUpdateEffect remoteUpdateEffect;
ColorFadeEffect colorFade;
TwinkleEffect twinkleEffect;
StrobeEffect strobeEffect;
RandomColorEffect randomColorEffect;
ColorWipeEffect colorWipeEffect;

//WebInterface interface(effectManager);

Light* light;
EffectManager* effectManager;
LightNode* node;
WebInterface* interface;
Button* button = nullptr;

CapTouch capTouch(5, 4, 1000);

void setup() {
  wifi_station_set_hostname(NAME);
  
  Serial.begin(115200);
  DebugPort.begin(DEBUG_PORT);

  light = new Light{NAME};
  //light->setDriver(std::unique_ptr<Driver>(new AnalogDriver(14, 12, 13)));
  //light->setDriver(std::unique_ptr<Driver>(new NeoPixelDriver(50, NeoPixelDriver::ColorOrder::RGB)));
  light->setDriver(std::unique_ptr<Driver>(new WhiteDriver(3)));
  light->setAdapter(std::unique_ptr<LightAdapter>(new LightAdapter(nullptr)));

  effectManager = new EffectManager{*light->getAdapter()};
  effectManager->addEffect(singleColorEffect);
  effectManager->addEffect(remoteUpdateEffect);
  effectManager->addEffect(randomColorEffect);
  effectManager->addEffect(colorFade);
  effectManager->addEffect(colorWipeEffect);
  effectManager->addEffect(twinkleEffect);
  effectManager->addEffect(strobeEffect);

  Light* lights[] = {light};

  node = new LightNode(NAME, lights, 1, *effectManager); //1, 2
  //button = new CapButton(*effectManager, "Single Color", 2, 1, {50, 10, 200, 5, 50});
  interface = new WebInterface(*effectManager, *light);

  Serial.print("\nConnecting to AP");
  
  WiFi.begin("108net", "3ricn3t1");
  //WiFi.begin("Eric is Awesome", "ericeric");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("done");

  Serial.println("Starting WebInterface");
  interface->begin(NAME);
  Serial.println("WebInterface started");

  Serial.print("Starting LightNode...");
  node->begin();
  Serial.println("done");

  effectManager->selectEffect("Off");

  capTouch.begin();
}

unsigned long nextTime = 0;

void loop() {
  auto curTime = millis();
  
  interface->run();

  light->run();

  if(curTime >= nextTime) {
    nextTime = curTime + 10;
    if(button != nullptr) {
      button->run();
    }
  }
}
