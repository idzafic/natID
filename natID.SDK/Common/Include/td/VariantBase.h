// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VariantBase.h
    @brief Base storage union for all Variant types, providing raw value storage and the visitor dispatch mechanism. */
#pragma once
#include <td/Types.h>
#include <td/DateTime.h>
#include <td/String.h>
#include <td/Decimal.h>
#include <td/BoolCh.h>
#include <td/SmallChFix.h>
#include <td/Color.h>
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/DotPattern.h>
#include <td/Anchor.h>

namespace db
{
	template <int NPTRS, int CHUNK_IN_BYTES>
	class Recordset;
}

namespace dp
{
    template <int NPTRS, int CHUNK_IN_BYTES>
    class DataSet;
}

namespace td
{

/// @brief Sentinel type used as the "empty" case in visitor dispatch.
class none
{
public:
	/// @brief Default constructor.
	none(){};

};

/// @brief Low-level storage base for all td::Variant values.
///
/// Holds a discriminated union of all supported value types and
/// provides templated visitor-dispatch helpers used by the Variant
/// class hierarchy.
class VariantBase
{
	template <typename TVAR>
	friend struct BaseToVariantVisitor;

	template <int NPTRS, int CHUNK_IN_BYTES>
	friend class db::Recordset;

    template <int NPTRS, int CHUNK_IN_BYTES>
    friend class dp::DataSet;

protected:
	union
	{
		//bool _boolVal;
		char _chVal[8];      ///< Raw 8-byte character array (used by ChFix7 and similar types).
		//struct Bool	_bVal;
		char _bVal;          ///< Boolean value stored as 'Y' or 'N'.
		BYTE _byteVal;       ///< Unsigned 8-bit integer value.
        td::ColorID _colorID;         ///< Color identifier value.
        td::LinePattern _linePattern; ///< Line pattern value.
        td::DotPattern _dotPattern;   ///< Dot pattern value.
        td::Anchor _anchor;           ///< Anchor position value.
		WORD _wordVal;       ///< Unsigned 16-bit integer value.
		INT2 _i2Val;         ///< Signed 16-bit integer value.
		UINT4 _u4Val;        ///< Unsigned 32-bit integer value.
		INT4 _i4Val;         ///< Signed 32-bit integer value.
		LUINT8 _lu8Val;      ///< Unsigned 64-bit integer value.
		LINT8 _li8Val;       ///< Signed 64-bit integer value.
		float _r4Val;        ///< Single-precision floating-point value.
		double _r8Val;       ///< Double-precision floating-point value.
		INT4 _dateVal;       ///< Date value encoded as a signed 32-bit integer.
        UINT4 _colorVal;     ///< Color value packed as unsigned 32-bit integer.
		INT4 _timeVal;       ///< Time value encoded as a signed 32-bit integer.
		LINT8 _dtVal;        ///< DateTime value encoded as a signed 64-bit integer.
		LINT8 _decVal;       ///< Fixed-precision Decimal (LINT8-backed) value.
		INT4 _smallDecVal;   ///< Small Decimal (INT4-backed) value.

		char* _strVal;	     ///< Pointer to the data holder of a td::String.
		bool* _ptrBool;      ///< Pointer to a boolean value.
		BYTE* _ptrByte;      ///< Pointer to an unsigned 8-bit integer value.
		WORD* _ptrWord;      ///< Pointer to an unsigned 16-bit integer value.
		INT2* _ptrI2;        ///< Pointer to a signed 16-bit integer value.
		UINT4* _ptrU4;       ///< Pointer to an unsigned 32-bit integer value.
		INT4* _ptrI4;        ///< Pointer to a signed 32-bit integer value.
		LUINT8* _ptrLU8;     ///< Pointer to an unsigned 64-bit integer value.
		LINT8* _ptrLI8;      ///< Pointer to a signed 64-bit integer value.
		float* _ptrR4;       ///< Pointer to a float value.
		double* _ptrR8;      ///< Pointer to a double value.
		Date* _ptrDate;      ///< Pointer to a Date value.
		Time* _ptrTime;      ///< Pointer to a Time value.
		DateTime* _ptrDT;    ///< Pointer to a DateTime value.
		Decimal0* _ptrDec0;  ///< Pointer to a Decimal0 value.
		Decimal1* _ptrDec1;  ///< Pointer to a Decimal1 value.
		Decimal2* _ptrDec2;  ///< Pointer to a Decimal2 value.
		Decimal3* _ptrDec3;  ///< Pointer to a Decimal3 value.
		Decimal4* _ptrDec4;  ///< Pointer to a Decimal4 value.
		SmallDecimal0* _ptrSDec0; ///< Pointer to a SmallDecimal0 value.
		SmallDecimal1* _ptrSDec1; ///< Pointer to a SmallDecimal1 value.
		SmallDecimal2* _ptrSDec2; ///< Pointer to a SmallDecimal2 value.
		SmallDecimal3* _ptrSDec3; ///< Pointer to a SmallDecimal3 value.
		SmallDecimal4* _ptrSDec4; ///< Pointer to a SmallDecimal4 value.
		Variant* _ptrVar;    ///< Pointer to a nested Variant value.
		void* _voidPtr;      ///< Generic void pointer value.
	};

protected:
	/// @brief Access the internal string value as a td::String reference.
	/// @return Reference to the td::String stored in _strVal.
	inline td::String& strValBase()
	{
		return reinterpret_cast<td::String&>(_strVal);
	}

public:
	/// @brief Default constructor; initializes all storage to zero.
	VariantBase()
		: _lu8Val(0)
	{}

	/// @brief Construct with a boolean value.
	/// @param val Boolean value to store ('Y' for true, 'N' for false).
	VariantBase(bool val)
		: _lu8Val(0)
	{
		if (val)
			_bVal = 'Y';
		else
			_bVal = 'N';
	}

	/// @brief Construct with a void pointer value.
	/// @param pPtr Void pointer to store.
    VariantBase(void* pPtr)
    {
        _voidPtr = pPtr;
    }

	/// @brief Construct with an unsigned 8-bit integer value.
	/// @param val BYTE value to store.
	VariantBase(td::BYTE val)
		: _byteVal(val)
	{}

	/// @brief Construct with an unsigned 16-bit integer value.
	/// @param val WORD value to store.
	VariantBase(td::WORD val)
		: _wordVal(val)
	{}

	/// @brief Construct with a signed 16-bit integer value.
	/// @param val INT2 value to store.
	VariantBase(td::INT2 val)
		: _i2Val(val)
	{}

	/// @brief Construct with a signed 32-bit integer value.
	/// @param val INT4 value to store.
	VariantBase(td::INT4 val)
		: _i4Val(val)
	{}

	/// @brief Construct with an unsigned 32-bit integer value.
	/// @param val UINT4 value to store.
	VariantBase(td::UINT4 val)
		: _u4Val(val)
	{}

	/// @brief Construct with an unsigned 64-bit integer value.
	/// @param val LUINT8 value to store.
	VariantBase(td::LUINT8 val)
		: _lu8Val(val)
	{}

	/// @brief Construct with a signed 64-bit integer value.
	/// @param val LINT8 value to store.
	VariantBase(td::LINT8 val)
		: _li8Val(val)
	{}

	/// @brief Construct with a single-precision float value.
	/// @param val Float value to store.
	VariantBase(float val)
		: _r4Val(val)
	{}

	/// @brief Construct with a double-precision float value.
	/// @param val Double value to store.
	VariantBase(double val)
		: _r8Val(val)
	{}

	/// @brief Construct with a Date value.
	/// @param val Date value to store.
	VariantBase(td::Date val)
		: _dateVal(val.getValue())
	{}

	/// @brief Construct with a Time value.
	/// @param val Time value to store.
	VariantBase(td::Time val)
		: _timeVal(val.getValue())
	{}

	/// @brief Construct with a DateTime value.
	/// @param val DateTime value to store.
	VariantBase(const td::DateTime& val)
		: _dtVal(val.getValue())
	{}

	/// @brief Construct with a td::String value.
	/// @param val String value to store.
	VariantBase(const td::String& val)
		: _lu8Val(0)
	{
		strValBase() = val;
	}

	/// @brief Construct with a Color value.
	/// @param val Color value to store.
    VariantBase(const td::Color& val)
        : _colorVal(val.getValue())
    {
    }

	/// @brief Construct with a ColorID value.
	/// @param val ColorID value to store.
    VariantBase(const td::ColorID& val)
        : _colorID(val)
    {
    }

	/// @brief Construct with a LinePattern value.
	/// @param val LinePattern value to store.
    VariantBase(const td::LinePattern& val)
        : _linePattern(val)
    {
    }

	/// @brief Construct with a DotPattern value.
	/// @param val DotPattern value to store.
    VariantBase(const td::DotPattern& val)
        : _dotPattern(val)
    {
    }

	/// @brief Construct with an Anchor value.
	/// @param val Anchor value to store.
    VariantBase(const td::Anchor& val)
        : _anchor(val)
    {
    }

    //template<size_t N >
    //VariantBase(const td::SmallChFix<N>& val)
    //    : _lu8Val(val.lu8Val())
    //{
    //}

	/// @brief Construct with a LINT8-backed Decimal value.
	/// @tparam NDEC Number of decimal places.
	/// @param val Decimal value to store.
	template<int NDEC >
	VariantBase(const Decimal<td::LINT8, NDEC>& val)
		: _decVal(val.getValue())
	{}

	/// @brief Construct with an INT4-backed Decimal value.
	/// @tparam NDEC Number of decimal places.
	/// @param val Decimal value to store.
	template<int NDEC >
	VariantBase(const Decimal<td::INT4, NDEC>& val)
		: _smallDecVal(val.getValue())
	{}

	/// @brief Construct with a SmallChFix value.
	/// @tparam NCH Fixed character capacity.
	/// @param val SmallChFix value to store.
	template<int NCH >
	VariantBase(const SmallChFix<NCH>& val)
		: _lu8Val(val.lu8Val())
	{
		//memcpy(&_chVal[0], val.c_str(), 8);
	}

	/// @brief Copy constructor; copies the raw 64-bit storage from another VariantBase.
	/// @param var Source VariantBase to copy from.
	VariantBase(const VariantBase& var)
		: _lu8Val(var._lu8Val)
	{}

	/// @brief Clean the storage for a given data type, releasing string memory if applicable.
	/// @param dataType DataType of the currently stored value.
	void clean(td::DataType dataType)
	{
		if (dataType == td::string8)
		{
			td::String& str = reinterpret_cast<td::String&>(_strVal);
			str.clean();
		}
		else
			_lu8Val = 0;
	}

	/// @brief Dispatch a mutable visitor to the stored value based on a DataType tag.
	/// @tparam TVIS Visitor type with overloaded operator() for each supported type.
	/// @tparam TRET Return type of the visitor.
	/// @param vis Visitor instance to invoke.
	/// @param type DataType of the currently stored value.
	/// @return Result returned by the matched visitor overload.
	template <typename TVIS, typename TRET>
	inline TRET applyVisitor(TVIS& vis, td::DataType type)
	{
		switch (type)
		{
		case td::boolean:      return vis(reinterpret_cast<td::BoolCh&>(_bVal));          break;
		case td::byte:         return vis(_byteVal);                                       break;
		case td::word:         return vis(_wordVal);                                       break;
		case td::int2:         return vis(_i2Val);                                         break;
		case td::uint4:        return vis(_u4Val);                                         break;
		case td::int4:         return vis(_i4Val);                                         break;
		case td::lint8:        return vis(_li8Val);                                        break;
		case td::luint8:       return vis(_lu8Val);                                        break;
		case td::real4:        return vis(_r4Val);                                         break;
		case td::real8:        return vis(_r8Val);                                         break;
		case td::string8:      return vis(reinterpret_cast<td::String&>(_strVal));         break;
		case td::ch7:          return vis(reinterpret_cast<td::ChFix7&>(_chVal));          break;
		case td::date:         return vis(reinterpret_cast<td::Date&>(_dateVal));          break;
		case td::time:         return vis(reinterpret_cast<td::Time&>(_timeVal));          break;
		case td::dateTime:     return vis(reinterpret_cast<td::DateTime&>(_dtVal));        break;
		case td::decimal0:     return vis(reinterpret_cast<td::Decimal0&>(_decVal));       break;
		case td::decimal1:     return vis(reinterpret_cast<td::Decimal1&>(_decVal));       break;
		case td::decimal2:     return vis(reinterpret_cast<td::Decimal2&>(_decVal));       break;
		case td::decimal3:     return vis(reinterpret_cast<td::Decimal3&>(_decVal));       break;
		case td::decimal4:     return vis(reinterpret_cast<td::Decimal4&>(_decVal));       break;
		case td::smallDecimal0: return vis(reinterpret_cast<td::SmallDecimal0&>(_smallDecVal)); break;
		case td::smallDecimal1: return vis(reinterpret_cast<td::SmallDecimal1&>(_smallDecVal)); break;
		case td::smallDecimal2: return vis(reinterpret_cast<td::SmallDecimal2&>(_smallDecVal)); break;
		case td::smallDecimal3: return vis(reinterpret_cast<td::SmallDecimal3&>(_smallDecVal)); break;
		case td::smallDecimal4: return vis(reinterpret_cast<td::SmallDecimal4&>(_smallDecVal)); break;
		case td::color:        return vis(reinterpret_cast<td::Color&>(_colorVal));        break;
		case td::colorID:      return vis(_colorID);                                       break;
		case td::linePattern:  return vis(_linePattern);                                   break;
		case td::dotPattern:   return vis(_dotPattern);                                    break;
		case td::anchor:       return vis(_anchor);                                        break;
		case td::TD_NONE:      return vis(none());                                         break;
		default: assert(false);
		}
		return vis(none());
	}

	/// @brief Dispatch a const visitor to the stored value based on a DataType tag.
	/// @tparam TVIS Visitor type with const overloaded operator() for each supported type.
	/// @tparam TRET Return type of the visitor.
	/// @param vis Const visitor instance to invoke.
	/// @param type DataType of the currently stored value.
	/// @return Result returned by the matched visitor overload.
	template <typename TVIS, typename TRET>
	inline TRET applyConstVisitor(TVIS& vis, td::DataType type) const
	{
		switch (type)
		{
		case td::boolean:      return vis(reinterpret_cast<const td::BoolCh&>(_bVal));          break;
		case td::byte:         return vis(_byteVal);                                             break;
		case td::word:         return vis(_wordVal);                                             break;
		case td::int2:         return vis(_i2Val);                                               break;
		case td::uint4:        return vis(_u4Val);                                               break;
		case td::int4:         return vis(_i4Val);                                               break;
		case td::lint8:        return vis(_li8Val);                                              break;
		case td::luint8:       return vis(_lu8Val);                                              break;
		case td::real4:        return vis(_r4Val);                                               break;
		case td::real8:        return vis(_r8Val);                                               break;
		case td::string8:      return vis(reinterpret_cast<const td::String&>(_strVal));         break;
		case td::ch7:          return vis(reinterpret_cast<const td::ChFix7&>(_chVal));          break;
		case td::date:         return vis(reinterpret_cast<const td::Date&>(_dateVal));          break;
		case td::time:         return vis(reinterpret_cast<const td::Time&>(_timeVal));          break;
		case td::dateTime:     return vis(reinterpret_cast<const td::DateTime&>(_dtVal));        break;
		case td::decimal0:     return vis(reinterpret_cast<const td::Decimal0&>(_decVal));       break;
		case td::decimal1:     return vis(reinterpret_cast<const td::Decimal1&>(_decVal));       break;
		case td::decimal2:     return vis(reinterpret_cast<const td::Decimal2&>(_decVal));       break;
		case td::decimal3:     return vis(reinterpret_cast<const td::Decimal3&>(_decVal));       break;
		case td::decimal4:     return vis(reinterpret_cast<const td::Decimal4&>(_decVal));       break;
		case td::smallDecimal0: return vis(reinterpret_cast<const td::SmallDecimal0&>(_smallDecVal)); break;
		case td::smallDecimal1: return vis(reinterpret_cast<const td::SmallDecimal1&>(_smallDecVal)); break;
		case td::smallDecimal2: return vis(reinterpret_cast<const td::SmallDecimal2&>(_smallDecVal)); break;
		case td::smallDecimal3: return vis(reinterpret_cast<const td::SmallDecimal3&>(_smallDecVal)); break;
		case td::smallDecimal4: return vis(reinterpret_cast<const td::SmallDecimal4&>(_smallDecVal)); break;
		case td::color:        return vis(reinterpret_cast<const td::Color&>(_colorVal));        break;
		case td::colorID:      return vis(_colorID);                                             break;
		case td::linePattern:  return vis(_linePattern);                                         break;
		case td::dotPattern:   return vis(_dotPattern);                                          break;
		case td::anchor:       return vis(_anchor);                                              break;
		case td::TD_NONE:      return vis(none());                                               break;
		default: assert(false);
		}
		return vis(none());
	}

	/// @brief Assign the value from a td::Variant using the BaseToVariantVisitor.
	/// @param var Source Variant whose value will be copied into this base.
	inline void operator = (const td::Variant& var);
};
}
