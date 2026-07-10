// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Range.h
    @brief Generic range type defined by a start location and a length. */
#pragma once
#include <td/Types.h>

namespace td
{

/// @brief Represents a contiguous range defined by a start position and an extent.
/// @tparam T Numeric type used for both the location and length (e.g. int, float).
template <typename T>
class Range
{
public:
    T location; ///< Start position of the range.
    T length;   ///< Extent (length) of the range from location.

    /// @brief Default constructor. Initialises location and length to zero.
    Range()
    : location(0)
    , length(0)
    {}

    /// @brief Copy constructor.
    /// @param range The range to copy.
    Range(const Range<T>& range)
    : location(range.location)
    , length(range.length)
    {}

    /// @brief Constructs a range from explicit location and length values.
    /// @param locIn    The start position.
    /// @param lengthIn The extent of the range.
    Range(T locIn, T lengthIn)
    : location(locIn)
    , length(lengthIn)
    {}

    /// @brief Copy assignment operator.
    /// @param range The range to assign from.
    void operator = (const Range& range)
    {
        location = range.location;
        length = range.length;
    };

    /// @brief Formats the range as a string into the given buffer.
    /// @param buff    Destination character buffer.
    /// @param buffLen Size of the buffer in bytes.
    void c_str(char* buff, int buffLen) const
    {
        if (SNPRINTF(buff, buffLen, _TRUNCATE, "location=\"%g\" length=\"%g\"", location, length) < 0)
            buff[buffLen - 1] = 0;
    }

    /// @brief Writes a human-readable representation to an output stream.
    /// @tparam TOSTREAM Output stream type.
    /// @param o The destination stream.
    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "(" << location << "," << length << ")";
    }

    /// @brief Deserialises the range from a binary archive.
    /// @tparam Archive Archive type providing operator>>.
    /// @param ar The source archive.
    /// @return True on success.
    template<class Archive>
    bool loadFromBinaryStream(Archive& ar)
    {
        ar >> location >> length;
        return true;
    }

    /// @brief Serialises the range to a binary archive.
    /// @tparam Archive Archive type providing operator<<.
    /// @param ar The destination archive.
    template<class Archive>
    void saveToBinaryStream(Archive& ar) const
    {
        ar << location << length;
    }

    /// @brief Shifts the location left by delta (decrements location by delta).
    /// @param delta The amount to subtract from location.
    void operator -= (T delta)
    {
        location -= delta;
    }

    /// @brief Shifts the location right by delta (increments location by delta).
    /// @param delta The amount to add to location.
    void operator += (T delta)
    {
        location += delta;
    }

    /// @brief Translates the range by adding delta to location.
    /// @param delta The translation amount.
    void translate(T delta)
    {
        location += delta;
    }

    /// @brief Scales both location and length by delta.
    /// @param delta The scaling factor.
    void scale(T delta)
    {
        location *= delta;
    }
};

} //namespace td
