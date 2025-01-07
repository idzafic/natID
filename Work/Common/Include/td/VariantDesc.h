// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Variant.h>
#include <td/SmallChFix.h>

namespace td
{
	class VariantDesc : public Variant
	{
	public:		
		td::String desc;		
		td::INT2 posIn;
		td::INT2 posOut;
		td::DataType outType;
		int outLen;

		VariantDesc()
			: Variant()
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		VariantDesc(td::DataType dt, int outLen)
			: Variant(dt)
			, posIn(-1)
			, posOut(-1)
			, outType(dt)
			, outLen(outLen)
		{
		}

		VariantDesc(const Variant& var)
			: Variant(var)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		VariantDesc(const char* pDesc, const Variant& var)
			: Variant(var)
			, desc(pDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		template <size_t size>
		VariantDesc(const char (&strDesc)[size])
			:Variant()
			, desc(strDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{			
		}

		template <size_t size>
		VariantDesc(const char (&strDesc)[size], const Variant& var)
			:Variant(var)
			, desc(strDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{			
		}

		VariantDesc(const char* pDesc)
			: Variant()
			, desc(pDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		VariantDesc(const td::String& strDesc, const Variant& var)
			: Variant(var)
			, desc(strDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		VariantDesc(const td::String& strDesc)
			: Variant()
			, desc(strDesc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		VariantDesc(const VariantDesc& var)
			: Variant(var)
			, desc(var.desc)
			, posIn(-1)
			, posOut(-1)
			, outType(td::TD_NONE)
			, outLen(0)
		{
		}

		template <size_t NCH>
                VariantDesc(const td::SmallChFix<NCH>& var)
                        : Variant(td::chFix)
			, posIn(-1)
			, posOut(-1)
			, outType(td::chFix)
			, outLen(NCH)
		{
			memcpy(&_chVal[0], var.c_str(), NCH);
			_chVal[NCH] = 0;
		}

		bool isPaired() const
		{			
			return ( (posOut >=0) && (posIn >= 0) );
		}
		//template <class TSTREAM>
		//TSTREAM& toStream(TSTREAM& s)
		//{
		//	s << desc << ":";
		//	return Variant::toStream(s);			
		//}

		template <class TSTREAM>
		TSTREAM& toStream(TSTREAM& s) const
		{
			s << desc << "[" << posIn << ", " << posOut <<"][" << mu::Utils::c_str(getType()) << ", " <<mu::Utils::c_str(outType);
			if (outLen)
				s << "(" << outLen << ")";
			s << "]=";
			return Variant::toStream(s);
		}


		template <size_t NCH>
		VariantDesc& operator = (SmallChFix<NCH> val)
		{
			Variant::operator=(val.c_str());
                        outType = td::nchFix;
			outLen = (int) NCH;
			return *this;
		}

		template <typename TVAL>
		VariantDesc& operator = (TVAL val)
		{
			Variant::operator=(val);
			return *this;
		}		

	protected:
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os,  const VariantDesc& rhs)
		{			
			return rhs.toStream(os);
		}
		
	};
}
