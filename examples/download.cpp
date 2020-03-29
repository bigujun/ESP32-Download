#include <Arduino.h>
#include "ESPDownload.h"
#include <FS.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const char* filePath = "/download/README.md";
const char* fileUrl = "https://raw.githubusercontent.com/bigujun/ESP32-Download/master/README.md";

ESPDownload downloader = ESPDownload();


void setup(){
  SPIFFS.begin(true);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  
  downloader.getFile(fileUrl, file);

}

void loop(){

}


