// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Types.h
    @brief Common data-binding types, wrapper templates, and helper free functions for the dp (data processing) layer. */
#pragma once
#include <td/String.h>
#include <td/GUID.h>
#include <td/BLOB.h>
#include <cnt/SafeFullVector.h>
#include <td/VariantDesc.h>
#include <td/BoolCh.h>

namespace dp
{
	/// @brief Identifies the category of an enumeration used in metadata descriptions.
	typedef enum _ET{
		ET_CPPType=0, ///< Enumeration describes C++ data types
		ET_HAlign,    ///< Enumeration describes horizontal alignment options
		ET_VAlign,    ///< Enumeration describes vertical alignment options
		ET_NamedColors ///< Enumeration describes named color values
	} EnumType;

	/// @brief Describes a single column using C++ type information.
	typedef struct _cd
	{
		td::String  name;    ///< Column name
		td::DataType tdType; ///< C++ data type of the column
		int len;             ///< Maximum length for string-typed columns, in characters
	} CPPColumnDesc;

	/// @brief Typed wrapper that holds a reference to a value and records its database type and precision.
	/// @tparam TREF The C++ type of the referenced value.
	template<class TREF>
	class Ref
	{
		bool isNullable; ///< Indicates whether the reference may point to a null value
	public:
		TREF* refPtr;          ///< Pointer to the referenced value
		td::DataType cppType;  ///< The td data type corresponding to TREF
		int precision;         ///< Precision or maximum character length for string types

		//TPARENT* parent;

		/// @brief Constructs a non-nullable reference bound to an existing value.
		/// @param refVal The value to reference.
		Ref(TREF& refVal)
			: refPtr(&refVal)
			, cppType(td::getType(refVal))
			, precision(0)
			, isNullable(false)
		{
		}

		/// @brief Constructs a non-nullable reference with explicit type and precision override.
		/// @param refVal The value to reference.
		/// @param prec Precision or maximum character length.
		/// @param type Explicit td data type override.
		Ref(TREF& refVal, int prec, td::DataType type = td::nch)
			: refPtr(&refVal)
			, cppType(type)
			, precision(prec)
			, isNullable(false)
		{
		}

		/// @brief Constructs a nullable reference placeholder that is not yet bound to any value.
		/// @param prec Precision or maximum character length.
		/// @param type Explicit td data type.
		Ref(int prec, td::DataType type = td::nch)
			: refPtr(0)
			, cppType(type)
			, precision(prec)
			, isNullable(true)
		{
		}

		/// @brief Marks this reference as nullable, allowing the internal pointer to be null.
		void setAsNullable()
		{
			isNullable = true;
		}

		/// @brief Assigns the reference to point to a different value; respects nullable semantics.
		/// @param refVal The new value to reference; if nullable and empty, the pointer is set to null.
		void operator = (TREF& refVal)
		{
			if (isNullable)
			{
				if (refVal.length() == 0)
					refPtr = 0;
				else
					refPtr = &refVal;
			}
			else
				refPtr = &refVal;
		}
	};


	/// @brief Wraps a date/time value together with its specific database type tag.
	/// @tparam T The C++ date/time type (e.g. td::Date, td::Time, td::DateTime).
	template<class T>
	class DBDateTime
	{
		T& tdDateTime;        ///< Reference to the underlying date/time value
		td::DataType tdType;  ///< The database type tag (dbDate, dbTime, or dbDateTime)
	public:
		/// @brief Constructs a DBDateTime wrapper.
		/// @param val Reference to the date/time value to wrap.
		/// @param type The database type identifier for this value.
		DBDateTime(T& val, td::DataType type)
			: tdDateTime(val)
			, tdType(type)
		{
		}

		/// @brief Returns a reference to the wrapped date/time value.
		/// @return Reference to the date/time value.
		T& getValue()
		{
			return tdDateTime;
		}

		/// @brief Returns the database type tag associated with this date/time wrapper.
		/// @return The td::DataType tag.
		td::DataType getType() const
		{
			return tdType;
		}
	};

	/// @brief Creates a DBDateTime wrapper for a td::Date value mapped to the dbDate type.
	/// @param val The date value to wrap.
	/// @return A DBDateTime<td::Date> wrapper tagged as dbDate.
	inline DBDateTime<td::Date> toDate(td::Date& val)
	{
		return DBDateTime<td::Date>(val, td::dbDate);
	}

	/// @brief Creates a DBDateTime wrapper for a td::Time value mapped to the dbTime type.
	/// @param val The time value to wrap.
	/// @return A DBDateTime<td::Time> wrapper tagged as dbTime.
	inline DBDateTime<td::Time> toTime(td::Time& val)
	{
		return DBDateTime<td::Time>(val, td::dbTime);
	}

	/// @brief Creates a DBDateTime wrapper for a td::DateTime value mapped to the dbDateTime type.
	/// @param val The datetime value to wrap.
	/// @return A DBDateTime<td::DateTime> wrapper tagged as dbDateTime.
	inline DBDateTime<td::DateTime> toDateTime(td::DateTime& val)
	{
		return DBDateTime<td::DateTime>(val, td::dbDateTime);
	}

	/// @brief Wraps a string value together with its database string type and declared character length.
	/// @tparam T The C++ string type (e.g. td::String).
	template<class T>
	class DBString
	{
		T& tdString;          ///< Reference to the underlying string value
		td::DataType tdType;  ///< The database string type tag (ch, nch, chFix, nchFix, etc.)
		td::WORD charLen;     ///< Declared maximum character length for this column or parameter
	public:
		/// @brief Constructs a DBString wrapper.
		/// @param val Reference to the string value to wrap.
		/// @param type The database string type identifier.
		/// @param chLen Maximum character length declared for the column or parameter.
		DBString(T& val, td::DataType type, td::WORD chLen)
			: tdString(val)
			, tdType(type)
			, charLen(chLen)
		{
		}

		/// @brief Returns a const pointer to the wrapped string value.
		/// @return Const pointer to the string value.
        const T* getValuePtr() const
        {
            return &tdString;
        }

		/// @brief Returns a mutable pointer to the wrapped string value.
		/// @return Pointer to the string value.
        T* getValuePtr()
        {
            return &tdString;
        }

		/// @brief Returns a reference to the wrapped string value.
		/// @return Reference to the string value.
		T& getValue()
		{
			return tdString;
		}

		/// @brief Returns the database type tag associated with this string wrapper.
		/// @return The td::DataType tag.
		td::DataType getType() const
		{
			return tdType;
		}

		/// @brief Returns the declared maximum character length for this string column or parameter.
		/// @return Maximum character length.
		td::WORD getLength() const
		{
			return charLen;
		}
	};

	/// @brief Reinterprets the high 64-bit word of a GUID as a signed 64-bit integer reference.
	/// @param guidVal The GUID value to access.
	/// @return Reference to the high word of the GUID as td::LINT8.
	inline td::LINT8& toGUID1(const td::GUID& guidVal)
	{
		return (td::LINT8&)guidVal.hi;
	}

	/// @brief Reinterprets the low 64-bit word of a GUID as a signed 64-bit integer reference.
	/// @param guidVal The GUID value to access.
	/// @return Reference to the low word of the GUID as td::LINT8.
	inline td::LINT8& toGUID2(const td::GUID& guidVal)
	{
		return (td::LINT8&) guidVal.lo;
	}

	/// @brief Creates a DBString wrapper for a variable-length ANSI character column (td::ch).
	/// @tparam T The string type.
	/// @param val Reference to the string value.
	/// @param charLen Maximum character length for the column.
	/// @return A DBString<T> wrapper tagged as td::ch.
	template<class T>
	DBString<T> toCh(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::ch, charLen);
	}

	/// @brief Creates a DBString wrapper for a variable-length Unicode character column (td::nch).
	/// @tparam T The string type.
	/// @param val Reference to the string value.
	/// @param charLen Maximum character length for the column.
	/// @return A DBString<T> wrapper tagged as td::nch.
	template<class T>
	DBString<T> toNCh(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::nch, charLen);
	}

	/// @brief Creates a DBString wrapper for a fixed-length ANSI character column (td::chFix).
	/// @tparam T The string type.
	/// @param val Reference to the string value.
	/// @param charLen Fixed character length for the column.
	/// @return A DBString<T> wrapper tagged as td::chFix.
	template<class T>
	DBString<T> toChFix(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::chFix, charLen);
	}

	/// @brief Creates a DBString wrapper for a fixed-length Unicode character column (td::nchFix).
	/// @tparam T The string type.
	/// @param val Reference to the string value.
	/// @param charLen Fixed character length for the column.
	/// @return A DBString<T> wrapper tagged as td::nchFix.
	template<class T>
	DBString<T> toNChFix(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::nchFix, charLen);
	}

	/// @brief Wraps a td::Decimal value so it can be passed as a typed database parameter.
	/// @tparam TDEC The underlying integer storage type of the decimal.
	/// @tparam NLEN The number of decimal places (scale).
	template<typename TDEC, int NLEN>
	class DBDecimal
	{
		td::Decimal<TDEC, NLEN>& tdDecimal; ///< Reference to the wrapped decimal value
	public:
		/// @brief Constructs a DBDecimal wrapper around an existing decimal value.
		/// @param val Reference to the decimal value to wrap.
		DBDecimal(td::Decimal<TDEC, NLEN>& val)
			: tdDecimal(val)
		{
		}

		/// @brief Returns a reference to the wrapped decimal value.
		/// @return Reference to the decimal value.
		td::Decimal<TDEC, NLEN>& getValue()
		{
			return tdDecimal;
		}
	};

	/// @brief Creates a DBDecimal wrapper for a td::Decimal value.
	/// @tparam TDEC The underlying integer storage type of the decimal.
	/// @tparam NLEN The number of decimal places (scale).
	/// @param val Reference to the decimal value to wrap.
	/// @return A DBDecimal wrapper for the given value.
	template<typename TDEC, int NLEN>
	DBDecimal<TDEC, NLEN> toDec(td::Decimal<TDEC, NLEN>& val)
	{
		return DBDecimal<TDEC, NLEN>(val);
	}
}
