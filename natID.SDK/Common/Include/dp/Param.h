// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Param.h
    @brief Database parameter and parameter-collection types used for statement binding. */
#pragma once
#include <dp/Column.h>

namespace dp
{
/// @brief Represents a single database parameter with type, direction, precision, and scale metadata.
	class Param : public Column
	{
	public:
		/// @brief Specifies the direction of the parameter.
		enum class Type : unsigned char {
			INPUT = 1, ///< Parameter supplies a value to the statement
			OUTPUT     ///< Parameter receives a value from the statement
		};
        td::INT4 paramLength; ///< Actual length of the parameter value in bytes
        td::INT4 maxLength;   ///< Maximum allowed length of the parameter value in bytes
        td::WORD precision;   ///< Numeric precision or maximum character count for string types
        td::WORD scale;       ///< Number of digits to the right of the decimal point
		Type paramType;       ///< Direction of the parameter (INPUT or OUTPUT)
		td::DataType dbType;  ///< Database-level data type override for this parameter

		/// @brief Default constructor; leaves all fields uninitialised.
		Param() {}

		/// @brief Initialises the parameter from a DBDecimal value.
		/// @tparam TDEC The underlying integer storage type of the decimal.
		/// @tparam NLEN The number of decimal places (scale).
		/// @param val Reference to the DBDecimal value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		/// @param parPrecision Numeric precision; 0 uses the type default.
		/// @param databaseType Explicit database type override; td::nch means use the inferred type.
		template <typename TDEC, int NLEN>
		void init(DBDecimal<TDEC, NLEN>& val, Type parType = Param::Type::INPUT, td::WORD parPrecision = 0, td::DataType databaseType = td::nch)
		{
			//Column::init("", val);
			Column::init(val);

			dbType = cppType;

			precision = parPrecision;
			scale = 0;
			paramType = parType;
			maxLength = 0;
			int tmpType = td::getType(val.getValue());

			if (tmpType == td::decimal2)
			{
				precision = 19;
				scale = 2;
			}
			else if (tmpType == td::decimal3)
			{
				precision = 19;
				scale = 3;
			}
			else if (tmpType == td::decimal4)
			{
				precision = 19;
				scale = 4;
			}

			if ((databaseType == td::string8) || (cppType == td::string16))
			{
				//it is not allowed to have precision of zero length
				assert(precision != 0);
			}
		}

		/// @brief Initialises the parameter from a Ref<TREF> typed reference holder.
		/// @tparam TREF The referenced C++ value type.
		/// @param refVal Reference to the Ref wrapper to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		template<class TREF>
		void init(Ref<TREF>& refVal, Type parType = Type::INPUT)
		{
			Column::init(refVal);
			dbType = refVal.cppType;
			precision = refVal.precision;
			paramType = parType;
			maxLength = 0;
			paramLength = 0;
		}

		/// @brief Initialises the parameter from a Ref<TREF> using the reference-binding (initRef) path.
		/// @tparam TREF The referenced C++ value type.
		/// @param refVal Reference to the Ref wrapper to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		template<class TREF>
		void initRef(Ref<TREF>& refVal, Type parType = Param::Type::INPUT)
		{
			Column::initRef(refVal);
			dbType = refVal.cppType;
			precision = refVal.precision;
			paramType = parType;
			maxLength = 0;
			paramLength = 0;
		}

		/// @brief Initialises the parameter from a BLOB value.
		/// @param blob Reference to the BLOB value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		void initBLOB(td::BLOB& blob, Type parType = Param::Type::INPUT)
		{
			Column::init(blob);
			dbType = cppType;
			precision = 0;
			paramType = parType;
			maxLength = 0;
			paramLength = 0;
		}

		/// @brief Initialises the parameter from a td::Variant value, applying type-specific adjustments.
		/// @param val Reference to the variant value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		void init(td::Variant& val, Type parType = Param::Type::INPUT)
		{
			if (val.getType() == td::boolean)
			{
				Column::initBoolAsStr((char*)&val);
				dbType = td::chFix;
				precision = 1;
				scale = 0;
				paramType = parType;
				maxLength = 1;
				return;
			}
			else if (val.getType() == td::ch7)
			{
				Column::init(val.chFixVal());
				dbType = td::chFix;
				precision = (td::WORD) val.getDBLen();
				scale = 0;
				paramType = parType;
				maxLength = precision;
				return;
			}

			td::DataType dbType = val.getDBType();
			td::UINT4 len = 0;
			if (dbType != td::TD_NONE)
			{
				if (td::isDBString(dbType))
					len = val.getDBLen();
			}

			init(val, parType, (td::WORD) len, dbType);
		}

		/// @brief Initialises the parameter from a SmallChFix<NCH> fixed-character value.
		/// @tparam NCH The fixed character count of the SmallChFix type.
		/// @param val Reference to the fixed-character value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		template <size_t NCH>
		void init(td::SmallChFix<NCH>& val, Type parType = Param::Type::INPUT)
		{
			//Column::init("", val);
			Column::init(&val);

			dbType = td::chFix;

			precision = NCH;
			scale = 0;
			paramType = parType;
			maxLength = NCH;
		}

		/// @brief Initialises the parameter from a SmallChFix<7> with an explicit database type and length override.
		/// @param val Reference to the 7-character fixed value to bind.
		/// @param realDBLen Actual database column length to declare.
		/// @param realDbType Actual database type to declare.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		void init(td::SmallChFix<7>& val, td::WORD realDBLen, td::DataType realDbType, Type parType = Param::Type::INPUT)
		{
			//Column::init("", val);
			Column::init(&val, realDBLen);

			dbType = realDbType;

			precision = realDBLen;
			scale = 0;
			paramType = parType;
			maxLength = realDBLen;
		}

		/// @brief Initialises the parameter from a pointer to a td::String value.
		/// @param val Pointer to the string value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		/// @param parPrecision Declared character length; 0 means infer from the string type.
		/// @param databaseType Explicit database type override.
        void init(td::String* val, Type parType = Param::Type::INPUT, td::WORD parPrecision = 0, td::DataType databaseType = td::nch)
        {
            //Column::init("", val);
            Column::init(val);

            dbType = cppType;

            precision = parPrecision;
            scale = 0;
            paramType = parType;
            maxLength = 0;
            dbType = databaseType;
        }

		/// @brief Generic initialiser for any supported value type with optional precision and type override.
		/// @tparam T The C++ value type to bind.
		/// @param val Reference to the value to bind.
		/// @param parType Direction of the parameter (INPUT or OUTPUT).
		/// @param parPrecision Precision or maximum character length; 0 uses the type default.
		/// @param databaseType Explicit database type override; td::nch means use the inferred type.
		template <typename T>
		void init(T& val, Type parType = Param::Type::INPUT, td::WORD parPrecision = 0, td::DataType databaseType = td::nch)
		{
			//Column::init("", val);
			Column::init(val);

			dbType = cppType;

			precision = parPrecision;
			scale = 0;
			paramType = parType;
			maxLength = 0;

			switch (cppType)
			{
			case td::decimal0:
				precision = 19;
				scale = 0;
				break;
			case td::decimal1:
				precision = 19;
				scale = 1;
				break;
			case td::decimal2:
				precision = 19;
				scale = 2;
				break;
			case td::decimal3:
				precision = 19;
				scale = 3;
				break;
			case td::decimal4:
				precision = 19;
				scale = 4;
				break;
			case td::smallDecimal0:
				precision = 9;
				scale = 0;
				break;
			case td::smallDecimal1:
				precision = 9;
				scale = 1;
				break;
			case td::smallDecimal2:
				precision = 9;
				scale = 2;
				break;
			case td::smallDecimal3:
				precision = 9;
				scale = 3;
				break;
			case td::smallDecimal4:
				precision = 9;
				scale = 4;
				break;
			case td::string8:
			case td::string16:
			case td::ch7:
				dbType = databaseType;
				//it is not allowed to have precision of zero length
				//assert(precision != 0);
				break;
			case td::boolean:
				dbType = td::chFix;
				precision = 1;
				break;
			default:
				precision = parPrecision;
				scale = 0;
			}
		}
	};

/// @brief Helper class that wraps a parameter vector and provides a streaming operator interface
///        for sequential parameter binding.
	class Params
	{
	protected:
		cnt::SafeFullVector<Param>& _params; ///< Reference to the underlying parameter storage vector
		int iParPos;                          ///< Current write position (zero-based) within the parameter vector
		Param::Type parType;                  ///< Direction applied to the next parameter pushed via operator<<
		//td::WORD parPrecision;
		//td::DataType databaseType;

		/// @brief Resets transient streaming state to its default values after each parameter is pushed.
		void cleanTmpData()
		{
			parType = Param::Type::INPUT;
			//parPrecision = 0;
			//databaseType = td::nch;
		}

		/// @brief Binds a td::String value to the next parameter position.
		/// @param strVal Reference to the string value to bind.
		/// @return Reference to this Params object for chaining.
		Params& operator << (td::String& strVal)
		{
			_params[iParPos++].init(strVal, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

	public:
		/// @brief Returns the parameter at the given index by reference.
		/// @param i Zero-based index of the parameter.
		/// @return Reference to the Param at position i.
		Param& operator[] (int i) { return _params[i]; }

		/// @brief Copy constructor; shares the underlying parameter vector.
		/// @param pars The Params instance to copy from.
		Params(const Params& pars)
			: _params(pars._params)
            , parType (Param::Type::INPUT)
			, iParPos(0)
		{
			cleanTmpData();
		}

		/// @brief Constructs a Params wrapper around an existing parameter vector.
		/// @param params Reference to the SafeFullVector of Param objects to wrap.
		Params(cnt::SafeFullVector<Param>& params)
			: _params(params)
			, iParPos(0)
		{
			cleanTmpData();
		}

		/// @brief Sets the direction applied to the next parameter pushed via operator<<.
		/// @param paramType The direction to apply (INPUT or OUTPUT).
		/// @return Reference to this Params object for chaining.
		Params& operator << (Param::Type paramType)
		{
			parType = paramType;
			return *this;
		}

		/// @brief Binds a BLOB value to the next parameter position.
		/// @param blob Reference to the BLOB value to bind.
		/// @return Reference to this Params object for chaining.
		Params& operator << (td::BLOB& blob)
		{
			_params[iParPos++].initBLOB(blob, parType);
			//cleanTmpData();
			return *this;
		}

		/// @brief Binds a Ref<TREF> typed reference holder to the next parameter position.
		/// @tparam TREF The referenced C++ value type.
		/// @param refHolder The Ref wrapper to bind.
		/// @return Reference to this Params object for chaining.
		template<class TREF>
		Params& operator << (Ref<TREF>& refHolder)
		{
			_params[iParPos++].initRef(refHolder, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

		/// @brief Binds a const DBString wrapper to the next parameter position.
		/// @tparam T The underlying string type.
		/// @param val Const reference to the DBString wrapper to bind.
		/// @return Reference to this Params object for chaining.
        template <typename T>
        Params& operator << (const dp::DBString<T>& val)
        {
            const T* ptrCVal = val.getValuePtr();
            T* ptrVal = const_cast<T*>(ptrCVal);
            _params[iParPos++].init(ptrVal, parType, val.getLength(), val.getType()); //, parType, parPrecision, databaseType);
            cleanTmpData();
            return *this;
        }

		/// @brief Binds a mutable DBString wrapper to the next parameter position.
		/// @tparam T The underlying string type.
		/// @param val Reference to the DBString wrapper to bind.
		/// @return Reference to this Params object for chaining.
        template <typename T>
        Params& operator << (dp::DBString<T>& val)
        {
            T* ptrVal = val.getValuePtr();
            _params[iParPos++].init(ptrVal, parType, val.getLength(), val.getType()); //, parType, parPrecision, databaseType);
            cleanTmpData();
            return *this;
        }

		/// @brief Binds a td::VariantDesc descriptor to the next parameter position,
		///        applying string-type handling when the output type is a character type.
		/// @param var Reference to the VariantDesc to bind.
		/// @return Reference to this Params object for chaining.
		Params& operator << (td::VariantDesc& var)
		{
			td::DataType dt = var.outType;
			if (dt >= td::ch && dt <= td::nchFix)
			{
				dp::DBString<td::String> dbStr(var.strVal(), dt, (td::WORD) var.outLen);
				*this << dbStr;
			}
			else
				*this << (td::Variant&)(var);

			return *this;
		}

		/// @brief Binds a SmallChFix<NCH> fixed-character value to the next parameter position.
		/// @tparam NCH The fixed character count of the SmallChFix type.
		/// @param val Reference to the fixed-character value to bind.
		/// @return Reference to this Params object for chaining.
		template <size_t NCH>
		Params& operator << (td::SmallChFix<NCH>& val)
		{
			_params[iParPos++].init(val, parType);
			cleanTmpData();
			return *this;
		}

		/// @brief Binds a td::Variant value to the next parameter position,
		///        applying string and fixed-char type-specific handling.
		/// @param val Reference to the variant value to bind.
		/// @return Reference to this Params object for chaining.
		Params& operator << (td::Variant& val)
		{
			td::DataType dt = val.getType();
			td::DataType dbType = val.getDBType();
			if (dt == td::string8)
			{
				assert(dbType >= td::ch && dt <= td::nchFix);
				td::WORD dbLen = val.getDBLen();
				assert(dbLen > 0);
				dp::DBString<td::String> dbStr(val.strVal(), dbType, dbLen);
				*this << dbStr;
				return *this;
			}
			else if (dt == td::ch7)
			{
				td::SmallChFix<7>& chFixVal = val.chFixVal();
				td::WORD dbLen = val.getDBLen();
				assert(dbLen > 0 && dbLen < 8);
				_params[iParPos++].init(chFixVal, dbLen, dbType, parType);
				cleanTmpData();
				return *this;
			}
			_params[iParPos++].init(val, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

		/// @brief Generic streaming operator that binds any supported value type to the next parameter position.
		/// @tparam T The C++ value type to bind.
		/// @param val Reference to the value to bind.
		/// @return Reference to this Params object for chaining.
		template <typename T>
		Params& operator << (T& val)
		{
			_params[iParPos++].init(val, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

		/// @brief Returns the total number of parameters in the underlying vector.
		/// @return Parameter count.
		size_t size() const
		{
			return _params.size();
		}
		//template <typename T>
		//Params& operator << (PAR<T>& parVal)
		//{
		//	_params[iParPos++].init(parVal.getVal(), parType, parPrecision, databaseType);
		//	cleanTmpData();
		//	return *this;
		//}
	};
}
