// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file UTFConverter.h
    @brief Provides static methods for measuring and converting between UTF-8, UTF-16, and UTF-32 encodings. */
#pragma once
#include <td/Types.h>
#include <mu/muLib.h>

namespace mu
{
    using UTF32 = td::UINT4;  ///< Alias for a 32-bit unsigned integer representing a UTF-32 code unit.
    using UTF16 = td::WORD;   ///< Alias for a 16-bit unsigned integer representing a UTF-16 code unit.
    using UTF8  = td::BYTE;   ///< Alias for an 8-bit unsigned integer representing a UTF-8 code unit.

/// @brief Provides static utility methods for Unicode encoding conversion and measurement.
class MAINUTILS_API UTFConverter
{
public:
    /// @brief Status codes returned by conversion operations.
    enum class Result : unsigned char {
        ConversionOK=0,    ///< Conversion completed successfully.
        SourceExhausted,   ///< Source buffer ended in the middle of a multi-unit character.
        TargetExhausted,   ///< Target buffer is too small to hold the converted output.
        SourceIllegal      ///< Source sequence contains illegal or malformed code units.
    };

    /// @brief Flags controlling strictness of the conversion.
    enum class Flags : unsigned char {
        StrictConversion = 0, ///< Reject illegal sequences and return SourceIllegal.
        NonStrictConversion   ///< Allow and pass through illegal sequences where possible.
    } ;

    /// @brief Measures the number of UTF-16 code units needed to represent the given UTF-32 source.
    /// @param source Pointer to the start of the UTF-32 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-32 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-16 code units required, or -1 on error.
    static int measureUTF32toUTF16 (const mu::UTF32* source, const mu::UTF32* sourceEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);

    /// @brief Converts a UTF-32 sequence to UTF-16.
    /// @param source Pointer to the start of the UTF-32 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-32 source sequence.
    /// @param target Pointer to the start of the UTF-16 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-16 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static Result convertUTF32toUTF16 (const mu::UTF32* source, const mu::UTF32* sourceEnd, mu::UTF16* target, mu::UTF16* targetEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);

    /// @brief Measures the number of UTF-32 code units needed to represent the given UTF-16 source.
    /// @param source Pointer to the start of the UTF-16 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-16 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-32 code units required, or -1 on error.
    static int measureUTF16toUTF32 (const mu::UTF16* source, const mu::UTF16* sourceEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);

    /// @brief Converts a UTF-16 sequence to UTF-32.
    /// @param source Pointer to the start of the UTF-16 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-16 source sequence.
    /// @param target Pointer to the start of the UTF-32 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-32 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static  Result convertUTF16toUTF32 (const mu::UTF16* source, const mu::UTF16* sourceEnd, mu::UTF32* target, mu::UTF32* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Measures the number of UTF-8 bytes needed to represent the given UTF-16 source.
    /// @param source Pointer to the start of the UTF-16 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-16 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-8 bytes required, or -1 on error.
    static int measureUTF16toUTF8 (const mu::UTF16* source, const mu::UTF16* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Converts a UTF-16 sequence to UTF-8.
    /// @param source Pointer to the start of the UTF-16 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-16 source sequence.
    /// @param target Pointer to the start of the UTF-8 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-8 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static Result convertUTF16toUTF8 (const mu::UTF16* source, const mu::UTF16* sourceEnd, mu::UTF8* target, mu::UTF8* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Checks whether the given UTF-8 byte sequence of the specified length is legally encoded.
    /// @param source Pointer to the start of the UTF-8 sequence.
    /// @param length Number of bytes to validate.
    /// @return true if the sequence is legal UTF-8; false otherwise.
    static bool isLegalUTF8(const mu::UTF8 *source, int length);

    /// @brief Measures the number of UTF-16 code units needed to represent the given UTF-8 source.
    /// @param source Pointer to the start of the UTF-8 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-8 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-16 code units required, or -1 on error.
    static int measureUTF8toUTF16 (const mu::UTF8* source, const mu::UTF8* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Converts a UTF-8 sequence to UTF-16.
    /// @param source Pointer to the start of the UTF-8 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-8 source sequence.
    /// @param target Pointer to the start of the UTF-16 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-16 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static Result convertUTF8toUTF16 (const mu::UTF8* source, const mu::UTF8* sourceEnd, mu::UTF16* target, mu::UTF16* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Measures the number of UTF-8 bytes needed to represent the given UTF-32 source.
    /// @param source Pointer to the start of the UTF-32 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-32 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-8 bytes required, or -1 on error.
    static int measureUTF32toUTF8 (const mu::UTF32* source, const mu::UTF32* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Converts a UTF-32 sequence to UTF-8.
    /// @param source Pointer to the start of the UTF-32 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-32 source sequence.
    /// @param target Pointer to the start of the UTF-8 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-8 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static Result convertUTF32toUTF8 (const mu::UTF32* source, const mu::UTF32* sourceEnd, mu::UTF8* target, mu::UTF8* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Measures the number of UTF-32 code units needed to represent the given UTF-8 source.
    /// @param source Pointer to the start of the UTF-8 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-8 source sequence.
    /// @param flags Conversion strictness flag.
    /// @return Number of UTF-32 code units required, or -1 on error.
    static int measureUTF8toUTF32 (const mu::UTF8* source, const mu::UTF8* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    /// @brief Converts a UTF-8 sequence to UTF-32.
    /// @param source Pointer to the start of the UTF-8 source sequence.
    /// @param sourceEnd Pointer one past the end of the UTF-8 source sequence.
    /// @param target Pointer to the start of the UTF-32 target buffer.
    /// @param targetEnd Pointer one past the end of the UTF-32 target buffer.
    /// @param flags Conversion strictness flag.
    /// @return Conversion result code.
    static Result convertUTF8toUTF32 (const mu::UTF8* source, const mu::UTF8* sourceEnd, mu::UTF32* target, mu::UTF32* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);
};

} //namespace mu
