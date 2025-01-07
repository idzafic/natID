//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <no/HTTP.h>
#include <no/URL.h>
#include <cnt/StringBuilder.h>
#include "RAMDiskFile.h"

inline bool testGet(const char* word, td::String& strRet)
{
    td::String strReq;
    no::HTTP::Cmd httpCmd = no::HTTP::Cmd::Get;
    td::String serverName;
    td::String payLoad;

    strReq.format("https://www.e-rjecnik.net/dictionary/english-croatian/%s", word);

    td::UINT4 retCode;
    if (no::HTTP::sendSecured(httpCmd, strReq, payLoad, strRet, retCode))
    {
        return true;
    }
    return false;
}

inline bool testPost(const char* word, cnt::SafeFullVector <no::HTTP::Attrib>* phttpHeaderAttribs, td::String& strRet)
{
    td::String strReq;
    no::HTTP::Cmd httpCmd = no::HTTP::Cmd::Post;
    td::String serverName;
    td::String payLoad;
    
    strReq = "http://www.englesko.hrvatski-rjecnik.com/";
    httpCmd = no::HTTP::Cmd::Post;
    cnt::StringBuilderSmall strBuilder;
    td::String httpWord = no::encodeURL(word, strBuilder, true, true);

    payLoad.format("rijec=%s&textfield=&jezik=S+ENGLESKOG+NA+HRVATSKI&related=checkbox&smjer=en_to_hr", httpWord.c_str());

    td::UINT4 retCode;
    if (no::HTTP::sendSecured(httpCmd, strReq, payLoad, strRet, retCode, phttpHeaderAttribs))
    {
        return true;
    }
    return false;
}

inline void initHttpAttribs(cnt::SafeFullVector<no::HTTP::Attrib>& httpHeaderAttribs, bool full= false)
{
    if (full)
        httpHeaderAttribs.reserve(6);
    else
        httpHeaderAttribs.reserve(1);
    httpHeaderAttribs[0].name = "Content-Type";
    httpHeaderAttribs[0].value = "application/x-www-form-urlencoded";
    if (!full)
        return;

    httpHeaderAttribs[0].name = "Accept";
    httpHeaderAttribs[0].value = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8";

    httpHeaderAttribs[1].name = "Accept-Language";
    httpHeaderAttribs[1].value = "en-US,en;q=0.9,hr;q=0.8,bs;q=0.7,de;q=0.6,sr;q=0.5";

    httpHeaderAttribs[2].name = "Cache-Control";
    httpHeaderAttribs[2].value = "max-age=0";

    httpHeaderAttribs[3].name = "Accept-Charset";
    httpHeaderAttribs[3].value = "windows-1250";
    //httpHeaderAttribs[3].value = "utf-8";

    //g_httpAttribEngHr[3].name = "Connection";
    //g_httpAttribEngHr[3].value = "close";

    httpHeaderAttribs[4].name = "Content-Type";
    httpHeaderAttribs[4].value = "application/x-www-form-urlencoded";

    httpHeaderAttribs[5].name = "Host";
    httpHeaderAttribs[5].value = "www.englesko.hrvatski-rjecnik.com";

//    httpHeaderAttribs[6].name = "User-Agent";
//    httpHeaderAttribs[6].value = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";
}


inline void testTTPGetAndPos()
{    
    td::String strRet;
    
    fo::OutFile f1;
    td::String strFn = getFileNameOnRamDisk("TestGET.html");
    f1.open(strFn.c_str());
    
    if (testGet("home", strRet))
    {
        std::cout << "GET OK!" << td::endl;
    }
    else
    {
        std::cout << "GET NotOK!" << td::endl;
    }
    
    f1 << strRet << td::endl;
    f1.close();
    
    cnt::SafeFullVector<no::HTTP::Attrib> httpAttribs;
    initHttpAttribs(httpAttribs, true);
    
    if (testPost("home", &httpAttribs, strRet))
    {
        std::cout << "POST OK!" << td::endl;
    }
    else
    {
        std::cout << "POST NotOK!" << td::endl;
    }
    
    strFn = getFileNameOnRamDisk("TestPOST.html");
    f1.open(strFn.c_str());
    f1 << strRet << td::endl;
    f1.close();
}

inline void testMapDownloaderStr()
{
    td::String strRet;
    
    fo::OutFile f1;
    td::String strFn = getFileNameOnRamDisk("Sarajevo.xml");
    f1.open(strFn.c_str());
    
    //td::String strReq = "https://api.openstreetmap.org/api/0.6/map?bbox=18.356,43.824,18.496,43.894"; //cijelo sarajevo
    td::String strReq = "https://api.openstreetmap.org/api/0.6/map?bbox=18.426,43.854,18.446,43.864";
    no::HTTP::Cmd httpCmd = no::HTTP::Cmd::Get;
    td::String serverName;
    td::String payLoad;

    td::UINT4 retCode;
    if (no::HTTP::sendSecured(httpCmd, strReq, payLoad, strRet, retCode))
    {
        std::cout << "Map downloading OK!" << td::endl;
    }
    else
    {
        std::cout << "ERROR! Map downloading failed" << td::endl;
    }
    
    f1 << strRet << td::endl;
    f1.close();
    
}


//transfers returned data from the communication interface to a file
class MapConsumer : public no::HTTP::IConsumer
{
    fo::OutFile _f;
    bool consume(const char* buff, size_t len) override
    {
        if (!_f.is_open())
            return false;
        _f.write(buff, len);  //put data to file
        return _f.good();
    }
public:
    MapConsumer(const td::String& fileName)
    {
        _f.open(fileName.c_str());
    }
};

inline void testMapDownloaderConsumer()
{
    td::String strRet;
    
    td::String strFn = getFileNameOnRamDisk("Sarajevo_viaConsumer.xml");
    MapConsumer mapConsumer(strFn);
    
    //td::String strReq = "https://api.openstreetmap.org/api/0.6/map?bbox=18.356,43.824,18.496,43.894"; //cijelo sarajevo
    td::String strReq = "https://api.openstreetmap.org/api/0.6/map?bbox=18.426,43.854,18.446,43.864";
    no::HTTP::Cmd httpCmd = no::HTTP::Cmd::Get;
    td::String serverName;
    td::String payLoad;

    td::UINT4 retCode;
    if (no::HTTP::sendSecured(httpCmd, strReq, payLoad, &mapConsumer, retCode))
    {
        std::cout << "Map downloading with consumer is OK!" << td::endl;
    }
    else
    {
        std::cout << "ERROR! Map downloading withh consumer failed" << td::endl;
    }
}

