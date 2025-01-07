// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/Types.h>

namespace td
{
    template<class T, class = std::enable_if_t<std::is_enum_v<T>>>
    inline DataType getType(T)
    {
        return getType(std::underlying_type_t<T>{});
    }
}

namespace dp
{
	class Column
	{
	public:
		Column()
			: pData(nullptr)
            , dataSize(0)
            , cppType(td::TD_NONE)
			, posInSelect(-1)
			, dbSelType(0)
            , isReference(0)
            //, fromDP(1)
#ifdef _DEBUG
			, initialized(0)
#endif
		{}
		td::String name;
        void* pData;
		td::INT4 dataSize;
        td::DataType cppType;
		char posInSelect;
        unsigned char dbSelType : 4; //samo za SQLite
        unsigned char isReference : 1;
        //char fromDP : 1;
#ifdef _DEBUG
		unsigned char initialized : 1;
#endif

		template <class TSTREAM>
		void show(TSTREAM& o)
		{
			o << "Name: " << name.c_str() << std::endl;
		}

		template <size_t size>
		void setName(const char(&strColumnName)[size])
		{
			name.fromCTString(strColumnName);
		}

		void setName(const char* strColumnName)
		{
			name = strColumnName;
		}
        
        void setName(const td::String& strColumnName)
        {
            name = strColumnName;
        }

		template <typename T>
		void updatePtr(T& val)
		{
			pData = reinterpret_cast<void*>(&val);
		}

		template<class TREF>
		void init(Ref<TREF>& refHolder)
		{
			cppType = refHolder.cppType;
			pData = reinterpret_cast<void*> (&refHolder);
			dataSize = sizeof(TREF);
			isReference = 1;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template<class TREF>
		void initRef(Ref<TREF>& refHolder)
		{
			cppType = refHolder.cppType;
			pData = reinterpret_cast<void*> (&refHolder);
			dataSize = sizeof(TREF);
			isReference = 1;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <typename TDEC, int NLEN>
		void init(DBDecimal<TDEC, NLEN>& val)
		{
			cppType = td::dbDecimal;
			pData = reinterpret_cast<void*>(&(val.getValue()));
			dataSize = (td::INT4) sizeof(td::Decimal<TDEC, NLEN>);
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		void initBoolAsStr(char* val)
		{
			cppType = td::boolean;
			pData = (void*)val;
			dataSize = 1;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <size_t NCH>
		void init(td::SmallChFix<NCH>* val, int realLen = NCH)
		{
			cppType = td::ch7;
			pData = (void*)val;
			dataSize = realLen;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		void init(td::BLOB& blob)
		{
			cppType = td::binary;
			pData = reinterpret_cast<void*>(&blob);
			dataSize = 0;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <typename T>
		void init(T& val)
		{
			cppType = td::getType(val);
			pData = reinterpret_cast<void*>(&val);
			dataSize = (td::INT4) sizeof(T);
#ifdef _DEBUG
			initialized = 1;
#endif
		}
        
        template <typename T>
        void init(T* val)
        {
            cppType = td::getType(*val);
            pData = reinterpret_cast<void*>(val);
            dataSize = (td::INT4) sizeof(T);
#ifdef _DEBUG
            initialized = 1;
#endif
        }

		template <typename TDEC, int NLEN, size_t size>
		void init(const char(&strColumnName)[size], DBDecimal<TDEC, NLEN>& val)
		{
			name.fromCTString(strColumnName);
			cppType = td::dbDecimal;
			pData = reinterpret_cast<void*>(&(val.getValue()));
			dataSize = (td::INT4) sizeof(td::Decimal<TDEC, NLEN>);
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <typename T, size_t size>
		void init(const char(&strColumnName)[size], T& val)
		{
			name.fromCTString(strColumnName);
			cppType = td::getType(val);
			pData = reinterpret_cast<void*>(&val);
			dataSize = (td::INT4) sizeof(T);
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <typename TDEC, int NLEN>
		void init(const td::String& strColumnName, DBDecimal<TDEC, NLEN>& val)
		{
			name = strColumnName;
			cppType = td::dbDecimal;
			pData = reinterpret_cast<void*>(&(val.getValue()));
			dataSize = (td::INT4) sizeof(td::Decimal<TDEC, NLEN>);
#ifdef _DEBUG
			initialized = 1;
#endif
		}

		template <typename T>
		void init(const td::String& strColumnName, T& val)
		{
			name = strColumnName;
			cppType = td::getType(val);
			pData = reinterpret_cast<void*>(&val);
			dataSize = (td::INT4) sizeof(T);
#ifdef _DEBUG
			initialized = 1;
#endif
		}
	};

	class Columns
	{
	protected:
		cnt::SafeFullVector<Column>& _columns;
		int iCol;

		template <size_t size, typename TVAL>
		friend class COL;
	public:
		Column& operator[] (size_t i)
		{
			return _columns[i];
		}

		Columns(const Columns& cols)
			: _columns(cols._columns)
			, iCol(0)
		{
		}

		template <size_t size>
		Columns& operator << (const char(&strColumnName)[size])
		{
			_columns[iCol].setName(strColumnName);
			return *this;
		}

		Columns& operator << (const char* strColumnName)
		{
			_columns[iCol].setName(strColumnName);
			return *this;
		}

		Columns& operator << (td::Variant& var)
		{
			_columns[iCol++].init(var);
			return *this;
		}

		Columns& operator << (td::VariantDesc& var)
		{
			*this << (td::Variant&)(var);
			return *this;
		}

		template <typename T>
		Columns& operator << (T& val)
		{
			_columns[iCol++].init(val);
			return *this;
		}

		Columns(cnt::SafeFullVector<Column>& cols)
			: _columns(cols)
			, iCol(0)
		{
		}
	};
}
