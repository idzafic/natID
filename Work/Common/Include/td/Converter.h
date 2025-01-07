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
#include <td/String.h>
#include <mu/EnumSerializer.h>
#include <cstdlib>

namespace td
{

inline void UpdatePtr(void* pValue, DataType dataType, const char* strValue, int lenValue, const mu::EnumSerializer* enumSerializer= 0)
{
    switch (dataType)
    {
    case td::real4:
        {
            float* pVal = (float*) pValue;
            *pVal = td::toFloat<float>(strValue);
            break;
        }
    case td::real8:
        {
            double* pVal = (double*) pValue;
            *pVal = td::toFloat<double>(strValue);
            break;
        }
    case td::boolean:
        {
            bool* pVal = (bool*) pValue;
            *pVal = td::toBoolean(strValue);
            break;
        }
    case td::byte:
        {
            td::BYTE* pVal = (td::BYTE*) pValue;
            *pVal = td::toInt<td::BYTE>(strValue);
            break;
        }
    case td::word:
        {
            td::WORD* pVal = (td::WORD*) pValue;
            *pVal = td::toInt<td::WORD>(strValue);
            break;
        }
    case td::int2:
        {
            td::INT2* pVal = (td::INT2*) pValue;
            *pVal = td::toInt<td::INT2>(strValue);
            break;
        }
    case td::uint4:
        {
            td::UINT4* pVal = (td::UINT4*) pValue;
            *pVal = td::toInt<td::UINT4>(strValue);
            break;
        }
    case td::int4:
        {
            td::INT4* pVal = (td::INT4*) pValue;
            *pVal = td::toInt<td::INT4>(strValue);
            break;
        }
    case td::lint8:
        {
            td::LINT8* pVal = (td::LINT8*) pValue;
            *pVal = td::toInt<td::LINT8>(strValue);
            break;
        }
    case td::luint8:
        {
            td::LUINT8* pVal = (td::LUINT8*) pValue;
            *pVal = td::toInt<td::LUINT8>(strValue);
            break;
        }
    case td::string8:
        {
            td::String* pVal = (td::String*) pValue;
            pVal->setString(strValue, lenValue);
            break;
        }
    case td::enumerator:
        {
            assert(enumSerializer != 0);
            enumSerializer->getIntValue(td::ZStringUTF8(strValue));
        }

    default:
        assert(false);
    }
}

template<class TSTREAM>
inline void ShowValue(TSTREAM& ss, td::BYTE* pValue, DataType dataType, const mu::EnumSerializer* enumSerializer= 0)
{
    switch (dataType)
    {
    case td::real4:
        {
            float* pVal = (float*) pValue;
            ss << *pVal;
            break;
        }
    case td::real8:
        {
            double* pVal = (double*) pValue;
            ss << *pVal;
            break;
        }
    case td::boolean:
        {
            bool* pVal = (bool*) pValue;
            ss << td::c_str(*pVal);
            break;
        }
    case td::byte:
        {
            td::BYTE* pVal = (td::BYTE*) pValue;
            ss << *pVal;
            break;
        }
    case td::word:
        {
            td::WORD* pVal = (td::WORD*) pValue;
            ss << *pVal;
            break;
        }
    case td::int2:
        {
            td::INT2* pVal = (td::INT2*) pValue;
            ss << *pVal;
            break;
        }
    case td::uint4:
        {
            td::UINT4* pVal = (td::UINT4*) pValue;
            ss << *pVal;
            break;
        }
    case td::int4:
        {
            td::INT4* pVal = (td::INT4*) pValue;
            ss << *pVal;
            break;
        }
    case td::lint8:
        {
            td::LINT8* pVal = (td::LINT8*) pValue;
            ss << *pVal;
            break;
        }
    case td::luint8:
        {
            td::LUINT8* pVal = (td::LUINT8*) pValue;
            ss << *pVal;
            break;
        }
    case td::string8:
        {
            td::String* pVal = (td::String*) pValue;
            ss << pVal->c_str();
            break;
        }
    case td::enumerator:
        {
            assert(enumSerializer != 0);
            td::UINT4* pVal = (td::UINT4*) pValue;
            const td::String& str = enumSerializer->getName(*pVal);
            ss << str.c_str();
        }

    default:
        assert(false);
    }
}

} //namespace td
