/**
 *
 * @file ESPDownload.h based on ESPDownload.cpp
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

#ifndef ___ESP_DOWNLOAD_H___
#define ___ESP_DOWNLOAD_H___

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Stream.h>
#include <FS.h>

#define HTTP_UE_TOO_LESS_SPACE              (-100)
#define HTTP_UE_SERVER_NOT_REPORT_SIZE      (-101)
#define HTTP_UE_SERVER_FILE_NOT_FOUND       (-102)
#define HTTP_UE_SERVER_FORBIDDEN            (-103)
#define HTTP_UE_SERVER_WRONG_HTTP_CODE      (-104)

enum DownloadResult {
    HTTP_DOWNLOAD_FAILED,
    HTTP_DOWNLOAD_OK
};

class ESPDownload
{
public:
    ESPDownload(void);
    ESPDownload(int timeout);
    ESPDownload(FS* fs, int timeout = 8000);
    ~ESPDownload(void);

    DownloadResult getFile(const String& url, Stream& stream);
    DownloadResult getFile(const String& url, const String& path);

    void clearDir(const char* path);
    void clearDir(const String& path);
    boolean fileExists(const char* path);
    boolean fileExists(const String& path);

protected:
    DownloadResult handleDownload(HTTPClient& http, Stream& stream);
    FS* _fs=nullptr;
    int _lastError;
private:
    int _timeout;
};

#endif