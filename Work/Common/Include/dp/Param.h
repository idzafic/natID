// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/Column.h>

namespace dp
{
	class Param : public Column
	{
	public:
		enum class Type : unsigned char { INPUT = 1, OUTPUT};
        td::INT4 paramLength;
        td::INT4 maxLength;
        td::WORD precision;
        td::WORD scale;
		Type paramType;
		td::DataType dbType;
		
		Param() {}

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

		void initBLOB(td::BLOB& blob, Type parType = Param::Type::INPUT)
		{
			Column::init(blob);
			dbType = cppType;
			precision = 0;
			paramType = parType;
			maxLength = 0;
			paramLength = 0;
		}

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

	class Params
	{
	protected:
		cnt::SafeFullVector<Param>& _params;
		int iParPos;
		Param::Type parType;
		//td::WORD parPrecision;
		//td::DataType databaseType;

		void cleanTmpData()
		{
			parType = Param::Type::INPUT;
			//parPrecision = 0;
			//databaseType = td::nch;
		}

		Params& operator << (td::String& strVal)
		{
			_params[iParPos++].init(strVal, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

	public:
		Param& operator[] (int i) { return _params[i]; }
		Params(const Params& pars)
			: _params(pars._params)
            , parType (Param::Type::INPUT)
			, iParPos(0)
		{
			cleanTmpData();
		}

		Params(cnt::SafeFullVector<Param>& params)
			: _params(params)
			, iParPos(0)
		{
			cleanTmpData();
		}


		Params& operator << (Param::Type paramType)
		{
			parType = paramType;
			return *this;
		}

		Params& operator << (td::BLOB& blob)
		{
			_params[iParPos++].initBLOB(blob, parType);
			//cleanTmpData();
			return *this;
		}

		template<class TREF>
		Params& operator << (Ref<TREF>& refHolder)
		{
			_params[iParPos++].initRef(refHolder, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

        template <typename T>
        Params& operator << (const dp::DBString<T>& val)
        {
            const T* ptrCVal = val.getValuePtr();
            T* ptrVal = const_cast<T*>(ptrCVal);
            _params[iParPos++].init(ptrVal, parType, val.getLength(), val.getType()); //, parType, parPrecision, databaseType);
            cleanTmpData();
            return *this;
        }
        
        template <typename T>
        Params& operator << (dp::DBString<T>& val)
        {
            T* ptrVal = val.getValuePtr();
            _params[iParPos++].init(ptrVal, parType, val.getLength(), val.getType()); //, parType, parPrecision, databaseType);
            cleanTmpData();
            return *this;
        }
        
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

		

		template <size_t NCH>
		Params& operator << (td::SmallChFix<NCH>& val)
		{
			_params[iParPos++].init(val, parType);
			cleanTmpData();
			return *this;
		}

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


		template <typename T>
		Params& operator << (T& val)
		{
			_params[iParPos++].init(val, parType); //, parType, parPrecision, databaseType);
			cleanTmpData();
			return *this;
		}

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
