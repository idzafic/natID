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

	class none
	{
	public:
		none(){};

	};
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
			char _chVal[8];
			//struct Bool	_bVal;
			char _bVal;
			BYTE _byteVal;
            td::ColorID _colorID;
            td::LinePattern _linePattern;
            td::DotPattern _dotPattern;
            td::Anchor _anchor;
			WORD _wordVal;
			INT2 _i2Val;
			UINT4 _u4Val;
			INT4 _i4Val;
			LUINT8 _lu8Val;
			LINT8 _li8Val;
			float _r4Val;
			double _r8Val;
			INT4 _dateVal;
            UINT4 _colorVal;
			INT4 _timeVal;
			LINT8 _dtVal;
			LINT8 _decVal;
			INT4 _smallDecVal; //za SmallDec			

			char* _strVal;	//pointer do dataHolder of td::String
			bool* _ptrBool;
			BYTE* _ptrByte;
			WORD* _ptrWord;
			INT2* _ptrI2;
			UINT4* _ptrU4;
			INT4* _ptrI4;
			LUINT8* _ptrLU8;
			LINT8* _ptrLI8;
			float* _ptrR4;
			double* _ptrR8;
			Date* _ptrDate;
			Time* _ptrTime;
			DateTime* _ptrDT;
			Decimal0* _ptrDec0;
			Decimal1* _ptrDec1;
			Decimal2* _ptrDec2;
			Decimal3* _ptrDec3;
			Decimal4* _ptrDec4;
			SmallDecimal0* _ptrSDec0;
			SmallDecimal1* _ptrSDec1;
			SmallDecimal2* _ptrSDec2;
			SmallDecimal3* _ptrSDec3;
			SmallDecimal4* _ptrSDec4;
			Variant* _ptrVar;
			void* _voidPtr;
		};

	protected:
		inline td::String& strValBase()
		{
			return (td::String&) _strVal;
		}
	public:
		VariantBase()
			: _lu8Val(0)
		{}

		VariantBase(bool val)
			: _lu8Val(0)
		{
			if (val)
				_bVal = 'Y';
			else
				_bVal = 'N';
		}
        
        VariantBase(void* pPtr)
        {
            _voidPtr = pPtr;
        }

		VariantBase(td::BYTE val)
			: _byteVal(val)			
		{}

		VariantBase(td::WORD val)
			: _wordVal(val)
		{}

		VariantBase(td::INT2 val)
			: _i2Val(val)
		{}

		VariantBase(td::INT4 val)
			: _i4Val(val)
		{}

		VariantBase(td::UINT4 val)
			: _u4Val(val)
		{}

		VariantBase(td::LUINT8 val)
			: _lu8Val(val)		
		{}

		VariantBase(td::LINT8 val)
			: _li8Val(val)
		{}

		VariantBase(float val)
			: _r4Val(val)
		{}

		VariantBase(double val)
			: _r8Val(val)
		{}

		VariantBase(td::Date val)
			: _dateVal(val.getValue())
		{}

		VariantBase(td::Time val)
			: _timeVal(val.getValue()) 			
		{}

		VariantBase(const td::DateTime& val)
			: _dtVal(val.getValue())			
		{}


		VariantBase(const td::String& val)
			: _lu8Val(0)
		{
			strValBase() = val;
		}
        
        VariantBase(const td::Color& val)
            : _colorVal(val.getValue())
        {
        }
        
        VariantBase(const td::ColorID& val)
            : _colorID(val)
        {
        }
        
        VariantBase(const td::LinePattern& val)
            : _linePattern(val)
        {
        }
        
        VariantBase(const td::DotPattern& val)
            : _dotPattern(val)
        {
        }
        
        VariantBase(const td::Anchor& val)
            : _anchor(val)
        {
        }
        
        //template<size_t N >
        //VariantBase(const td::SmallChFix<N>& val)
        //    : _lu8Val(val.lu8Val())
        //{
        //}

		template<int NDEC >
		VariantBase(const Decimal<td::LINT8, NDEC>& val)
			: _decVal(val.getValue())			
		{}


		template<int NDEC >
		VariantBase(const Decimal<td::INT4, NDEC>& val)
			: _smallDecVal(val.getValue())
		{}


		template<int NCH >
		VariantBase(const SmallChFix<NCH>& val)	
			: _lu8Val(val.lu8Val())
		{
			//memcpy(&_chVal[0], val.c_str(), 8);
		}


		VariantBase(const VariantBase& var)
			: _lu8Val(var._lu8Val)
		{}


		void clean(td::DataType dataType)
		{
			if (dataType == td::string8)
			{
				td::String& str = (td::String&) _strVal;
				str.clean();
			}
			else
				_lu8Val = 0;
		}

		template <typename TVIS, typename TRET>
		inline TRET applyVisitor(TVIS& vis, td::DataType type)
		{
			switch (type)
			{
			case td::boolean: return vis((td::BoolCh&) _bVal); break;
			case td::byte:	return vis(_byteVal); break;
			case td::word:	return vis(_wordVal); break;
			case td::int2:	return vis(_i2Val); break;
			case td::uint4:	return vis(_u4Val); break;
			case td::int4:	return vis(_i4Val); break;
			case td::lint8: return vis(_li8Val); break;
			case td::luint8: return vis(_lu8Val); break;
			case td::real4:	return vis(_r4Val); break;
			case td::real8:	return vis(_r8Val); break;
			case td::string8: return vis((td::String&) _strVal); break;
			case td::ch7: return vis((td::ChFix7&) _chVal); break;
			case td::date:	return vis((td::Date&)_dateVal); break;
			case td::time:	return vis((td::Time&)_timeVal); break;
			case td::dateTime: return vis((td::DateTime&)_dtVal); break;
			case td::decimal0: return vis((td::Decimal0&)_decVal); break;
			case td::decimal1: return vis((td::Decimal1&)_decVal); break;
			case td::decimal2: return vis((td::Decimal2&)_decVal); break;
			case td::decimal3: return vis((td::Decimal3&)_decVal); break;
			case td::decimal4: return vis((td::Decimal4&)_decVal); break;
			case td::smallDecimal0:	return vis((td::SmallDecimal0&)_smallDecVal); break;
			case td::smallDecimal1:	return vis((td::SmallDecimal1&)_smallDecVal); break;
			case td::smallDecimal2:	return vis((td::SmallDecimal2&)_smallDecVal); break;
			case td::smallDecimal3:	return vis((td::SmallDecimal3&)_smallDecVal); break;
			case td::smallDecimal4:	return vis((td::SmallDecimal4&)_smallDecVal); break;
            case td::color: return vis((td::Color&)_colorVal); break;
            case td::colorID: return vis(_colorID); break;
            case td::linePattern: return vis(_linePattern); break;
            case td::dotPattern: return vis(_dotPattern); break;
            case td::anchor: return vis(_anchor); break;
			case td::TD_NONE: return vis(none()); break;
				//case td::none:	return "NONE-VALUE";
			default: assert(false);
			}
			return vis(none());
		}

		template <typename TVIS, typename TRET>
		inline TRET applyConstVisitor(TVIS& vis, td::DataType type) const
		{
			switch (type)
			{
			case td::boolean: return vis((td::BoolCh&) _bVal); break;
			case td::byte:	return vis(_byteVal); break;
			case td::word:	return vis(_wordVal); break;
			case td::int2:	return vis(_i2Val); break;
			case td::uint4:	return vis(_u4Val); break;
			case td::int4:	return vis(_i4Val); break;
			case td::lint8: return vis(_li8Val); break;
			case td::luint8: return vis(_lu8Val); break;
			case td::real4:	return vis(_r4Val); break;
			case td::real8:
                    return vis(_r8Val); break;
			case td::string8: return vis((td::String&) _strVal); break;
			case td::ch7: return vis((td::ChFix7&) _chVal); break;
			case td::date:	return vis((td::Date&)_dateVal); break;
			case td::time:	return vis((td::Time&)_timeVal); break;
			case td::dateTime: return vis((td::DateTime&)_dtVal); break;
			case td::decimal0: return vis((td::Decimal0&)_decVal); break;
			case td::decimal1: return vis((td::Decimal1&)_decVal); break;
			case td::decimal2: return vis((td::Decimal2&)_decVal); break;
			case td::decimal3: return vis((td::Decimal3&)_decVal); break;
			case td::decimal4: return vis((td::Decimal4&)_decVal); break;
			case td::smallDecimal0:	return vis((td::SmallDecimal0&)_smallDecVal); break;
			case td::smallDecimal1:	return vis((td::SmallDecimal1&)_smallDecVal); break;
			case td::smallDecimal2:	return vis((td::SmallDecimal2&)_smallDecVal); break;
			case td::smallDecimal3:	return vis((td::SmallDecimal3&)_smallDecVal); break;
			case td::smallDecimal4:	return vis((td::SmallDecimal4&)_smallDecVal); break;
            case td::color: return vis((td::Color&)_colorVal); break;
            case td::colorID: return vis(_colorID); break;
            case td::linePattern: return vis(_linePattern); break;
            case td::dotPattern: return vis(_dotPattern); break;
            case td::anchor: return vis(_anchor); break;
			case td::TD_NONE: return vis(none()); break;
			default: assert(false); 
			}
			return vis(none());
		}

		inline void operator = (const td::Variant& var);		
	};	
}
