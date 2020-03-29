# ESPDownload (ESP32 / ES8266)

An ESP32 / ESP8266 library to download any file from a url and asve to a File System,
or output file to a Stream (Serial, SDCard, File...);

Obs. Not tested on esp8266


```c++
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

```
#Methods
```c++

  // Download and save file to specified path
  DownloadResult getFile(const String& url, const String& path);
  // Download and file and send to a Stream (Serial, SDCard file, ...)
  DownloadResult getFile(const String& url, Stream& stream);

  // Remove all files in a directory, Util to clearing your downloads folder
  void clearDir(const char* path);
  void clearDir(const String& path);

  //To check if file exists before download
  boolean fileExists(const char* path);
  boolean fileExists(const String& path);
```
