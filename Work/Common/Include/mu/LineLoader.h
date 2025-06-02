// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/MutableString.h>
#include <fstream>
#include <mu/muLib.h>

namespace mu
{

class ILineLoader
{
public:
    virtual td::UINT4 getLineContentAtPosition(td::MutableString& strOut, td::UINT4 lineHeadPos) const = 0;
};

class MAINUTILS_API LineLoader
{
public:
    enum class Source : td::BYTE {File=0, String, TextEdit};
private:
    union
    {
        std::ifstream* _pFile = nullptr;
        const td::String* _pStr;
        ILineLoader* _pILL;
    };

    td::UINT4 _prevPosition;
    td::UINT4 _currPosition;
    td::UINT4 _offset = 0;
    Source _source;
    
    LineLoader() = delete;
public:
    LineLoader(td::UINT4 currPosition, Source source);
    void setSource(std::ifstream* pFile);
    void setSource(const td::String* pStr);
    void setSource(ILineLoader* pILL);
    void setPosition(td::UINT4 newPosition);
    bool getLine(td::MutableString& mStr);
    void setOffset(td::UINT4 offset);
    td::UINT4 getPreviousPosition() const;
    td::UINT4 getCurrentPosition() const;
};

} //namespace mu
