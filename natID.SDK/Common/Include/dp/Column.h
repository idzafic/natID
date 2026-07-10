// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Column.h
 * @brief Defines the Column and Columns classes for binding C++ variables to database columns.
 */
#pragma once
#include <dp/Types.h>

namespace td
{
    /// @brief Returns the td::DataType tag for any enum type by delegating to its underlying integer type.
    /// @tparam T Enum type whose underlying type is queried.
    /// @param  (unnamed) Unused value; used only for type deduction.
    /// @return The DataType corresponding to the enum's underlying integral type.
    template<class T, class = std::enable_if_t<std::is_enum_v<T>>>
    inline DataType getType(T)
    {
        return getType(std::underlying_type_t<T>{});
    }
}

namespace dp
{
    /// @brief Represents a single database column binding, associating a name, data type, and pointer to a C++ variable.
	class Column
	{
	public:
        /// @brief Default constructor; initialises all fields to safe defaults.
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
		td::String name;         ///< Column name as used in the SQL statement.
        void* pData;             ///< Pointer to the bound C++ variable.
		td::INT4 dataSize;       ///< Size in bytes of the bound variable.
        td::DataType cppType;    ///< C++ data type identifier of the bound variable.
		char posInSelect;        ///< Zero-based position of this column in the SELECT list.
        unsigned char dbSelType : 4; //samo za SQLite ///< Database selection type flags (SQLite only).
        unsigned char isReference : 1; ///< Non-zero when the column is bound via a Ref<> holder.
        //char fromDP : 1;
#ifdef _DEBUG
		unsigned char initialized : 1; ///< Debug flag; non-zero after a successful init() call.
#endif

        /// @brief Writes a brief description of the column to the supplied output stream.
        /// @tparam TSTREAM Type of the output stream.
        /// @param o The stream to write to.
		template <class TSTREAM>
		void show(TSTREAM& o)
		{
			o << "Name: " << name.c_str() << std::endl;
		}

        /// @brief Sets the column name from a compile-time string literal.
        /// @tparam size Length of the string literal including the null terminator.
        /// @param strColumnName The column name as a character array literal.
		template <size_t size>
		void setName(const char(&strColumnName)[size])
		{
			name.fromCTString(strColumnName);
		}

        /// @brief Sets the column name from a null-terminated C string.
        /// @param strColumnName Pointer to the null-terminated column name.
		void setName(const char* strColumnName)
		{
			name = strColumnName;
		}

        /// @brief Sets the column name from a String object.
        /// @param strColumnName The column name as a td::String.
        void setName(const td::String& strColumnName)
        {
            name = strColumnName;
        }

        /// @brief Updates the data pointer to point at the given variable.
        /// @tparam T Type of the variable.
        /// @param val The variable whose address will be stored in pData.
		template <typename T>
		void updatePtr(T& val)
		{
			pData = reinterpret_cast<void*>(&val);
		}

        /// @brief Initialises the column binding from a Ref<> holder.
        /// @tparam TREF The referenced value type.
        /// @param refHolder Reference holder whose type and pointer will be recorded.
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

        /// @brief Initialises the column as a reference binding from a Ref<> holder (alias for init).
        /// @tparam TREF The referenced value type.
        /// @param refHolder Reference holder whose type and pointer will be recorded.
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

        /// @brief Initialises the column binding from a DBDecimal wrapper.
        /// @tparam TDEC Underlying decimal integer type.
        /// @tparam NLEN Number of decimal places.
        /// @param val The DBDecimal value to bind.
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

        /// @brief Initialises the column binding for a boolean value stored as a single character.
        /// @param val Pointer to the character that holds the boolean value.
		void initBoolAsStr(char* val)
		{
			cppType = td::boolean;
			pData = (void*)val;
			dataSize = 1;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

        /// @brief Initialises the column binding from a fixed-size small character array.
        /// @tparam NCH Maximum number of characters.
        /// @param val Pointer to the SmallChFix variable.
        /// @param realLen Actual length to use; defaults to NCH.
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

        /// @brief Initialises the column binding from a BLOB variable.
        /// @param blob The BLOB object to bind.
		void init(td::BLOB& blob)
		{
			cppType = td::binary;
			pData = reinterpret_cast<void*>(&blob);
			dataSize = 0;
#ifdef _DEBUG
			initialized = 1;
#endif
		}

        /// @brief Initialises the column binding from an arbitrary typed variable (by reference).
        /// @tparam T Type of the variable.
        /// @param val The variable to bind.
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

        /// @brief Initialises the column binding from an arbitrary typed variable (by pointer).
        /// @tparam T Type of the variable.
        /// @param val Pointer to the variable to bind.
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

        /// @brief Initialises the column with a name (literal) and a DBDecimal binding.
        /// @tparam TDEC Underlying decimal integer type.
        /// @tparam NLEN Number of decimal places.
        /// @tparam size Length of the name literal.
        /// @param strColumnName Column name as a character array literal.
        /// @param val The DBDecimal value to bind.
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

        /// @brief Initialises the column with a name (literal) and a typed variable binding.
        /// @tparam T Type of the variable.
        /// @tparam size Length of the name literal.
        /// @param strColumnName Column name as a character array literal.
        /// @param val The variable to bind.
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

        /// @brief Initialises the column with a String name and a DBDecimal binding.
        /// @tparam TDEC Underlying decimal integer type.
        /// @tparam NLEN Number of decimal places.
        /// @param strColumnName Column name as a td::String.
        /// @param val The DBDecimal value to bind.
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

        /// @brief Initialises the column with a String name and a typed variable binding.
        /// @tparam T Type of the variable.
        /// @param strColumnName Column name as a td::String.
        /// @param val The variable to bind.
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

    /// @brief Provides a stream-like interface for populating a vector of Column objects sequentially.
	class Columns
	{
	protected:
		cnt::SafeFullVector<Column>& _columns; ///< Reference to the underlying column storage.
		int iCol;                              ///< Index of the column currently being configured.

		template <size_t size, typename TVAL>
		friend class COL;
	public:
        /// @brief Accesses the column at the given index.
        /// @param i Zero-based column index.
        /// @return Reference to the requested Column.
		Column& operator[] (size_t i)
		{
			return _columns[i];
		}

        /// @brief Copy constructor; shares the same underlying column storage.
        /// @param cols The Columns object to copy from.
		Columns(const Columns& cols)
			: _columns(cols._columns)
			, iCol(0)
		{
		}

        /// @brief Sets the name of the current column from a string literal and advances the cursor.
        /// @tparam size Length of the literal.
        /// @param strColumnName The column name.
        /// @return Reference to this Columns object for chaining.
		template <size_t size>
		Columns& operator << (const char(&strColumnName)[size])
		{
			_columns[iCol].setName(strColumnName);
			return *this;
		}

        /// @brief Sets the name of the current column from a C string and advances the cursor.
        /// @param strColumnName Null-terminated column name.
        /// @return Reference to this Columns object for chaining.
		Columns& operator << (const char* strColumnName)
		{
			_columns[iCol].setName(strColumnName);
			return *this;
		}

        /// @brief Binds the current column to a Variant value and advances the cursor.
        /// @param var The Variant variable to bind.
        /// @return Reference to this Columns object for chaining.
		Columns& operator << (td::Variant& var)
		{
			_columns[iCol++].init(var);
			return *this;
		}

        /// @brief Binds the current column to a VariantDesc (forwarded as Variant) and advances the cursor.
        /// @param var The VariantDesc variable to bind.
        /// @return Reference to this Columns object for chaining.
		Columns& operator << (td::VariantDesc& var)
		{
			*this << (td::Variant&)(var);
			return *this;
		}

        /// @brief Binds the current column to an arbitrary typed variable and advances the cursor.
        /// @tparam T Type of the variable.
        /// @param val The variable to bind.
        /// @return Reference to this Columns object for chaining.
		template <typename T>
		Columns& operator << (T& val)
		{
			_columns[iCol++].init(val);
			return *this;
		}

        /// @brief Constructs a Columns wrapper over the given column storage vector.
        /// @param cols Reference to the vector of Column objects to wrap.
		Columns(cnt::SafeFullVector<Column>& cols)
			: _columns(cols)
			, iCol(0)
		{
		}
	};
}
