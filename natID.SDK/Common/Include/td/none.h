// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file none.h
    @brief Defines the td::none sentinel type representing an absent or untyped value. */
#pragma once
#include <td/Types.h>

namespace td
{

/// @brief Sentinel class used to represent an absent, null, or untyped variant value.
class none
{
    const td::LUINT8 val; ///< Internal placeholder value; always zero.
public:
    /// @brief Default constructor; initialises the internal value to zero.
    none(): val(0) {}

    /// @brief No-op assignment; none is immutable.
    /// @return Reference to this none instance.
    const none& operator = (const none&) const
    {
        return *this;
    }

    /// @brief Equality comparison; none never equals none.
    /// @return Always false.
    bool operator == (const none&) const
    {

        return false;
    }

    /// @brief Inequality comparison; none never differs from none.
    /// @return Always false.
    bool operator != (const none& ) const
    {
        return false;
    }

    /// @brief Less-than comparison; always returns false for none.
    /// @return Always false.
    bool operator < (const none&) const
    {
        return false;
    }

    /// @brief Less-than-or-equal comparison; always returns false for none.
    /// @return Always false.
    bool operator <= (const none&) const
    {
        return false;
    }

    /// @brief Greater-than comparison; always returns false for none.
    /// @return Always false.
    bool operator > (const none&) const
    {
        return false;
    }

    /// @brief Greater-than-or-equal comparison; always returns false for none.
    /// @return Always false.
    bool operator >= (const none&) const
    {
        return false;
    }

    /// @brief Outputs the literal string "TD_NONE" to a stream.
    /// @tparam CharT Character type of the stream.
    /// @tparam Traits Traits type of the stream.
    /// @param os The output stream.
    /// @param rhs The none instance to stream.
    /// @return The output stream.
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
        operator<<(std::basic_ostream<CharT, Traits>& os, const none& rhs)
    {
        os << "TD_NONE";
        return os;
    }
};
}
