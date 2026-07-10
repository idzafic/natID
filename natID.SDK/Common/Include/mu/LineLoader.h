// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LineLoader.h
    @brief Provides line-by-line reading from files, strings, or custom text sources. */
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

/// @brief Interface for custom line content providers used by LineLoader.
class ILineLoader
{
public:
    /// @brief Retrieves the content of a line starting at the given byte position.
    /// @param strOut Output string that receives the line content.
    /// @param lineHeadPos Byte offset of the line's beginning in the source.
    /// @return The byte position immediately after the retrieved line.
    virtual td::UINT4 getLineContentAtPosition(td::MutableString& strOut, td::UINT4 lineHeadPos) const = 0;
};

/// @brief Loads individual lines from a file, an in-memory string, or a custom ILineLoader source.
class MAINUTILS_API LineLoader
{
public:
    /// @brief Identifies the type of source from which lines are loaded.
    enum class Source : td::BYTE {File=0, ///< Source is an open std::ifstream.
                                   String,  ///< Source is an in-memory td::String.
                                   TextEdit ///< Source is a custom ILineLoader implementation.
                                  };
private:
    union
    {
        std::ifstream* _pFile = nullptr; ///< Pointer to the file stream when source is File.
        const td::String* _pStr; ///< Pointer to the string when source is String.
        ILineLoader* _pILL; ///< Pointer to the custom loader when source is TextEdit.
    };

    td::UINT4 _prevPosition; ///< Byte position before the last getLine call.
    td::UINT4 _currPosition; ///< Current byte position within the source.
    td::UINT4 _offset = 0; ///< Additional byte offset applied when reading.
    Source _source; ///< The active source type.

    LineLoader() = delete;
public:
    /// @brief Constructs a LineLoader at the given initial position with the specified source type.
    /// @param currPosition Initial byte position within the source.
    /// @param source The type of source to read from.
    LineLoader(td::UINT4 currPosition, Source source);

    /// @brief Sets the line source to an open file stream.
    /// @param pFile Pointer to an already-open std::ifstream.
    void setSource(std::ifstream* pFile);

    /// @brief Sets the line source to an in-memory string.
    /// @param pStr Pointer to the string that contains the text.
    void setSource(const td::String* pStr);

    /// @brief Sets the line source to a custom ILineLoader implementation.
    /// @param pILL Pointer to the custom line loader.
    void setSource(ILineLoader* pILL);

    /// @brief Updates the current reading position within the source.
    /// @param newPosition New byte position to seek to.
    void setPosition(td::UINT4 newPosition);

    /// @brief Reads the next line from the current source into the output string.
    /// @param mStr Mutable string that receives the line content.
    /// @return true if a line was successfully read, false at end of source.
    bool getLine(td::MutableString& mStr);

    /// @brief Sets an additional byte offset applied when reading lines.
    /// @param offset Byte offset value to add to position calculations.
    void setOffset(td::UINT4 offset);

    /// @brief Returns the byte position before the most recent getLine call.
    /// @return Byte position of the previous line's start.
    td::UINT4 getPreviousPosition() const;

    /// @brief Returns the current byte position within the source.
    /// @return Current byte position after the last read operation.
    td::UINT4 getCurrentPosition() const;
};

} //namespace mu
