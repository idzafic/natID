// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <mu/muLib.h>

namespace mu
{

    typedef td::UINT4	UTF32;	// unsigned 32 bits 
    typedef td::WORD	UTF16;	// unsigned 16 bits 
    typedef td::BYTE	UTF8;	// unsigned 8 bits

class MAINUTILS_API UTFConverter
{
public:
    enum class Result : unsigned char {
        ConversionOK=0, 		// conversion successful
        SourceExhausted,	// partial character in source, but hit end
        TargetExhausted,	// insuff. room in target for conversion
        SourceIllegal		// source sequence is illegal/malformed
    };

    enum class Flags : unsigned char {
        StrictConversion = 0,
        NonStrictConversion
    } ;

    static int measureUTF32toUTF16 (const mu::UTF32* source, const mu::UTF32* sourceEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);
    
    static Result convertUTF32toUTF16 (const mu::UTF32* source, const mu::UTF32* sourceEnd, mu::UTF16* target, mu::UTF16* targetEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);

    static int measureUTF16toUTF32 (const mu::UTF16* source, const mu::UTF16* sourceEnd, UTFConverter::Flags flags = UTFConverter::Flags::StrictConversion);
    
    static  Result convertUTF16toUTF32 (const mu::UTF16* source, const mu::UTF16* sourceEnd, mu::UTF32* target, mu::UTF32* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    static int measureUTF16toUTF8 (const mu::UTF16* source, const mu::UTF16* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);

    static Result convertUTF16toUTF8 (const mu::UTF16* source, const mu::UTF16* sourceEnd, mu::UTF8* target, mu::UTF8* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static bool isLegalUTF8(const mu::UTF8 *source, int length);
    
    static int measureUTF8toUTF16 (const mu::UTF8* source, const mu::UTF8* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static Result convertUTF8toUTF16 (const mu::UTF8* source, const mu::UTF8* sourceEnd, mu::UTF16* target, mu::UTF16* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static int measureUTF32toUTF8 (const mu::UTF32* source, const mu::UTF32* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static Result convertUTF32toUTF8 (const mu::UTF32* source, const mu::UTF32* sourceEnd, mu::UTF8* target, mu::UTF8* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static int measureUTF8toUTF32 (const mu::UTF8* source, const mu::UTF8* sourceEnd, UTFConverter::Flags flags = Flags::StrictConversion);
    
    static Result convertUTF8toUTF32 (const mu::UTF8* source, const mu::UTF8* sourceEnd, mu::UTF32* target, mu::UTF32* targetEnd, UTFConverter::Flags flags = Flags::StrictConversion);
};

} //namespace mu
