// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VariantVisitors.h
    @brief Visitor structs implementing common operations (copy, compare, arithmetic, conversion) over all Variant value types. */
#pragma once
#include <td/Types.h>
#include <td/VariantBase.h>
#include <td/String.h>
#include <mu/mu.h>
#include <td/Conv.h>

namespace td
{
	/// @brief Visitor that copies all fields from a source VariantBase into the target value.
	/// @tparam TVAR Source variant type (must be derived from VariantBase).
	template <typename TVAR>
	struct BaseToVariantVisitor
	{
		const TVAR& vb; ///< Reference to the source variant base.
		/// @brief Construct with a reference to the source variant.
		/// @param vbIn Source variant to copy values from.
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
        void operator()(td::Anchor& val) const { val = vb._anchor; }
		void operator()(td::LINT8& val) const { val = vb._li8Val; }
		void operator()(td::LUINT8& val) const { val = vb._lu8Val; }
		void operator()(float& val) const { val = vb._r4Val; }
		void operator()(double& val) const { val = vb._r8Val; }
		void operator()(td::String& val) const
		{
			const td::String& strIn = reinterpret_cast<const td::String&>(vb._strVal);
			val = strIn;
			//int g = 5;
		}
		void operator()(td::Date& val) const { val = reinterpret_cast<const td::Date&>(vb._dateVal); }
		void operator()(td::Time& val) const { val = reinterpret_cast<const td::Time&>(vb._timeVal); }
		void operator()(td::DateTime& val) const
		{
			val = reinterpret_cast<const td::DateTime&>(vb._dtVal);
		}
		/// @brief Copy a LINT8-backed Decimal value from the source.
		/// @tparam NDEC Number of decimal places.
		/// @param val Destination Decimal value.
		template< int NDEC >
		void operator()(td::Decimal<td::LINT8, NDEC>& val) const
		{
			val.setValue(vb._decVal);
		}
		/// @brief Copy an INT4-backed Decimal value from the source.
		/// @tparam NDEC Number of decimal places.
		/// @param val Destination Decimal value.
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

	/// @brief Visitor that tests equality between the stored value and the corresponding field of a reference variant.
	/// @tparam VAR Variant type providing typed accessors (e.g., byteVal(), i4Val(), etc.).
	template <typename VAR>
	struct EqualityVisitor
	{
		const VAR& vb; ///< Reference variant to compare against.
		/// @brief Construct with a reference variant.
		/// @param vIn Reference variant to compare against.
		EqualityVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(const td::BoolCh& val) const { return val == vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val == vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val == vb.i2Val(); }
		bool operator()(td::WORD val) const { return val == vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val == vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val == vb.u4Val(); }
        bool operator()(const td::Color& val) const { return val == vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val == vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val == vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val == vb.dotPattern(); }
        bool operator()(td::Anchor val) const { return val == vb.anchor(); }
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

	/// @brief Visitor that tests strict less-than ordering between the stored value and a reference variant.
	/// @tparam VAR Variant type providing typed accessors.
	template <typename VAR>
	struct LessThanVisitor
	{
		const VAR& vb; ///< Reference variant to compare against.
		/// @brief Construct with a reference variant.
		/// @param vIn Reference variant to compare against.
		LessThanVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(const td::BoolCh&  val) const { return val < vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val < vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val <vb.i2Val(); }
		bool operator()(td::WORD val) const { return val < vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val < vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val < vb.u4Val(); }
        bool operator()(const td::Color& val) const { return val < vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val < vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val < vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val < vb.dotPattern(); }
        bool operator()(td::Anchor val) const { return val < vb.anchor(); }
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

	/// @brief Visitor that tests less-than-or-equal ordering between the stored value and a reference variant.
	/// @tparam VAR Variant type providing typed accessors.
	template <typename VAR>
	struct LessEqualThanVisitor
	{
		const VAR& vb; ///< Reference variant to compare against.
		/// @brief Construct with a reference variant.
		/// @param vIn Reference variant to compare against.
		LessEqualThanVisitor(const VAR& vIn) : vb(vIn) {}
		bool operator()(none) const { return false; }
		bool operator()(const td::BoolCh&  val) const { return val <= vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val <= vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val <= vb.i2Val(); }
		bool operator()(td::WORD val) const { return val <= vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val <= vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val <= vb.u4Val(); }
        bool operator()(const td::Color& val) const { return val <= vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val <= vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val <= vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val <= vb.dotPattern(); }
        bool operator()(td::Anchor val) const { return val <= vb.anchor(); }
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


	/// @brief Visitor that tests strict greater-than ordering between the stored value and a reference variant.
	/// @tparam VAR Variant type providing typed accessors.
	template <typename VAR>
	struct GreatherThanVisitor
	{
		const VAR& vb; ///< Reference variant to compare against.
		/// @brief Construct with a reference variant.
		/// @param vIn Reference variant to compare against.
		GreatherThanVisitor(const VAR& vIn) : vb(vIn){}
		bool operator()(none) const { return false; }
		bool operator()(const td::BoolCh& val) const { return val > vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val > vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val > vb.i2Val(); }
		bool operator()(td::WORD val) const { return val > vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val > vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val > vb.u4Val(); }
        bool operator()(const td::Color& val) const { return val > vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val > vb.colorID(); }
        bool operator()(td::LinePattern val) const { return val > vb.linePattern(); }
		bool operator()(td::DotPattern val) const { return val > vb.dotPattern(); }
        bool operator()(td::Anchor val) const { return val > vb.anchor(); }
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

	/// @brief Visitor that tests greater-than-or-equal ordering between the stored value and a reference variant.
	/// @tparam VAR Variant type providing typed accessors.
	template <typename VAR>
	struct GreatherEqualThanVisitor
	{
		const VAR& vb; ///< Reference variant to compare against.
		/// @brief Construct with a reference variant.
		/// @param vIn Reference variant to compare against.
		GreatherEqualThanVisitor(const VAR& vIn) : vb(vIn) {}
		bool operator()(none) const { return false; }
		bool operator()(const td::BoolCh& val) const { return val >= vb.boolVal(); }
		bool operator()(td::BYTE val) const { return val >= vb.byteVal(); }
		bool operator()(td::INT2 val) const { return val >= vb.i2Val(); }
		bool operator()(td::WORD val) const { return val >= vb.wordVal(); }
		bool operator()(td::INT4 val) const { return val >= vb.i4Val(); }
		bool operator()(td::UINT4 val) const { return val >= vb.u4Val(); }
        bool operator()(const td::Color& val) const { return val >= vb.colorVal(); }
        bool operator()(td::ColorID val) const { return val >= vb.colorID();}
        bool operator()(td::LinePattern val) const { return val >= vb.linePattern();}
		bool operator()(td::DotPattern val) const { return val >= vb.dotPattern();}
        bool operator()(td::Anchor val) const { return val >= vb.anchor();}
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


	/// @brief Visitor that tests whether the stored value is zero (or logically empty).
	struct IsZeroVisitor
	{
		/// @brief Default constructor.
		IsZeroVisitor(){}
		bool operator()(none) const { return false; }
		bool operator()(bool val) const { return val; }
		/// @brief Generic zero-test for arithmetic types.
		/// @tparam T Arithmetic type.
		/// @param val Value to test.
		/// @return true if val == 0.
		template <typename T>
		bool operator()(T val) const { return val == 0; }
		bool operator()(const td::String& val) const { return val.length() == 0; }
		bool operator()(const td::DateTime& val) const { return (val.getDate() == 0 && val.getTime() == 0); }
		bool operator()(const td::ChFix7& val) const { return val.isZero(); }
		bool operator()(const td::Color& val) const { return val.isZero(); }
        bool operator()(td::ColorID val) const { return val == td::ColorID::Black; }
        bool operator()(td::LinePattern val) const { return val == td::LinePattern::Solid; }
		bool operator()(td::DotPattern val) const { return val == td::DotPattern::Pixel; }
        bool operator()(td::Anchor val) const { return val == td::Anchor::Fixed; }
	};


	/// @brief Visitor that converts any stored value to a C string using a utility formatter.
	/// @tparam TUTIL Utility type with a c_str() method for each supported type.
	template <typename TUTIL>
	struct ToCStrVisitor
	{
		TUTIL& util; ///< Reference to the utility formatter.
		/// @brief Construct with a utility formatter reference.
		/// @param utilIn Utility formatter to use for conversion.
		ToCStrVisitor(TUTIL& utilIn) : util(utilIn){}
		const char* operator()(none) const { return "TD_NONE"; }
		const char* operator()(const td::String& val) const { return val.c_str(); }
		const char* operator()(const td::BoolCh& val) const { return util.c_str(val()); }
		/// @brief Generic conversion for arithmetic types via the utility formatter.
		/// @tparam TVAL Type of the value.
		/// @param val Value to convert.
		/// @return Null-terminated C string representation.
		template <typename TVAL>
		const char* operator()(const TVAL& val) const { return util.c_str(val); }
		const char* operator()(const td::ChFix7& val) const { return val.c_str(); }
	};

	/// @brief Visitor that sets a Variant's stored value from an integer, ignoring incompatible types.
	/// @tparam TINTEGER Integer type of the source value.
	template <typename TINTEGER>
	struct SetToIntegerVisitor
	{
		TINTEGER& intVal; ///< Reference to the integer source value.
		/// @brief Construct with a reference to the integer value.
		/// @param intValIn Integer value to use as the source.
		SetToIntegerVisitor(TINTEGER& intValIn) : intVal(intValIn){}
		void operator()(none) const { assert(false); }
		void operator()(td::BoolCh& val) const { val = (intVal != 0); }
		void operator()(td::String&) const { assert(false); }
		void operator()(td::ChFix7&) const { assert(false); }
		void operator()(td::Date& ) const { assert(false); }
		void operator()(td::Time& ) const { assert(false); }
		void operator()(td::DateTime&) const { assert(false); }
		/// @brief Generic integer assignment with a cast.
		/// @tparam TVAL Target type.
		/// @param val Destination value to set.
		template <typename TVAL>
		void operator()(TVAL& val) const { val = (TVAL) intVal; }
	};

	/// @brief Visitor that parses a C string and assigns the result to the stored value.
	struct FromStringVisitor
	{
		const char* pStr; ///< Null-terminated source string to parse.
		/// @brief Construct with a source string pointer.
		/// @param pStrIn Null-terminated string to parse.
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
		void operator()(float& val) const { val = (float) mu::toDouble(pStr); }
		void operator()(double& val) const { val = mu::toDouble(pStr); }
		void operator() (td::String& val) const { val = pStr; }
		void operator() (td::ChFix7& val) const { val = pStr; }
        void operator() (td::ColorID& val) const { val = td::toColorID(pStr); }
        void operator() (td::LinePattern& val) const { val = td::toLinePattern(pStr); }
        void operator() (td::DotPattern& val) const { val = td::toDotPattern(pStr); }
        void operator() (td::Anchor& val) const { val = td::toAnchor(pStr); }

		/// @brief Generic fallback that calls val.fromString(pStr) for composite types.
		/// @tparam TVAL Type with a fromString() method.
		/// @param val Destination value.
		template <typename TVAL>
		void operator()(TVAL& val) const { val.fromString(pStr); }
	};

	/// @brief Visitor that assigns a numeric value to the stored variant field with type conversion.
	/// @tparam TVAL Type of the numeric input value.
	template <typename TVAL>
	struct FromNumericVisitor
	{
		const TVAL& _inVal; ///< Reference to the numeric source value.

		/// @brief Construct with a reference to the numeric source value.
		/// @param inVal Numeric value to use as the source.
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
        
        void operator()(td::Decimal0& val) const
            {
                td::LINT8 scaledValue = (td::LINT8)(_inVal * 1.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::Decimal1& val) const
            {
                td::LINT8 scaledValue = (td::LINT8)(_inVal * 10.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::Decimal2& val) const
            {
                td::LINT8 scaledValue = (td::LINT8)(_inVal * 100.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::Decimal3& val) const
            {
                td::LINT8 scaledValue = (td::LINT8)(_inVal * 1000.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::Decimal4& val) const
            {
                td::LINT8 scaledValue = (td::LINT8)(_inVal * 10000.0 + 0.5);
                val.setValue(scaledValue);
            }

            // SMALL DECIMAL TYPES (INT4-backed)
            void operator()(td::SmallDecimal0& val) const
            {
                td::INT4 scaledValue = (td::INT4)(_inVal * 1.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::SmallDecimal1& val) const
            {
                td::INT4 scaledValue = (td::INT4)(_inVal * 10.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::SmallDecimal2& val) const
            {
                td::INT4 scaledValue = (td::INT4)(_inVal * 100.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::SmallDecimal3& val) const
            {
                td::INT4 scaledValue = (td::INT4)(_inVal * 1000.0 + 0.5);
                val.setValue(scaledValue);
            }
            
            void operator()(td::SmallDecimal4& val) const
            {
                td::INT4 scaledValue = (td::INT4)(_inVal * 10000.0 + 0.5);
                val.setValue(scaledValue);
            }

		/// @brief Fallback for types that are not convertible from this numeric type (no-op).
		/// @tparam TOTHERVAL Target type.
		template <typename TOTHERVAL>
		void operator()(TOTHERVAL& ) const {  }
	};

	/// @brief Visitor that writes the stored value to an output stream.
	/// @tparam TSTR Stream type supporting the << operator.
	template <typename TSTR>
	struct ToStreamVisitor
	{
		TSTR& s; ///< Reference to the destination output stream.
		/// @brief Construct with a reference to the output stream.
		/// @param sIn Destination output stream.
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

        TSTR& operator()(td::Anchor val) const
        {
            td::String strVal(td::toString(val));
            s << strVal;
            return s;
        }

		/// @brief Generic stream output for arithmetic and other streamable types.
		/// @tparam TVAL Type of the value.
		/// @param val Value to write.
		/// @return Reference to the stream after writing.
		template <typename TVAL>
		TSTR& operator()(const TVAL& val) const
		{
			s << val;
			return s;
		}
	};

	/// @brief Visitor that reads the stored value from an input stream.
	/// @tparam TSTR Stream type supporting the >> operator.
	template <typename TSTR>
	struct FromStreamVisitor
	{
		TSTR& s; ///< Reference to the source input stream.
		/// @brief Construct with a reference to the input stream.
		/// @param sIn Source input stream.
		FromStreamVisitor(TSTR& sIn) : s(sIn){}
		TSTR& operator()(none) const { return s; }

		TSTR& operator()(td::BoolCh& val) const
		{
			bool inVal = false;
			s >> inVal;
			val = inVal;
			return s;
		}

		/// @brief Generic stream input for arithmetic and other streamable types.
		/// @tparam TVAL Type of the value.
		/// @param val Destination value to populate.
		/// @return Reference to the stream after reading.
		template <typename TVAL>
		TSTR& operator()(TVAL& val) const
		{
			s >> val;
			return s;
		}
	};

	/// @brief Visitor that formats the stored value to a C string using locale-aware formatting.
	/// @tparam TREG Regionals/locale helper type providing format methods.
	template <typename TREG>
	struct FormatCStrVisitor
	{
		TREG* pReg;      ///< Pointer to the locale/format helper.
		int nDec;        ///< Number of decimal places or date format code.
		int showThousendSep; ///< Non-zero to show thousand separator; also used as time format code.
        td::FormatFloat floatFormat = td::FormatFloat::Decimal; ///< Float display format.
		/// @brief Construct with formatting parameters.
		/// @param reg Pointer to the locale/format helper.
		/// @param nDecOrDateFmt Number of decimal places or date format code.
		/// @param showThSepOrTimeFmt Thousand separator flag or time format code.
		/// @param floatFormatIn Float display format (Decimal or Scientific).
		FormatCStrVisitor(TREG* reg, int nDecOrDateFmt, int showThSepOrTimeFmt, td::FormatFloat floatFormatIn = td::FormatFloat::Decimal)
        : pReg(reg), nDec(nDecOrDateFmt), showThousendSep(showThSepOrTimeFmt), floatFormat(floatFormatIn)
        {}

		const char* operator()(none) const { return "TD_NONE"; }
		const char* operator()(const td::BoolCh& val) const { return td::c_str(val()); }
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

        const char* operator()(td::Anchor val) const
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
		/// @brief Format a Decimal value using the locale helper.
		/// @tparam TVAL Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Decimal value to format.
		/// @return Null-terminated formatted C string.
		template<typename TVAL, int NDEC >
		const char* operator()(const td::Decimal<TVAL, NDEC>& val) const
		{
			pReg->format(val, showThousendSep != 0);
			return pReg->c_str();
		}

	};

	/// @brief Visitor that converts the stored value to a numeric type TNUM.
	/// @tparam TNUM Target numeric type (e.g., double, td::INT4).
	template <typename TNUM>
	struct ToNumberVisitor
	{

		/// @brief Convert a string to a double for float output.
		/// @param str Source string.
		/// @param val Destination double value.
		inline void stringNumber(const td::String& str, double& val) const
		{
			val = mu::toDouble(str.c_str());
		}

		/// @brief Convert a string to a float for float output.
		/// @param str Source string.
		/// @param val Destination float value.
		inline void stringNumber(const td::String& str, float& val) const
		{
			val = float(mu::toDouble(str.c_str()));
		}

		/// @brief Convert a string to an integer type.
		/// @tparam TINTVAL Integer destination type.
		/// @param str Source string.
		/// @param val Destination integer value.
		template <typename TINTVAL>
		void stringToNumber(const td::String& str, TINTVAL& val) const
		{
			val = (TINTVAL) atoi(str.c_str());
		}

		/// @brief Return zero for the none type (asserts in debug).
		/// @return Zero cast to TNUM.
		TNUM operator()(none) const
		{
			assert(false);
			return (TNUM)0;
		}
		/// @brief Convert a string variant to TNUM via integer parsing.
		/// @param str Source td::String.
		/// @return Parsed numeric value.
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

		/// @brief Convert a Decimal value to TNUM via its float representation.
		/// @tparam TDEC Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Decimal value to convert.
		/// @return Numeric result cast to TNUM.
		template< typename TDEC, int NDEC >
		TNUM operator()(const td::Decimal<TDEC, NDEC>& val) const
		{
			return (TNUM)val.getAsFloat();
		}

		TNUM operator()(const td::ChFix7& val) const
		{
            TNUM result = td::toNumber<TNUM>(val.c_str());
			return result;
		}

		/// @brief Generic numeric conversion for remaining arithmetic types.
		/// @tparam TVAL Source arithmetic type.
		/// @param val Value to convert.
		/// @return Value cast to TNUM.
		template <typename TVAL>
		TNUM operator()(TVAL val) const { return TNUM(val); }
	};

	/// @brief Visitor that copies the stored value into an untyped void pointer destination.
	struct CopyToVoidPtrVisitor
	{
		void* ptr; ///< Destination void pointer.

		/// @brief Construct with a destination void pointer.
		/// @param pInPtr Pointer to the destination memory.
		CopyToVoidPtrVisitor(void* pInPtr) : ptr(pInPtr) {}

		/// @brief Assertion guard for the none (empty) type.
		void operator()(none) const
		{
			assert(false);
		}

		/// @brief Copy a typed value to the destination pointer with a cast.
		/// @tparam TVAL Type of the source value.
		/// @param val Source value to copy.
		template <typename TVAL>
		void operator() (const TVAL& val)
		{
			TVAL* ptrTyped = (TVAL*)ptr;
			*ptrTyped = val;
		}
	};

	/// @brief Visitor that converts the stored value to a td::String.
	struct ToStringVisitor
	{
		/// @brief Format a floating-point value to a td::String using %g.
		/// @tparam TVAL Float type (float or double).
		/// @param val Value to convert.
		/// @return td::String representation.
		template <typename TVAL>
		inline td::String floatToNumber(TVAL val) const
		{
			char tmp[32];
			SNPRINTF(tmp, 32, _TRUNCATE, "%g", val);
			td::String str(&tmp[0]);
			return str;
		}

		/// @brief Convert a signed 32-bit integer to a td::String.
		/// @param val INT4 value to convert.
		/// @return td::String representation.
		inline td::String intToStr(td::INT4 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, "%d", val);
			td::String str(&tmp[0]);
			return str;
		}

		/// @brief Convert an unsigned 32-bit integer to a td::String.
		/// @param val UINT4 value to convert.
		/// @return td::String representation.
		inline td::String intToStr(td::UINT4 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, "%u", val);
			td::String str(&tmp[0]);
			return str;
		}

		/// @brief Convert a signed 64-bit integer to a td::String.
		/// @param val LINT8 value to convert.
		/// @return td::String representation.
		inline td::String intToStr(td::LINT8 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, FMT_LINT8, val);
			td::String str(&tmp[0]);
			return str;
		}

		/// @brief Convert an unsigned 64-bit integer to a td::String.
		/// @param val LUINT8 value to convert.
		/// @return td::String representation.
		inline td::String intToStr(td::LUINT8 val) const
		{
			char tmp[32];
            SNPRINTF(tmp, 32, _TRUNCATE, FMT_LUINT8, val);
			td::String str(&tmp[0]);
			return str;
		}

		/// @brief Convert a string to an integer type (helper for string-to-number).
		/// @tparam TINTVAL Integer destination type.
		/// @param str Source string.
		/// @param val Destination integer value.
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

		/// @brief Convert a Decimal value to a td::String.
		/// @tparam TDEC Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Decimal value to convert.
		/// @return td::String representation.
		template< typename TDEC, int NDEC >
		td::String operator()(const td::Decimal<TDEC, NDEC>& val) const
		{
			return val.toString();
		}

		td::String operator()(const td::ChFix7& val) const
		{
			td::String str(val.c_str());
			return str;
		}

        td::String operator()(const td::Color& val) const
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

        td::String operator()(td::Anchor val) const
        {
            return td::String(toString(val));
        }
	};

	/// @brief Visitor that performs += on the stored value using the corresponding field from a reference variant.
	/// @tparam TVAR Variant type providing typed accessors.
	template <typename TVAR>
	struct PlusEqVisitor
	{
		const TVAR& var; ///< Reference variant providing the right-hand operand.

		/// @brief Construct with the right-hand variant reference.
		/// @param varIn Right-hand variant for addition.
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
        void operator() (td::Anchor) const{}

		void operator()(td::BoolCh& val) const
		{
			if (var.boolVal())
				val = true;
		}

		/// @brief Add a Decimal value from the reference variant.
		/// @tparam TVAL Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Left-hand Decimal to accumulate into.
		template <typename TVAL, int NDEC>
		void operator()(td::Decimal<TVAL, NDEC>& val) const
		{
			//TVAL x;
			assert(var.getType() == td::getType(val));
			const td::Decimal<TVAL, NDEC>& vToAdd = var.getVal(val);
			val += vToAdd;
		}

		/// @brief Add a generic arithmetic value from the reference variant.
		/// @tparam TVAL Arithmetic type.
		/// @param val Left-hand value to accumulate into.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val += vToAdd;
		}
	};

	/// @brief Visitor that performs -= on the stored value using the corresponding field from a reference variant.
	/// @tparam TVAR Variant type providing typed accessors.
	template <typename TVAR>
	struct MinusEqVisitor
	{
		const TVAR& var; ///< Reference variant providing the right-hand operand.

		/// @brief Construct with the right-hand variant reference.
		/// @param varIn Right-hand variant for subtraction.
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
        void operator() (td::Anchor) const{}

		/// @brief Subtract a Decimal value from the reference variant.
		/// @tparam TVAL Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Left-hand Decimal to subtract from.
		template <typename TVAL, int NDEC>
		void operator()(td::Decimal<TVAL, NDEC>& val) const
		{
			//TVAL x;
			assert(var.getType() == td::getType(val));
			const td::Decimal<TVAL, NDEC>& vToAdd = var.getVal(val);
			val -= vToAdd;
		}

		/// @brief Subtract a generic arithmetic value from the reference variant.
		/// @tparam TVAL Arithmetic type.
		/// @param val Left-hand value to subtract from.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val -= vToAdd;
		}
	};


	/// @brief Visitor that performs *= on the stored value using the corresponding field from a reference variant.
	/// @tparam TVAR Variant type providing typed accessors.
	template <typename TVAR>
	struct MultEqVisitor
	{
		const TVAR& var; ///< Reference variant providing the right-hand operand.

		/// @brief Construct with the right-hand variant reference.
		/// @param varIn Right-hand variant for multiplication.
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
        void operator() (td::Anchor) const{}
		/// @brief Multiply a generic arithmetic value by the reference variant.
		/// @tparam TVAL Arithmetic type.
		/// @param val Left-hand value to multiply.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val *= vToAdd;
		}
	};

	/// @brief Visitor that performs /= on the stored value using the corresponding field from a reference variant.
	/// @tparam TVAR Variant type providing typed accessors.
	template <typename TVAR>
	struct DivEqVisitor
	{
		const TVAR& var; ///< Reference variant providing the right-hand operand.

		/// @brief Construct with the right-hand variant reference.
		/// @param varIn Right-hand variant for division.
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
        void operator() (td::Anchor) const{}

		/// @brief Divide a generic arithmetic value by the reference variant.
		/// @tparam TVAL Arithmetic type.
		/// @param val Left-hand value to divide.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			//TVAL x;
			const TVAL& vToAdd = var.getVal(val);
			val /= vToAdd;
		}
	};

	/// @brief Visitor that replaces the stored value with its absolute value (in-place).
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
        void operator() (td::Anchor) const{}
		/// @brief Negate the value if it is negative, making it non-negative.
		/// @tparam TVAL Arithmetic type.
		/// @param val Value to make absolute in place.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			if (val < 0)
				val *= -1;
		}
	};

	/// @brief Visitor that assigns values between variants of potentially different types via numeric conversion.
	/// @tparam TVAR Source variant type providing typed accessors.
	template <typename TVAR>
	struct AssignVisitor
	{
		const TVAR& var; ///< Reference to the source variant.

		/// @brief Construct with the source variant reference.
		/// @param varIn Source variant to assign from.
		AssignVisitor(const TVAR& varIn) : var(varIn) {}

		void operator()(none)  const{ assert(false); }
		void operator() (td::String& str) const{ str = var.strVal(); }
		void operator() (td::Date& val) const { val = var.dateVal(); }
		void operator() (td::Time& val) const{ val = var.timeVal(); }
		void operator() (td::DateTime& val) const{ val = var.dtVal(); }
		void operator() (td::Color& val) const { val = var.colorVal(); }
        void operator() (td::ColorID& val) const { val = var.colorID(); }
        void operator() (td::LinePattern& val) const { val = var.linePattern(); }
        void operator() (td::DotPattern& val) const { val = var.dotPattern(); }
        void operator() (td::Anchor& val) const { val = var.anchor(); }
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

		/// @brief Assign a Decimal value from the source variant via double conversion.
		/// @tparam THOLDER Decimal backing type.
		/// @tparam NDEC Number of decimal places.
		/// @param val Destination Decimal to assign.
		template< typename THOLDER, int NDEC >
		void operator()(td::Decimal<THOLDER, NDEC>& val) const
		{
			val = var.template toNumber<double>();
		}

		/// @brief Assign a generic arithmetic value from the source variant via numeric conversion.
		/// @tparam TVAL Arithmetic type.
		/// @param val Destination value to assign.
		template <typename TVAL>
		void operator()(TVAL& val) const
		{
			val = var.template toNumber<TVAL>();
		}
	};

}
