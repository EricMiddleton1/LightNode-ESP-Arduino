#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESPAsyncUDP.h>
#include <ArduinoJson.h>

#include <memory>

#include "DebugPort.h"

#include "Light.h"
#include "WhiteDriver.h"
#include "AnalogDriver.h"
#include "NeoPixelDriver.h"
#include "APA102Driver.h"
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

char* NAME = "TV";
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

void setup() {
  wifi_station_set_hostname(NAME);
  
  Serial.begin(115200);
  DebugPort.begin(DEBUG_PORT);
  ArduinoOTA.setHostname(NAME);

  light = new Light{NAME};
  //light->setDriver(std::unique_ptr<Driver>(new AnalogDriver(14, 12, 13)));
  //light->setDriver(std::unique_ptr<Driver>(new NeoPixelDriver(50, NeoPixelDriver::ColorOrder::RGB)));
  light->setDriver(std::unique_ptr<Driver>(new WhiteDriver(3)));
  //light->setDriver(std::unique_ptr<Driver>(new APA102Driver(130)));
  light->setAdapter(std::unique_ptr<LightAdapter>(new LightAdapter(nullptr)));
  //light->setAdapter(std::unique_ptr<LightAdapter>(new MatrixAdapter(nullptr, 10, 13, {PixelMapper::Stride::Rows, PixelMapper::StrideOrder::Progressive, PixelMapper::Start::BottomRight})));

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

  WiFi.mode(WIFI_STA);
  WiFi.begin("108net", "3ricn3t1");
  //WiFi.begin("Eric is Awesome", "ericeric");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.print("done: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting WebInterface");
  interface->begin(NAME);
  Serial.println("WebInterface started");

  Serial.print("Starting LightNode...");
  node->begin();
  Serial.println("done");

  effectManager->selectEffect("Off");
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

unsigned long nextTime = 0;

void loop() {
  auto curTime = millis();
  
  interface->run();
  light->run();
  ArduinoOTA.handle();

  if(curTime >= nextTime) {
    nextTime = curTime + 1000;
    Serial.print("\tFree: ");
    Serial.println(ESP.getFreeHeap());
    if(button != nullptr) {
      button->run();
    }
  }
}
