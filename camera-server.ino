#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <BluetoothSerial.h>
#include <ESPAsyncWebSrv.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <Wire.h>

BluetoothSerial SerialBT;
const char* WIFI_SSID = "Tufts_Robot";
const char* WIFI_PASS = "";
 
AsyncWebServer server(80);
static auto loRes = esp32cam::Resolution::find(320, 240);

void serveJpg(){
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));
 
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}
 
void handleJpgLo(){
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
 
 
void  setup(){
  Serial.begin(115200);
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(loRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "OK" : "FAIL");
  }
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
 
  server.on("/cam-lo.jpg", handleJpgLo);
  server.begin();
}
 
void loop() {
  server.handleClient();
}
