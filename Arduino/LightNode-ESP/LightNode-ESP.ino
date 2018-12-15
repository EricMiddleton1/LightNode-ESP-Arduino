#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESPAsyncUDP.h>
#include <ArduinoJson.h>
#include <FS.h>

#include <memory>

extern "C" {
 #include "user_interface.h"
}

#include "ConfigFile.h"
#include "DebugPort.h"

#include "Light.h"
#include "LightAdapter.h"
#include "MatrixAdapter.h"

#include "ColorSet.h"

#include "EffectManager.h"
#include "SolidColorEffect.h"
#include "RemoteUpdateEffect.h"
#include "ColorFadeEffect.h"
#include "TwinkleEffect.h"
#include "StrobeEffect.h"
#include "RandomColorEffect.h"
#include "ColorWipeEffect.h"
#include "ColorWellEffect.h"

#include "WebInterface.h"
#include "Button.h"
#include "CapButton.h"

const uint16_t DEBUG_PORT = 1234;

std::shared_ptr<IColorPalette> christmasPalette = std::make_shared<ColorSet>(std::vector<Color>{ Color{255, 0, 0}, Color{0, 255, 0} }, true);
std::shared_ptr<IColorPalette> whitePalette = std::make_shared<ColorSet>(std::vector<Color>{ Color{255, 255, 255} }, false);

SolidColorEffect solidColorEffect;
RemoteUpdateEffect remoteUpdateEffect;
ColorFadeEffect colorFade;
TwinkleEffect twinkleEffect("Twinkle", 1, 0, christmasPalette);
TwinkleEffect christmasEffect("Christmas", 2, 4, christmasPalette);
TwinkleEffect winterEffect("Winter", 2, 4, whitePalette);
StrobeEffect strobeEffect;
RandomColorEffect randomColorEffect;
ColorWipeEffect colorWipeEffect;
ColorWellEffect christmasSwoopEffect("Christmas Swoop", christmasPalette);
ColorWellEffect winterSwoopEffect("Winter Swoop", whitePalette);

Light* light;
EffectManager* effectManager;
WebInterface* interface;
Button* button = nullptr;

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();

  Serial.println("[Info] Loading config file...");
  ConfigFile config;
  Serial.println("[Info] Done loading config file");

  auto& globalConfig = config.get();
  auto& lightConfig = config.get("light");
  String name = globalConfig["name"];
  String hostname = globalConfig["hostname"];

  Serial.printf("[Info] Light name=%s, hostname=%s\n", name.c_str(),
    hostname.c_str());

  wifi_station_set_hostname(const_cast<char*>(hostname.c_str()));
  
  DebugPort.begin(DEBUG_PORT);
  ArduinoOTA.setHostname(name.c_str());

  light = new Light(name);
  light->setDriver(LightDriver::Build(lightConfig));
  light->setAdapter(std::unique_ptr<LightAdapter>(new LightAdapter(light, nullptr)));

  effectManager = new EffectManager{*light->getAdapter()};
  effectManager->addEffect(solidColorEffect);
  effectManager->addEffect(remoteUpdateEffect);
  effectManager->addEffect(randomColorEffect);
  effectManager->addEffect(colorFade);
  effectManager->addEffect(colorWipeEffect);
  effectManager->addEffect(twinkleEffect);
  effectManager->addEffect(christmasEffect);
  effectManager->addEffect(winterEffect);
  effectManager->addEffect(christmasSwoopEffect);
  effectManager->addEffect(winterSwoopEffect);
  //effectManager->addEffect(strobeEffect);

  Light* lights[] = {light};

  //button = new CapButton(*effectManager, "Single Color", 2, 1, {50, 10, 200, 5, 50});
  interface = new WebInterface(*effectManager, *light);

/*
  WiFi.mode(WIFI_AP);
  WiFi.softAP(NAME, "diegolights");
*/
  
  Serial.print("\nConnecting to AP");

  WiFi.mode(WIFI_STA);
  WiFi.begin(AP, PSK);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.print("done: ");
  Serial.println(WiFi.localIP());
  

  Serial.println("Starting WebInterface");
  interface->begin(hostname);
  Serial.println("WebInterface started");

  Serial.print("Starting LightNode...");
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
  
  //interface->run();
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
