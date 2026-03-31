// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <td/GUID.h>
#include <td/BLOB.h>
#include <cnt/SafeFullVector.h>
#include <td/VariantDesc.h>
#include <td/BoolCh.h>

namespace dp
{
	typedef enum _ET{ET_CPPType=0, ET_HAlign, ET_VAlign, ET_NamedColors} EnumType;
	
	typedef struct _cd
	{
		td::String  name;
		td::DataType tdType;
		int len;
	} CPPColumnDesc;

	template<class TREF>
	class Ref
	{
		bool isNullable;
	public:
		TREF* refPtr;
		td::DataType cppType;
		int precision;

		//TPARENT* parent;

		Ref(TREF& refVal)
			: refPtr(&refVal)
			, cppType(td::getType(refVal))
			, precision(0)
			, isNullable(false)
		{
		}

		Ref(TREF& refVal, int prec, td::DataType type = td::nch)
			: refPtr(&refVal)
			, cppType(type)
			, precision(prec)
			, isNullable(false)
		{
		}

		Ref(int prec, td::DataType type = td::nch)
			: refPtr(0)
			, cppType(type)
			, precision(prec)
			, isNullable(true)
		{
		}

		void setAsNullable()
		{
			isNullable = true;
		}

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


	template<class T>
	class DBDateTime
	{
		T& tdDateTime;
		td::DataType tdType;
	public:
		DBDateTime(T& val, td::DataType type)
			: tdDateTime(val)
			, tdType(type)
		{
		}

		T& getValue()
		{
			return tdDateTime;
		}

		td::DataType getType() const
		{
			return tdType;
		}
	};

	inline DBDateTime<td::Date> toDate(td::Date& val)
	{
		return DBDateTime<td::Date>(val, td::dbDate);
	}

	inline DBDateTime<td::Time> toTime(td::Time& val)
	{
		return DBDateTime<td::Time>(val, td::dbTime);
	}

	inline DBDateTime<td::DateTime> toDateTime(td::DateTime& val)
	{
		return DBDateTime<td::DateTime>(val, td::dbDateTime);
	}

	template<class T>
	class DBString
	{
		T& tdString;
		td::DataType tdType;
		td::WORD charLen;
	public:
		DBString(T& val, td::DataType type, td::WORD chLen)
			: tdString(val)
			, tdType(type)
			, charLen(chLen)
		{
		}

        const T* getValuePtr() const
        {
            return &tdString;
        }
        
        T* getValuePtr()
        {
            return &tdString;
        }
        
		T& getValue()
		{
			return tdString;
		}

		td::DataType getType() const
		{
			return tdType;
		}

		td::WORD getLength() const
		{
			return charLen;
		}
	};

	inline td::LINT8& toGUID1(const td::GUID& guidVal)
	{
		return (td::LINT8&)guidVal.hi;
	}

	inline td::LINT8& toGUID2(const td::GUID& guidVal)
	{
		return (td::LINT8&) guidVal.lo;
	}

	template<class T>
	DBString<T> toCh(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::ch, charLen);
	}

	template<class T>
	DBString<T> toNCh(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::nch, charLen);
	}

	template<class T>
	DBString<T> toChFix(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::chFix, charLen);
	}

	template<class T>
	DBString<T> toNChFix(T& val, td::WORD charLen)
	{
		return DBString<T>(val, td::nchFix, charLen);
	}

	template<typename TDEC, int NLEN>
	class DBDecimal
	{
		td::Decimal<TDEC, NLEN>& tdDecimal;
	public:
		DBDecimal(td::Decimal<TDEC, NLEN>& val)
			: tdDecimal(val)
		{
		}

		td::Decimal<TDEC, NLEN>& getValue()
		{
			return tdDecimal;
		}
	};

	template<typename TDEC, int NLEN>
	DBDecimal<TDEC, NLEN> toDec(td::Decimal<TDEC, NLEN>& val)
	{
		return DBDecimal<TDEC, NLEN>(val);
	}	
}
