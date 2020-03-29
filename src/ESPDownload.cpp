/**
 *
 * @file ESPDownload.cpp based om ESPDownload.cpp
 * @date 27.11.2019
 * @author Anderson Juncowski
 *
 * Copyright (c) 2019 Anderson Juncowski. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "ESPDownload.h"

ESPDownload::~ESPDownload(void)
{
}

ESPDownload::ESPDownload(void) : _timeout(8000)
{
}

ESPDownload::ESPDownload(int timeout): _timeout(timeout)
{
}

ESPDownload::ESPDownload(FS* fs, int timeout):_fs(fs),_timeout(timeout)
{
}



DownloadResult ESPDownload::getFile(const String& url, const String& path)
{
    if(_fs==nullptr){
        Serial.println("File sistem not defined!");
        return HTTP_DOWNLOAD_FAILED;
    }
    
    if(_fs->exists(path)){
        _fs->remove(path);
    }

    File file = _fs->open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Can not open file!");
        return HTTP_DOWNLOAD_FAILED;
    }
    getFile(url, file);
    file.close();
}


DownloadResult ESPDownload::getFile(const String& url, Stream& stream)
{
    HTTPClient http;
    if(!http.begin(url))
    {
        return HTTP_DOWNLOAD_FAILED;
    }

    return handleDownload(http, stream);
}

/**
 *
 * @param http HTTPClient *
 * @param outStream Stream *
 * @return ESPDownloadResult
 */
DownloadResult ESPDownload::handleDownload(HTTPClient& http, Stream& outStream)
{

    DownloadResult ret = HTTP_DOWNLOAD_FAILED;

    // use HTTP/1.0 for update since the update handler not support any transfer Encoding
    http.useHTTP10(true);
    http.setTimeout(_timeout);


    int code = http.GET();
    int len = http.getSize();

    if(code <= 0) {
        log_e("HTTP error: %s\n", http.errorToString(code).c_str());
        _lastError = code;
        http.end();
        return HTTP_DOWNLOAD_FAILED;
    }

    log_d(" - code: %d\n", code);
    log_d(" - len: %d\n", len);

   
    log_d("ESP32 info:\n");
    // TODO log_d(" - free Space: %d\n", ESP.getFreeSketchSpace());
    // TODO check space before download

    switch(code) {
    case HTTP_CODE_OK:
        if(len > 0) {

            uint8_t buff[128] = { 0 };

            // get tcp stream
            WiFiClient * stream = http.getStreamPtr();
            while(http.connected() && (len > 0 || len == -1)) {
            // get available data size
            size_t size = stream->available();

            if(size) {
                // read up to 128 byte
                size_t c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                // write it to stream
                outStream.write(buff, c);

                if(len > 0) {
                    len -= c;
                }
            }
            delay(1);
          }
        } else {
            _lastError = HTTP_UE_SERVER_NOT_REPORT_SIZE;
            ret = HTTP_DOWNLOAD_FAILED;
            log_e("Content-Length was 0 or wasn't set by Server?!\n");
        }
        break;
    case HTTP_CODE_NOT_FOUND:
        _lastError = HTTP_UE_SERVER_FILE_NOT_FOUND;
        ret = HTTP_DOWNLOAD_FAILED;
        break;
    case HTTP_CODE_FORBIDDEN:
        _lastError = HTTP_UE_SERVER_FORBIDDEN;
        ret = HTTP_DOWNLOAD_FAILED;
        break;
    default:
        _lastError = HTTP_UE_SERVER_WRONG_HTTP_CODE;
        ret = HTTP_DOWNLOAD_FAILED;
        log_e("HTTP Code is (%d)\n", code);
        break;
    }

    http.end();
    return ret;
}

void ESPDownload::clearDir(const char* path){
  if(_fs ==nullptr)return;
  File dir = _fs->open(path);
  if(!dir.isDirectory())return;

  File f = dir.openNextFile();
  Serial.println();
  while(f){
    String rmfile = f.name();
    f.close();
    _fs->remove(rmfile);
    f = dir.openNextFile();
  }
}

void ESPDownload::clearDir(const String& path){
  return clearDir(path.c_str());
}

//Check if file exists and is not null
boolean ESPDownload::fileExists(const char* path){
  if(_fs==nullptr)return false;
  if(_fs->exists(path)){
    File f = _fs->open(path);
    boolean isDir = f.isDirectory();
    size_t fSize = f.size();
    f.close();
    if(!isDir && fSize>0){
      return true;
    }
  }
  return false;
}

boolean ESPDownload::fileExists(const String& path){
  return fileExists(path.c_str());
}