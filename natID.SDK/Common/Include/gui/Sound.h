// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Sound.h
 * @brief Provides the Sound class for playing audio resources or files.
 */
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

/// @brief Holds raw audio buffer data for playback.
class NATGUI_API SoundBuffer
{
public:
    void* pFormat; ///< Pointer to the audio format descriptor.
    td::BYTE* pData; ///< Pointer to the raw audio data.
    td::UINT4 length; ///< Length of audio data in bytes.
    td::UINT4 bufferID; ///< Platform-specific buffer identifier.
    td::BYTE fromRes; ///< Non-zero if loaded from embedded resources.
    td::BYTE loop; ///< Non-zero if the sound should loop.
    td::BYTE played; ///< Non-zero if the sound has been played.

    /// @brief Default constructor. Initializes an empty sound buffer.
    SoundBuffer();

    /// @brief Destructor. Releases the audio buffer.
    ~SoundBuffer();
};

/// @brief Represents an audio sound that can be loaded from a file or resource and played.
class NATGUI_API Sound : public NatObject
{
public:
    /// @brief Enumerates built-in system sound types.
    enum class Type : unsigned char {Beep=0, SelectionChanged};
    /// @brief Enumerates possible playback states.
    enum class State : unsigned char {NotPlaying, Playing, ContinuousPlay, Paused};
private:
    friend class SoundHelper;
    friend class Resources;

    SoundBuffer _buffer; ///< The audio buffer holding the sound data.
    State _state = State::NotPlaying; ///< Current playback state.

    Sound() = delete;
    Sound(const Sound& sound);
    Sound& operator =(const Sound&) = delete;
public:
    /// @brief Constructs a Sound by loading from a file path or resource name (C string).
    /// @param fileOrResName File path or embedded resource name.
    Sound(const char* fileOrResName);

    /// @brief Constructs a Sound by loading from a file path or resource name (td::String).
    /// @param fileOrResName File path or embedded resource name.
    Sound(const td::String& fileOrResName);

    /// @brief Destructor. Stops and releases the sound.
    ~Sound();

    /// @brief Starts playback of this sound.
    /// @param loop Whether to loop the sound continuously.
    void play(bool loop = false);
    //void pause();
    //void cont();
    /// @brief Stops playback of this sound.
    void stop();

    /// @brief Enables or disables playing a sound on selection changes globally.
    /// @param bPlay True to enable, false to disable.
    static void setPlaySoundOnSelectionChange(bool bPlay);

    /// @brief Returns whether a sound plays on selection change.
    /// @return True if enabled.
    static bool shouldPlaySoundOnSelectionChange();

    /// @brief Plays a built-in system sound.
    /// @param soundType The type of system sound to play.
    static void play(Sound::Type soundType);

    /// @brief Returns the current playback state.
    /// @return Current Sound::State value.
    Sound::State getState() const;
};

} //namespace gui
