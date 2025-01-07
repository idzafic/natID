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
#include <td/VariantBase.h>
#include <td/String.h>


namespace td
{	
	template <typename TVAR>
	struct BaseToVariantVisitor
	{
		const TVAR& vb;
		BaseToVariantVisitor(const TVAR& vbIn) : vb(vbIn){}
		void operator()(none) const {}
		void operator()(td::BoolCh& val) const { val = (vb._bVal == 'Y'); }
		void operator()(td::BYTE& val) const { val = vb._byteVal; }
		void operator()(td::INT2& val) const { val = vb._i2Val; }
		void operator()(td::WORD& val) const { val = vb._wordVal; }
		void operator()(td::INT4& val) const { val = vb._i4Val; }
		void operator()(td::UINT4& val) const { val = vb._u4Val; }
        void operator()(td::Color& val) const { val = vb._colorVal; }
        void operator()(td::ColorID& val) const { val = vb._colorID; }
        void operator()(td::LinePattern& val) const { val = vb._linePattern; }
		void operator()(td::DotPattern& val) const { val = vb._dotPattern; }
		void operator()(td::LINT8& val) const { val = vb._li8Val; }
		void operator()(td::LUINT8& val) const { val = vb._lu8Val; }
		void operator()(float& val) const { val = vb._r4Val; }
		void operator()(double& val) const { val = vb._r8Val; }
		void operator()(td::String& val) const 
		{ 
			const td::String& strIn = (td::String&) (vb._strVal);
			val = strIn;
			//int g = 5;
		}
		void operator()(td::Date& val) const { val = (const td::Date&) (vb._dateVal); }
		void operator()(td::Time& val) const { val = (const td::Time&) (vb._timeVal); }
		void operator()(td::DateTime& val) const 
		{ 
			val = (const td::DateTime&) vb._dtVal; 
		}
		template< int NDEC >
		void operator()(td::Decimal<td::LINT8, NDEC>& val) const
		{			
			val.setValue(vb._decVal);
		}
		template< int NDEC >
		void operator()(td::Decimal<td::INT4, NDEC>& val) const
		{
			val.setValue(vb._smallDecVal);
		}

		void operator()(td::ChFix7& val) const
		{
			val = (const char*) &vb._chVal[0];
			//val.setValue(vb._smallDecVal);
		}
	};

	template <typename VAR>
	struct EqualityVisitor
	{
		const VAR& vb;
		EqualityVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(td::BoolCh& val) const { return val == vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val == vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val == vb.i2Val(); }
		bool operator()(td::WORD val) const { return val == vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val == vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val == vb.u4Val(); }
        bool operator()(td::Color& val) const { return val == vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val == vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val == vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val == vb.dotPattern(); }
		bool operator()(td::LINT8 val) const { return val == vb.li8Val(); }
		bool operator()(td::LUINT8 val) const { return val == vb.lu8Val(); }
		bool operator()(float val) const { return val == vb.r4Val(); }
		bool operator()(double val) const { return val == vb.r8Val(); }
		bool operator()(const td::String& val) const { return val == vb.strVal(); }
		bool operator()(const td::Date& val) const { return val == vb.dateVal(); }
		bool operator()(const td::Time& val) const { return val == vb.timeVal(); }
		bool operator()(const td::DateTime& val) const { return val == vb.dtVal(); }
		bool operator()(const td::Decimal0& val) const { return val == vb.dec0Val(); }
		bool operator()(const td::Decimal1& val) const { return val == vb.dec1Val(); }
		bool operator()(const td::Decimal2& val) const { return val == vb.dec2Val(); }
		bool operator()(const td::Decimal3& val) const { return val == vb.dec3Val(); }
		bool operator()(const td::Decimal4& val) const { return val == vb.dec4Val(); }
		bool operator()(const td::SmallDecimal0& val) const { return val == vb.sdec0Val(); }
		bool operator()(const td::SmallDecimal1& val) const { return val == vb.sdec1Val(); }
		bool operator()(const td::SmallDecimal2& val) const { return val == vb.sdec2Val(); }
		bool operator()(const td::SmallDecimal3& val) const { return val == vb.sdec3Val(); }
		bool operator()(const td::SmallDecimal4& val) const { return val == vb.sdec4Val(); }
		bool operator()(const td::ChFix7& val) const { return val == vb.chFixVal(); }
	};

	template <typename VAR>
	struct LessThanVisitor
	{
		const VAR& vb;
		LessThanVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(td::BoolCh&  val) const { return val < vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val < vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val <vb.i2Val(); }
		bool operator()(td::WORD val) const { return val < vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val < vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val < vb.u4Val(); }
        bool operator()(td::Color& val) const { return val < vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val < vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val < vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val < vb.dotPattern(); }
		bool operator()(td::LINT8 val) const { return val < vb.li8Val(); }
		bool operator()(td::LUINT8 val) const { return val < vb.lu8Val(); }
		bool operator()(float val) const { return val < vb.r4Val(); }
		bool operator()(double val) const { return val < vb.r8Val(); }
		bool operator()(const td::String& val) const { return val < vb.strVal(); }
		bool operator()(const td::Date& val) const { return val < vb.dateVal(); }
		bool operator()(const td::Time& val) const { return val < vb.timeVal(); }
		bool operator()(const td::DateTime& val) const { return val < vb.dtVal(); }
		bool operator()(const td::Decimal0& val) const { return val < vb.dec0Val(); }
		bool operator()(const td::Decimal1& val) const { return val < vb.dec1Val(); }
		bool operator()(const td::Decimal2& val) const { return val < vb.dec2Val(); }
		bool operator()(const td::Decimal3& val) const { return val < vb.dec3Val(); }
		bool operator()(const td::Decimal4& val) const { return val < vb.dec4Val(); }
		bool operator()(const td::SmallDecimal0& val) const { return val < vb.sdec0Val(); }
		bool operator()(const td::SmallDecimal1& val) const { return val < vb.sdec1Val(); }
		bool operator()(const td::SmallDecimal2& val) const { return val < vb.sdec2Val(); }
		bool operator()(const td::SmallDecimal3& val) const { return val < vb.sdec3Val(); }
		bool operator()(const td::SmallDecimal4& val) const { return val < vb.sdec4Val(); }
		bool operator()(const td::ChFix7& val) const { return val < vb.chFixVal(); }
	};

	template <typename VAR>
	struct LessEqualThanVisitor
	{
		const VAR& vb;
		LessEqualThanVisitor(const VAR& vIn) : vb(vIn) {}
		bool operator()(none) const { return false; }
		bool operator()(td::BoolCh&  val) const { return val <= vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val <= vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val <= vb.i2Val(); }
		bool operator()(td::WORD val) const { return val <= vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val <= vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val <= vb.u4Val(); }
        bool operator()(td::Color& val) const { return val <= vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val <= vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val <= vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val <= vb.dotPattern(); }
		bool operator()(td::LINT8 val) const { return val <= vb.li8Val(); }
		bool operator()(td::LUINT8 val) const { return val <= vb.lu8Val(); }
		bool operator()(float val) const { return val <= vb.r4Val(); }
		bool operator()(double val) const { return val <= vb.r8Val(); }
		bool operator()(const td::String& val) const { return val <= vb.strVal(); }
		bool operator()(const td::Date& val) const { return val <= vb.dateVal(); }
		bool operator()(const td::Time& val) const { return val <= vb.timeVal(); }
		bool operator()(const td::DateTime& val) const { return val <= vb.dtVal(); }
		bool operator()(const td::Decimal0& val) const { return val <= vb.dec0Val(); }
		bool operator()(const td::Decimal1& val) const { return val <= vb.dec1Val(); }
		bool operator()(const td::Decimal2& val) const { return val <= vb.dec2Val(); }
		bool operator()(const td::Decimal3& val) const { return val <= vb.dec3Val(); }
		bool operator()(const td::Decimal4& val) const { return val <= vb.dec4Val(); }
		bool operator()(const td::SmallDecimal0& val) const { return val <= vb.sdec0Val(); }
		bool operator()(const td::SmallDecimal1& val) const { return val <= vb.sdec1Val(); }
		bool operator()(const td::SmallDecimal2& val) const { return val <= vb.sdec2Val(); }
		bool operator()(const td::SmallDecimal3& val) const { return val <= vb.sdec3Val(); }
		bool operator()(const td::SmallDecimal4& val) const { return val <= vb.sdec4Val(); }
		bool operator()(const td::ChFix7& val) const { return val <= vb.chFixVal(); }
	};


	template <typename VAR>
	struct GreatherThanVisitor
	{
		const VAR& vb;
		GreatherThanVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(td::BoolCh& val) const { return val > vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val > vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val > vb.i2Val(); }
		bool operator()(td::WORD val) const { return val > vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val > vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val > vb.u4Val(); }
        bool operator()(td::Color& val) const { return val > vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val > vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val > vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val > vb.dotPattern(); }
		bool operator()(td::LINT8 val) const { return val > vb.li8Val(); }
		bool operator()(td::LUINT8 val) const { return val > vb.lu8Val(); }
		bool operator()(float val) const { return val > vb.r4Val(); }
		bool operator()(double val) const { return val > vb.r8Val(); }
		bool operator()(const td::String& val) const { return val > vb.strVal(); }
		bool operator()(const td::Date& val) const { return val > vb.dateVal(); }
		bool operator()(const td::Time& val) const { return val > vb.timeVal(); }
		bool operator()(const td::DateTime& val) const { return val > vb.dtVal(); }
		bool operator()(const td::Decimal0& val) const { return val > vb.dec0Val(); }
		bool operator()(const td::Decimal1& val) const { return val > vb.dec1Val(); }
		bool operator()(const td::Decimal2& val) const { return val > vb.dec2Val(); }
		bool operator()(const td::Decimal3& val) const { return val > vb.dec3Val(); }
		bool operator()(const td::Decimal4& val) const { return val > vb.dec4Val(); }
		bool operator()(const td::SmallDecimal0& val) const { return val > vb.sdec0Val(); }
		bool operator()(const td::SmallDecimal1& val) const { return val > vb.sdec1Val(); }
		bool operator()(const td::SmallDecimal2& val) const { return val > vb.sdec2Val(); }
		bool operator()(const td::SmallDecimal3& val) const { return val > vb.sdec3Val(); }
		bool operator()(const td::SmallDecimal4& val) const { return val > vb.sdec4Val(); }
		bool operator()(const td::ChFix7& val) const { return val > vb.chFixVal(); }
	};

	template <typename VAR>
	struct GreatherEqualThanVisitor
	{
		const VAR& vb;
		GreatherEqualThanVisitor(const VAR& vIn) : vb(vIn) {}
		bool operator()(none) const { return false; }
		bool operator()(td::BoolCh& val) const { return val >= vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val >= vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val >= vb.i2Val(); }
		bool operator()(td::WORD val) const { return val >= vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val >= vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val >= vb.u4Val(); }
        bool operator()(td::Color& val) const { return val >= vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val >= vb.colorID();}
        bool operator()(td::LinePattern val) const { return val >= vb.linePattern();}
		bool operator()(td::DotPattern val) const { return val >= vb.dotPattern();}
		bool operator()(td::LINT8 val) const { return val >= vb.li8Val(); }
		bool operator()(td::LUINT8 val) const { return val >= vb.lu8Val(); }
		bool operator()(float val) const { return val >= vb.r4Val(); }
		bool operator()(double val) const { return val >= vb.r8Val(); }
		bool operator()(const td::String& val) const { return val >= vb.strVal(); }
		bool operator()(const td::Date& val) const { return val >= vb.dateVal(); }
		bool operator()(const td::Time& val) const { return val >= vb.timeVal(); }
		bool operator()(const td::DateTime& val) const { return val >= vb.dtVal(); }
		bool operator()(const td::Decimal0& val) const { return val >= vb.dec0Val(); }
		bool operator()(const td::Decimal1& val) const { return val >= vb.dec1Val(); }
		bool operator()(const td::Decimal2& val) const { return val >= vb.dec2Val(); }
		bool operator()(const td::Decimal3& val) const { return val >= vb.dec3Val(); }
		bool operator()(const td::Decimal4& val) const { return val >= vb.dec4Val(); }
		bool operator()(const td::SmallDecimal0& val) const { return val >= vb.sdec0Val(); }
		bool operator()(const td::SmallDecimal1& val) const { return val >= vb.sdec1Val(); }
		bool operator()(const td::SmallDecimal2& val) const { return val >= vb.sdec2Val(); }
		bool operator()(const td::SmallDecimal3& val) const { return val >= vb.sdec3Val(); }
		bool operator()(const td::SmallDecimal4& val) const { return val >= vb.sdec4Val(); }
		bool operator()(const td::ChFix7& val) const { return val >= vb.chFixVal(); }
	};

	
	struct IsZeroVisitor
	{		
		IsZeroVisitor(){}
		bool operator()(none) const { return false; }
		bool operator()(bool val) const { return val; }
		template <typename T>
		bool operator()(T val) const { return val == 0; }		
		bool operator()(const td::String& val) const { return val.length() == 0; }
		bool operator()(const td::DateTime& val) const { return (val.getDate() == 0 && val.getTime() == 0); }
		bool operator()(const td::ChFix7& val) const { return val.isZero(); }
		bool operator()(const td::Color& val) const { return val.isZero(); }
        bool operator()(td::ColorID val) const { return val == td::ColorID::Black; }
        bool operator()(td::LinePattern val) const { return val == td::LinePattern::Solid; }
		bool operator()(td::DotPattern val) const { return val == td::DotPattern::Pixel; }
	};


	template <typename TUTIL>
	struct ToCStrVisitor
	{
		TUTIL& util;
		ToCStrVisitor(TUTIL& utilIn) : util(utilIn){}
		const char* operator()(none) const { return "TD_NONE"; }
		const char* operator()(const td::String& val) const { return val.c_str(); }
		const char* operator()(const td::BoolCh& val) const { return util.c_str(val()); }
		template <typename TVAL>
		const char* operator()(const TVAL& val) const { return util.c_str(val); }
		const char* operator()(const td::ChFix7& val) const { return val.c_str(); }
	};

	template <typename TINTEGER>
	struct SetToIntegerVisitor
	{
		TINTEGER& intVal;
		SetToIntegerVisitor(TINTEGER& intValIn) : intVal(intValIn){}
		void operator()(none) const { assert(false); }
		void operator()(td::BoolCh& val) const { val = (intVal != 0); }
		void operator()(td::String&) const { assert(false); }
		void operator()(td::ChFix7&) const { assert(false); }
		void operator()(td::Date& ) const { assert(false); }
		void operator()(td::Time& ) const { assert(false); }
		void operator()(td::DateTime&) const { assert(false); }
		template <typename TVAL>
		void operator()(TVAL& val) const { val = (TVAL) intVal; }		
	};

	struct FromStringVisitor
	{			
		const char* pStr;
		FromStringVisitor(const char* pStrIn) : pStr(pStrIn){}

		void operator()(none) const { }
		void operator() (td::BoolCh& val) const { val = td::isTrue(pStr); }
		void operator()(td::BYTE& val) const { val = (td::BYTE) atoi(pStr); }
		void operator()(td::INT2& val) const { val = (td::INT2) atoi(pStr); }
		void operator()(td::WORD& val) const { val = (td::WORD) atoi(pStr); }
		void operator()(td::INT4& val) const { val = (td::INT4) atoi(pStr); }
		void operator()(td::UINT4& val) const { val = (td::UINT4) atoi(pStr); }
		void operator()(td::LINT8& val) const 
		{ 
			char *pStop = 0;
			val = CSTR_TO_LINT8(pStr, &pStop, 10);	
		}
		void operator()(td::LUINT8& val) const 
		{ 
			char *pStop = 0;
			val = CSTR_TO_LUINT8(pStr, &pStop, 10);
		}
		void operator()(float& val) const { val = (float) atof(pStr); }
		void operator()(double& val) const { val = atof(pStr); }
		void operator() (td::String& val) const { val = pStr; }
		void operator() (td::ChFix7& val) const { val = pStr; }
        void operator() (td::ColorID& val) const { val = td::toColorID(pStr); }
        void operator() (td::LinePattern& val) const { val = td::toLinePattern(pStr); }
        void operator() (td::DotPattern& val) const { val = td::toDotPattern(pStr); }

		template <typename TVAL>
		void operator()(TVAL& val) const { val.fromString(pStr); }
	};

	template <typename TVAL>
	struct FromNumericVisitor
	{
		const TVAL& _inVal;
		
		FromNumericVisitor(const TVAL& inVal) : _inVal(inVal){}

		void operator()(none) const { }
		void operator() (td::BoolCh& val) const { val = ((td::LUINT8)_inVal != 0); }
		void operator()(td::BYTE& val) const { val = (td::BYTE) _inVal; }
		void operator()(td::INT2& val) const { val = (td::INT2) _inVal; }
		void operator()(td::WORD& val) const { val = (td::WORD) _inVal; }
		void operator()(td::INT4& val) const { val = (td::INT4) _inVal; }
		void operator()(td::UINT4& val) const { val = (td::UINT4) _inVal; }
		void operator()(td::LINT8& val) const { val = (td::LINT8) _inVal; }
		void operator()(td::LUINT8& val) const { val = (td::LUINT8) _inVal; }		
		void operator()(float& val) const { val = (float)_inVal; }
		void operator()(double& val) const { val = (double) _inVal; }
        void operator()(td::Date& val) const
        {
            td::Date tmp( (td::INT4) _inVal);
            val = tmp;
        }
        void operator()(td::Time& val) const
        {
            td::Time tmp( (td::INT4) _inVal);
            val = tmp;
        }
		void operator() (td::String& val) const 
		{ 			
			val.fromNumber(_inVal); 
		}

		void operator() (td::ChFix7& val) const 
		{ 			
			td::String str;
			str.fromNumber(_inVal);
			val = str.c_str(); 
		}

		template <typename TOTHERVAL>
		void operator()(TOTHERVAL& ) const {  }
	};

	template <typename TSTR>
	struct ToStreamVisitor
	{
		TSTR& s;
		ToStreamVisitor(TSTR& sIn) : s(sIn){}
		TSTR& operator()(none) const { return s; }
		
		TSTR& operator()(const td::BoolCh& val) const 
		{  
			bool bVal = val();
			s << bVal;
			return s;
		}
        
        TSTR& operator()(td::ColorID val) const
        {
            td::String strVal(td::toString(val));
            s << strVal;
            return s;
        }
        
        TSTR& operator()(td::LinePattern val) const
        {
            td::String strVal(td::toString(val));
            s << strVal;
            return s;
        }
        
        TSTR& operator()(td::DotPattern val) const
        {
            td::String strVal(td::toString(val));
            s << strVal;
            return s;
        }

		template <typename TVAL>
		TSTR& operator()(const TVAL& val) const 
		{ 
			s << val; 
			return s; 
		}
	};

	template <typename TSTR>
	struct FromStreamVisitor
	{
		TSTR& s;
		FromStreamVisitor(TSTR& sIn) : s(sIn){}
		TSTR& operator()(none) const { return s; }

		TSTR& operator()(td::BoolCh& val) const
		{
			bool inVal = false;
			s >> inVal;
			val = inVal;
			return s;
		}

		template <typename TVAL>
		TSTR& operator()(TVAL& val) const
		{
			s >> val;
			return s;
		}
	};	
	
	template <typename TREG>
	struct FormatCStrVisitor
	{
		TREG* pReg;
		int nDec;
		int showThousendSep;
        td::FormatFloat floatFormat = td::FormatFloat::Decimal;
		FormatCStrVisitor(TREG* reg, int nDecOrDateFmt, int showThSepOrTimeFmt, td::FormatFloat floatFormatIn = td::FormatFloat::Decimal)
        : pReg(reg), nDec(nDecOrDateFmt), showThousendSep(showThSepOrTimeFmt), floatFormat(floatFormatIn)
        {}
        
		const char* operator()(none) const { return "TD_NONE"; }
		const char* operator()(td::BoolCh& val) const { return td::c_str(val()); }
		const char* operator()(td::BYTE val) const
		{
			pReg->formatUInt(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::INT2 val) const 
		{
			pReg->formatInt(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::WORD val) const 
		{
			pReg->formatUInt(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::INT4 val) const 
		{
			pReg->formatInt(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::UINT4 val) const 
		{
			pReg->formatUInt(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::LINT8 val) const 
		{
			pReg->formatLINT(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(td::LUINT8 val) const 
		{
			pReg->formatLUINT(val, showThousendSep != 0);
			return pReg->c_str();
		}
		const char* operator()(float val) const 
		{
            const char* trail = nullptr;
            pReg->formatFloat(val, nDec, showThousendSep != 0, floatFormat, trail);
			return pReg->c_str();
		}
		const char* operator()(double val) const
		{
            const char* trail = nullptr;
            pReg->formatFloat(val, nDec, showThousendSep != 0, floatFormat, trail);
			return pReg->c_str();
		}
		const char* operator()(const td::String& val) const { return val.c_str(); }

		const char* operator()(const td::ChFix7& val) const { return val.c_str(); }

        const char* operator()(const td::Color& val) const
        {
            pReg->format(val);
            return pReg->c_str();
            //return val.c_str(pReg);
        }
        
        const char* operator()(td::ColorID val) const
        {
            return td::toString(val);
        }
        
        const char* operator()(td::LinePattern val) const
        {
            return td::toString(val);
        }
        
        const char* operator()(td::DotPattern val) const
        {
            return td::toString(val);
        }
        
		const char* operator()(const td::Date& val) const
		{
			td::Date::Format fmt = (td::Date::Format) nDec;
			pReg->format(fmt, val);			
			return pReg->c_str();
		}
		const char* operator()(const td::Time& val) const 
		{
			td::Time::Format fmt = (td::Time::Format) showThousendSep;
			pReg->format(fmt, val);			
			return pReg->c_str();
		}
		const char* operator()(const td::DateTime& val) const 
		{
			td::Date::Format dateFmt = (td::Date::Format) nDec;
			td::Time::Format timeFmt = (td::Time::Format) showThousendSep;
			pReg->format(dateFmt, timeFmt, val);
			return pReg->c_str();
		}		
		template<typename TVAL, int NDEC >
		const char* operator()(const td::Decimal<TVAL, NDEC>& val) const		
		{
			pReg->format(val, showThousendSep != 0);
			return pReg->c_str();
		}
		
	};

	template <typename TNUM>
	struct ToNumberVisitor
	{				

		inline void stringNumber(const td::String& str, double& val) const
		{
			val = atof(str.c_str());
		}

		inline void stringNumber(const td::String& str, float& val) const
		{
			val = atof(str.c_str());
		}

		template <typename TINTVAL>
		void stringToNumber(const td::String& str, TINTVAL& val) const
		{
			val = (TINTVAL) atoi(str.c_str());
		}

		TNUM operator()(none) const 
		{ 
			assert(false); 
			return (TNUM)0;
		}
		TNUM operator() (const td::String& str) const 
		{ 
			TNUM valOut = (TNUM)0;
			stringToNumber(str, valOut);
			return valOut;
		}

		TNUM operator()(const td::BoolCh& val) const 
		{ 
			if (val()) 
				return (TNUM)1;
			else return (TNUM) 0;
		}


		TNUM operator()(double val) const { return TNUM(val); }

		TNUM operator()(float val) const { return TNUM(val); }

		TNUM operator()(int val) const { return TNUM(val); }

		TNUM operator()(unsigned int val) const { return TNUM(val); }

		TNUM operator()(td::LUINT8 val) const { return TNUM(val); }

		TNUM operator()(td::LINT8 val) const { return TNUM(val); }

        TNUM operator()(const td::Color& val) const { return (TNUM)val.getValue(); }
        
        TNUM operator()(td::ColorID val) const { return (TNUM)val; }
        
		TNUM operator()(const td::Date& val) const { return (TNUM)val.getValue(); }
		TNUM operator()(const td::Time& val) const { return (TNUM)val.getValue(); }
		TNUM operator()(const td::DateTime& val) const { return (TNUM)val.getValue(); }

		template< typename TDEC, int NDEC >
		TNUM operator()(const td::Decimal<TDEC, NDEC>& val) const
		{
			return (TNUM)val.getAsFloat();
		}

		TNUM operator()(td::ChFix7&) const
		{
			return (TNUM)0;
		}

		template <typename TVAL>
		TNUM operator()(TVAL val) const { return TNUM(val); }
	};	

	struct CopyToVoidPtrVisitor
	{		
		void* ptr;		
		
		CopyToVoidPtrVisitor(void* pInPtr) : ptr(pInPtr) {}
		
		void operator()(none) const
		{
			assert(false);
		}

		template <typename TVAL>
		void operator() (const TVAL& val)
		{			
			TVAL* ptrTyped = (TVAL*)ptr;
			*ptrTyped = val;
		}
	};
	
	struct ToStringVisitor
	{		
		template <typename TVAL>
		inline td::String floatToNumber(TVAL val) const
		{
			char tmp[32];
			SNPRINTF(tmp, 32, _TRUNCATE, "%g", val);
			td::String str(&tmp[0]);
			return str;
		}
				
		inline td::String intToStr(td::INT4 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, "%d", val);
			td::String str(&tmp[0]);
			return str;
		}

		inline td::String intToStr(td::UINT4 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, "%u", val);
			td::String str(&tmp[0]);
			return str;
		}
		
		inline td::String intToStr(td::LINT8 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, FMT_LINT8, val);
			td::String str(&tmp[0]);
			return str;
		}

		inline td::String intToStr(td::LUINT8 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, FMT_LUINT8, val);
			td::String str(&tmp[0]);
			return str;
		}

		template <typename TINTVAL>
		void stringToNumber(const td::String& str, TINTVAL& val) const
		{
			val = (TINTVAL)atoi(str.c_str());
		}

		td::String operator()(none) const
		{
			assert(false);
			return td::String();
		}
		td::String operator() (const td::String& str) const
		{			
			return str;
		}

		td::String operator()(const td::BoolCh& val) const
		{			
			td::String str(val.c_str());
			return str;
		}
		
		td::String operator()(td::BYTE val) const { return intToStr((td::UINT4)val); }
		td::String operator()(td::UINT2 val) const { return intToStr((td::UINT4)val); }
		td::String operator()(td::UINT4 val) const { return intToStr((td::UINT4)val); }
		td::String operator()(td::INT1 val) const { return intToStr((td::INT4)val); }
		td::String operator()(td::INT2 val) const { return intToStr((td::INT4)val); }
		td::String operator()(td::INT4 val) const { return intToStr((td::INT4)val); }
		td::String operator()(float val) const { return floatToNumber(val); }
		td::String operator()(double val) const { return floatToNumber(val); }
		td::String operator()(td::LINT8 val) const { return intToStr(val); }
		td::String operator()(td::LUINT8 val) const { return intToStr(val); }		

		td::String operator()(const td::Date& val) const { return val.toString(); }
		td::String operator()(const td::Time& val) const { return val.toString(); }
		td::String operator()(const td::DateTime& val) const { return val.toString(); }

		template< typename TDEC, int NDEC >
		td::String operator()(const td::Decimal<TDEC, NDEC>& val) const
		{
			return val.toString();
		}

		td::String operator()(td::ChFix7& val) const
		{
			td::String str(val.c_str());
			return str;
		}
        
        td::String operator()(td::Color& val) const
        {
            return val.toString();
        }
        
        td::String operator()(td::ColorID val) const
        {
            return td::String(toString(val));
        }
        
        td::String operator()(td::LinePattern val) const
        {
            return td::String(toString(val));
        }
        
        td::String operator()(td::DotPattern val) const
        {
            return td::String(toString(val));
        }
	};
	
	template <typename TVAR>
	struct PlusEqVisitor
	{
		const TVAR& var;

		PlusEqVisitor(const TVAR& varIn) : var(varIn) {}
		void operator()(none)  const{ assert(false); }
		void operator() (td::String& str) const
		{
			str += var.strVal();
		}

		void operator() (td::Date&) const{}
		void operator() (td::Time&) const{}
		void operator() (td::DateTime&) const{}
		void operator()(td::ChFix7&) const	{}
        void operator() (td::Color&) const{}
        void operator() (td::ColorID) const{}
        void operator() (td::LinePattern) const{}
        void operator() (td::DotPattern) const{}

		void operator()(td::BoolCh& val) const
		{
			if (var.boolVal())
				val = true;
		}

		template <typename TVAL, int NDEC>
		void operator()(td::Decimal<TVAL, NDEC>& val) const
		{
			//TVAL x;
			assert(var.getType() == td::getType(val));
			const td::Decimal<TVAL, NDEC>& vToAdd = var.getVal(val);
			val += vToAdd;
		}

		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val += vToAdd;
		}
	};

	template <typename TVAR>
	struct MinusEqVisitor
	{
		const TVAR& var;

		MinusEqVisitor(const TVAR& varIn) : var(varIn) {}

		void operator()(none)  const{assert(false);	}
		void operator() (td::String&) const{}
		void operator() (td::Date&) const{}
		void operator() (td::Time&) const{}
		void operator() (td::DateTime&) const{}
		void operator()(td::BoolCh&) const{}
		void operator()(td::ChFix7&) const	{}
        void operator() (td::Color&) const{}
        void operator() (td::ColorID) const{}
        void operator() (td::LinePattern) const{}
        void operator() (td::DotPattern) const{}

		template <typename TVAL, int NDEC>
		void operator()(td::Decimal<TVAL, NDEC>& val) const
		{
			//TVAL x;
			assert(var.getType() == td::getType(val));
			const td::Decimal<TVAL, NDEC>& vToAdd = var.getVal(val);
			val -= vToAdd;
		}

		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val -= vToAdd;
		}
	};


	template <typename TVAR>
	struct MultEqVisitor
	{
		const TVAR& var;

		MultEqVisitor(const TVAR& varIn) : var(varIn) {}

		void operator()(none)  const{ assert(false); }
		void operator() (td::String&) const{}
		void operator() (td::Date&) const{}
		void operator() (td::Time&) const{}
		void operator() (td::DateTime&) const{}
		void operator()(td::BoolCh&) const{}
		void operator()(td::ChFix7&) const	{}
        void operator() (td::Color&) const{}
        void operator() (td::ColorID) const{}
        void operator() (td::LinePattern) const{}
        void operator() (td::DotPattern) const{}
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val *= vToAdd;
		}
	};

	template <typename TVAR>
	struct DivEqVisitor
	{
		const TVAR& var;

		DivEqVisitor(const TVAR& varIn) : var(varIn) {}

		void operator()(none)  const{ assert(false); }
		void operator() (td::String&) const{}
		void operator() (td::Date&) const{}
		void operator() (td::Time&) const{}
		void operator() (td::DateTime&) const{}
		void operator()(td::BoolCh&) const{}
		void operator()(td::ChFix7&) const	{}
        void operator() (td::Color&) const{}
        void operator() (td::ColorID) const{}
        void operator() (td::LinePattern) const{}
        void operator() (td::DotPattern) const{}

		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val /= vToAdd;
		}
	};

	struct AbsVisitor
	{
		void operator()(none)  const{ assert(false); }
		void operator() (td::String&) const{}
		void operator() (td::Date&) const{}
		void operator() (td::Time&) const{}
		void operator() (td::DateTime&) const{}
		void operator()(td::BoolCh&) const{}
		void operator()(td::ChFix7&) const	{}
        void operator() (td::Color&) const{}
        void operator() (td::ColorID) const{}
        void operator() (td::LinePattern) const{}
        void operator() (td::DotPattern) const{}
		template <typename TVAL>
		void operator()(TVAL& val) const
		{	
			if (val < 0)
				val *= -1;			
		}
	};

	template <typename TVAR>
	struct AssignVisitor
	{
		const TVAR& var;

		AssignVisitor(const TVAR& varIn) : var(varIn) {}

		void operator()(none)  const{ assert(false); }
		void operator() (td::String& str) const{ str = var.strVal(); }
		void operator() (td::Date& val) const { val = var.dateVal(); }
		void operator() (td::Time& val) const{ val = var.timeVal(); }
		void operator() (td::DateTime& val) const{ val = var.dtVal(); }
		void operator() (td::Color& val) const { val = var.colorVal(); }
        void operator() (td::ColorID& val) const { val = var.colorID(); }
        void operator() (td::LinePattern& val) const { val = var.linePattern(); }
		void operator()(td::BoolCh& val) const{ val = var.boolVal(); }
		void operator()(td::ChFix7& val) const	{ val = var.chFixVal(); }
		
		//void operator()(float& val) const 
		//{ 
		//	val = var.toNumber<float>(); 
		//}

		//void operator()(double& val) const
		//{
		//	val = var.toNumber<double>();
		//}
	
		template< typename THOLDER, int NDEC >
		void operator()(td::Decimal<THOLDER, NDEC>& val) const
		{
			val = var.template toNumber<double>();
		}

		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			val = var.template toNumber<TVAL>();
		}
	};
	
}

