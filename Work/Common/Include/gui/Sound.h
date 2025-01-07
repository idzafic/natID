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
#include <gui/Types.h>
#include <gui/NatObject.h>

namespace gui
{

class SoundHelper;
class Resources;

class NATGUI_API SoundBuffer
{    
public:
    void* pFormat;
    td::BYTE* pData;
    td::UINT4 length;
    td::UINT4 bufferID;
    td::BYTE fromRes;
    td::BYTE loop;
    td::BYTE played;

    SoundBuffer();
    ~SoundBuffer();
};

class NATGUI_API Sound : public NatObject
{
public:
    enum class Type : unsigned char {Beep=0, SelectionChanged};
private:
    friend class SoundHelper;
    friend class Resources;

    SoundBuffer _buffer;

    Sound();
    Sound(const Sound& sound);
public:    
    Sound(const char* fileOrResName);
    Sound(const td::String& fileOrResName);
    ~Sound();
    void play(bool loop = false);
    //void pause();
    //void cont();
    void stop();
    
    static void setPlaySoundOnSelectionChange(bool bPlay);
    static bool shouldPlaySoundOnSelectionChange();
    static void play(Sound::Type soundType);
};

} //namespace gui
