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
#include <cassert>
#ifdef MU_CLANG
#pragma clang diagnostic ignored "-Wundefined-inline"
#endif

namespace td
{
	//class Variant;

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	class StringBase; // < T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>;

	template <typename THOLDER, int NDEC>
	class Decimal
	{
	protected:
		THOLDER _value;		
		template <typename T>
		inline THOLDER round(T val)
		{			
			if (val > getMinVal())
				return (THOLDER) (val * getExp() + (T)0.5);
			else if (val < -getMinVal())
				return (THOLDER) (val * getExp() - (T)0.5);
			return 0;
		}
		
	public:
		Decimal(double val)
			: _value(round(val))
		{
		}
		Decimal(float val)
			: _value(round(val))
		{
		}

		Decimal(td::INT4 val)
			: _value(val)
		{
		}

		Decimal(td::LINT8 val)
			: _value(val)
		{
		}

        Decimal( const Decimal& dec)
            : _value(dec.getValue())
        {
        }
        
		template <typename THOLDER2, int NDEC2>
		Decimal(const Decimal<THOLDER2, NDEC2>& dec)
		{
			operator = (dec);
		}

		//Decimal(THOLDER intPart, THOLDER decPart)	
		//{
		//	_value = intPart << getNumberOfDecimals();
		//	_value |= decPart;
		//}

		Decimal()		
		{
		}

		inline THOLDER getExp() const
		{
			assert (NDEC >= 0 && NDEC <= 4);
			if (NDEC == 0)
				return 1;
			else if (NDEC == 2)
				return 100;
			else if (NDEC == 1)
				return 10;
			else if (NDEC == 3)
				return 1000;
			//else if (NDEC == 4)
			return 10000;
		}

		void fromIntegers(THOLDER intPart, THOLDER decPart, bool negative, int nScale = NDEC)
		{
			//if (NDEC == 0)
				//decPart = 0;
			
			if (decPart != 0)
			{
				if (nScale < 0)
					nScale = 0;

				if (nScale < NDEC)
				{
					for (int i = nScale; i < NDEC; ++i)					
						decPart *= 10;
				}
				else if (nScale > NDEC)
				{
					THOLDER divider = 1;
					for (int i = nScale; i > NDEC; --i)	
						divider *= 10;
					double dDecPart = (double)decPart / divider;
					decPart /= divider;
					if (NDEC == 0)
					{
						if (dDecPart >= 0.5)
							++intPart;
						decPart = 0;
					}
					else if ((dDecPart - (int)dDecPart) >= 0.5)
						++decPart;
				}
			}
			
			_value = intPart;
			_value *= getExp();
			_value += decPart;

			if (negative)
				_value = (-_value);
		}
	protected:
		//for decimal
		inline void scanParts(const char* str, td::LINT8& intPart, td::LINT8& decPart, int& nScale) const
		{
			nScale = td::findFromRight(str, '.');
			if (nScale > 0)
			{
				if (*str == '.')
					SSCANF(str, "." FMT_LINT8, &decPart);
				else
					SSCANF(str, FMT_LINT8 "." FMT_LINT8, &intPart, &decPart);
			}
			else
			{
				SSCANF(str, FMT_LINT8, &intPart);
			}

			//if (nScale > 0)
			//{
			//	if (*str == '.')
			//		SSCANF(str, ".%lld", &decPart);
			//	else
			//		SSCANF(str, "%lld.%lld", &intPart, &decPart);
			//}
			//else
			//{
			//	SSCANF(str, "%lld", &intPart);
			//}
		}

		//for smalldecimal
		inline void scanParts(const char* str, td::INT4& intPart, td::INT4& decPart, int& nScale) const
		{
			nScale = td::findFromRight(str, '.');
			if (nScale > 0)
			{
				if (*str == '.')
					SSCANF(str, ".%d", &decPart);
				else
					SSCANF(str, "%d.%d", &intPart, &decPart);
			}
			else
			{
				SSCANF(str, "%d", &intPart);
			}
		}
	public:
		inline void fromString(const char* str)
		{
			THOLDER intPart=0;
			THOLDER decPart=0;

			if (!str)
			{
				fromIntegers(intPart, decPart, false);
				return;
			}
			if (*str== 0)
			{
				fromIntegers(intPart, decPart, false);
				return;
			}

			bool negative = false;
			if (*str == '-')
			{
				negative = true;
				++str;
			}

			int nScale = 0;
			scanParts(str, intPart, decPart, nScale);		
			fromIntegers(intPart, decPart, negative, nScale);
		}	
		void setValueFromString(const char* str)
		{
			fromString(str);
		}

		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

	protected:
		inline int formc_str(char* buffer, int buffLen, td::INT4 intPart, td::INT4 decPart) const
		{
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"
			
			int nLen = 0;
			if (_value < 0)
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-%d.%0*d", intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-%d", intPart);
			}				
			else
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "%d.%0*d", intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "%d", intPart);
			}
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
			return nLen;
		}


		inline int formc_str(char* buffer, int buffLen, td::LINT8 intPart, td::LINT8 decPart) const
		{
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"
			int nLen = 0;
			if (_value < 0)
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-" FMT_LINT8 ".%0*" FMT_LINT8_EXT, intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, "-" FMT_LINT8, intPart);
			}
			else
			{
				if (NDEC > 0)
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, FMT_LINT8 ".%0*" FMT_LINT8_EXT, intPart, NDEC, decPart);
				else
					nLen = (int)SNPRINTF(buffer, buffLen, _TRUNCATE, FMT_LINT8, intPart);
			}
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
			return nLen;
		}
		
	public:

		int formc_str(char* buffer, int buffLen) const
		{
			int nLen = formc_str(buffer, buffLen, getIntPart(), getDecPart());
			if (nLen < 0)
			{
				buffer[buffLen - 1] = 0; //just in case
				return buffLen -1;
			}
			return nLen;
		}

		template <size_t size>
		inline int formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in Date::formc_str");
			return formc_str(&buffer[0], size);
		}

		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		template <typename TUTIL>
		const char* c_str(TUTIL& u) const
		{
			return u.c_str(*this);
		}

		//const char* toString() const
		//{
		//	char buffer[24];
		//	int nLen = 0;
		//	if (typeid(td::INT4) == typeid(THOLDER))
		//	{
		//		if (_value < 0)
		//			nLen = (int)SNPRINTF(buffer, 24, _TRUNCATE, "-%d.%0*d", getIntPart(), NDEC, getDecPart());
		//		else
		//			nLen = (int)SNPRINTF(buffer, 24, _TRUNCATE, "%d.%0*d", getIntPart(), NDEC, getDecPart());
		//	}
		//	else
		//	{
		//		if (_value < 0)
		//			nLen = (int)SNPRINTF(buffer, 24, _TRUNCATE, "-%lld.%0*lld", getIntPart(), NDEC, getDecPart());
		//		else
		//			nLen = (int)SNPRINTF(buffer, 24, _TRUNCATE, "%lld.%0*lld", getIntPart(), NDEC, getDecPart());
		//	}


		//	if (nLen < 0)
		//	{
		//		buffer[23] = 0; //just in case				
		//	}
		//	return buffer;
		//}

		inline float getMinVal() const
		{
			assert (NDEC >= 0 && NDEC <= 4);
			if (NDEC == 0)
				return 0.1f;
			else if (NDEC == 2)
				return 1e-3f;
			else if (NDEC == 1)
				return 1e-2f;
			else if (NDEC == 3)
				return 1e-4f;
			//else if (NDEC == 4)
			return 1e-5f;
		}

		inline THOLDER getValue() const
		{
			return _value;
		}

		inline void setValue(THOLDER val)
		{
			_value = val;
		}

		template <typename T>
		inline THOLDER value(T& val) const
		{
			val = (T)_value / getExp();
		}

		double getAsFloat() const
		{
			THOLDER intPart = getIntPart();
			THOLDER decPart = getDecPart();
			if (_value < 0)
				return -((double) intPart + (double) decPart / getExp());
			return (double) intPart + (double) decPart / getExp();
			//return (double) _value / getExp();
		}

		THOLDER getIntPart() const
		{
			if (_value < 0)
				return ((-_value) / getExp());

			return _value / getExp();
		}

		THOLDER getDecPart() const
		{
			if (_value < 0)
				return ((-_value) % getExp());

			return (_value % getExp());
		}		

		inline int getNumberOfDecimals() const
		{
			return NDEC;
		}

		bool operator == (const THOLDER val) const
		{
			return (_value == val);
		}

		bool operator < (const THOLDER val) const
		{
			return (_value < val);
		}

		bool operator > (const THOLDER val) const
		{
			return (_value > val);
		}

		bool operator <= (const THOLDER val) const
		{
			return (_value <= val);
		}

		bool operator >= (const THOLDER val) const
		{
			return (_value >= val);
		}

		bool operator == (const Decimal& val) const
		{
			return (_value == val.getValue());
		}

		bool operator != (const Decimal& val) const
		{
			return (_value != val.getValue());
		}

		bool operator <= (const Decimal& val) const
		{
			return (_value <= val.getValue());
		}

		bool operator >= (const Decimal& val) const
		{
			return (_value >= val.getValue());
		}

		bool operator < (const Decimal& val) const
		{
			return (_value < val.getValue());
		}

		bool operator > (const Decimal& val) const
		{
			return (_value > val.getValue());
		}

		void operator += (const Decimal& val)
		{
			_value += val.getValue();
		}

		void operator += (float val)
		{
			_value += round(val);
		}

		void operator += (td::INT4 val)
		{
			_value += (THOLDER)(val * getExp());
		}

		void operator += (td::LINT8 val)
		{
			_value += (THOLDER)(val * getExp());
		}

		void operator += (double val)
		{
			_value += round(val);
		}

		void operator -= (const Decimal& val)
		{
			_value -= val.getValue();
		}

		void operator -= (float val)
		{
			_value -= round(val);
		}

		void operator -= (double val)
		{
			_value -= round(val);
		}

		template <typename THOLDER2, int NDEC2>
		Decimal& operator = (const Decimal<THOLDER2, NDEC2>& dec)
		{
			if (NDEC == NDEC2)
			{
				_value = (THOLDER)dec.getValue();
			}
			else
			{
				//TODO: Poboljsati
				double val = dec.getAsFloat();
				operator = (val);
			}
			return *this;
		}
				
		template <class TVARIANT>
		inline Decimal& operator = (const TVARIANT& var)
		{
			Decimal& dec = *this;
			var.getValue(dec);
			return *this;
		}

		void operator *= (td::INT4 val)
		{
			_value *= val; //(THOLDER)(val * getExp());
		}

		void operator *= (td::LUINT8 val)
		{
			_value *= val; //(THOLDER)(val * getExp());
		}

		template<typename TINHOLDER, int NDECIN>
		void operator *= (const Decimal<TINHOLDER, NDECIN>& dec)
		{
			//TODO: Poboljsati
			double val = getAsFloat();
			double valIn = dec.getAsFloat();
			operator = (val * valIn);
		}

		template<typename TINHOLDER, int NDECIN>
		void operator /= (const Decimal<TINHOLDER, NDECIN>& dec)
		{
			//TODO: Poboljsati
			double val = getAsFloat();
			double valIn = dec.getAsFloat();
			operator = (val / valIn);
		}

		template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
		void mult(const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
		{
			double val1 = dec1.getAsFloat();
			double val2 = dec2.getAsFloat();
			operator = (val1 * val2);
		}

		template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
		void div(const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
		{
			double val1 = dec1.getAsFloat();
			double val2 = dec2.getAsFloat();
			operator = (val1 / val2);
		}
		void operator = (float val)
		{
			if (val != 0)
				_value = round(val);
			else
				_value = 0;
		}

		void operator = (double val)
		{
			if (val != 0)
				_value = round(val);
			else
				_value = 0;
		}

		void operator = (td::INT4 val)
		{
			if (val != 0)
				_value = (THOLDER)(val * getExp());
			else
				_value = 0;
		}

		void operator = (td::LINT8 val)
		{
			if (val != 0)
				_value = (THOLDER)(val * getExp());
			else
				_value = 0;
		}
        
        template<class Archive>
        bool loadFromBinaryStream(Archive& ar)
        {
            ar >> _value;
            return true;
        }
        
        template<class Archive>
        void saveToBinaryStream(Archive& ar) const
        {
            ar << _value;
        }
        
	protected:
		//template<class CharT, class Traits>
		//friend std::basic_ostream<CharT, Traits>&
#ifndef NATRPT_EXPORTS
		template<class OSTREAM>
		friend OSTREAM& operator<<(OSTREAM& os,  const Decimal& rhs)
		{
			char tmp[32];
			rhs.formc_str(tmp, 32);
			os  << tmp;
			return os;
		}
        
        template<class ISTREAM>
        friend ISTREAM& operator>>(ISTREAM& is, Decimal& rhs)
        {
            std::string str;
            is >> str;
            rhs.fromString(str.c_str());
            return is;
        }
#endif
	};

	template<typename THOLDER1, int NDEC1>
	bool operator == (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() == dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	bool operator != (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() != dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	bool operator > (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() > dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	bool operator >= (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() >= dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	bool operator < (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() < dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	bool operator <= (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		return dec1.getValue() <= dec2.getValue();
	}

	template<typename THOLDER1, int NDEC1>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER1, NDEC1>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / dec2.getAsFloat();
	}

	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / dec2.getAsFloat();
	}

	template<typename THOLDER, int NDEC>
	double operator * (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * dec2.getAsFloat();
	}

	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator * (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * dec2.getAsFloat();
	}

	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator + (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() + dec2.getAsFloat();
	}

	template<typename THOLDER, int NDEC>
	Decimal<THOLDER, NDEC> operator - (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{		
		Decimal<THOLDER, NDEC> res(dec1);
		res -= dec2;
		return res;
	}

	template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	double operator - (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() - dec2.getAsFloat();
	}

	template<typename THOLDER, int NDEC>
	Decimal<THOLDER, NDEC> operator + (const Decimal<THOLDER, NDEC>& dec1, const Decimal<THOLDER, NDEC>& dec2)
	{
		Decimal<THOLDER, NDEC> res(dec1);
		res += dec2;
		return res;
	}

	template<typename THOLDER1, int NDEC1, typename TRHS>
	Decimal<THOLDER1, NDEC1> operator + (const Decimal<THOLDER1, NDEC1>& dec1, TRHS rhs)
	{
		//TODO: Poboljsati
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = rhs;
		dec2 += dec1;
		return dec2;
	}

	//template<typename THOLDER1, int NDEC1>
	//Decimal<THOLDER1, NDEC1> operator - (const Decimal<THOLDER1, NDEC1>& dec1, td::INT4 rhs)
	//{		
	//	Decimal<THOLDER1, NDEC1> dec2;
	//	dec2 = rhs;		
	//	Decimal<THOLDER1, NDEC1> dec3(dec1);
	//	dec3 -= dec2;
	//	return dec3;
	//}

	template<typename THOLDER1, int NDEC1, typename TLHS>
	Decimal<THOLDER1, NDEC1> operator + (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = lhs;		
		dec2 += dec1;
		return dec2;
	}

	template<typename THOLDER1, int NDEC1, typename TLHS>
	Decimal<THOLDER1, NDEC1> operator - (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = lhs;		
		dec2 -= dec1;
		return dec2;
	}


	template<typename THOLDER1, int NDEC1, typename TRHS>
	Decimal<THOLDER1, NDEC1> operator - (const Decimal<THOLDER1, NDEC1>& dec1,  TRHS rhs)
	{		
		Decimal<THOLDER1, NDEC1> dec2;
		dec2 = rhs;
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 -= dec2;
		return dec3;
	}

	template<typename THOLDER1, int NDEC1>
	Decimal<THOLDER1, NDEC1> operator * (const Decimal<THOLDER1, NDEC1>& dec1, td::INT4 rhs)
	{	
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 *= rhs;
		return dec3;
	}

	template<typename THOLDER1, int NDEC1>
	Decimal<THOLDER1, NDEC1> operator * (td::INT4 lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		Decimal<THOLDER1, NDEC1> dec3(dec1);
		dec3 *= lhs;
		return dec3;
		//return dec1.getAsFloat() * lhs;
	}

	template<typename THOLDER1, int NDEC1>
	double operator * (double lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * lhs;
	}

	template<typename THOLDER1, int NDEC1>
	double operator * (const Decimal<THOLDER1, NDEC1>& dec1, double rhs)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() * rhs;
	}

	template<typename THOLDER1, int NDEC1, typename TRHS>
	double operator / (const Decimal<THOLDER1, NDEC1>& dec1, TRHS rhs)
	{
		//TODO: Poboljsati
		return dec1.getAsFloat() / rhs;
	}

	//template<typename THOLDER1, int NDEC1>
	//double operator / (const Decimal<THOLDER1, NDEC1>& dec1, double rhs)
	//{
	//	//TODO: Poboljsati
	//	return dec1.getAsFloat() / rhs;
	//}

	template<typename THOLDER1, int NDEC1, typename TLHS>
	double operator / (TLHS lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	{
		//TODO: Poboljsati
		return lhs / dec1.getAsFloat();
	}


	//template<typename THOLDER1, int NDEC1, typename THOLDER2, int NDEC2>
	//double operator / (const Decimal<THOLDER1, NDEC1>& dec1, const Decimal<THOLDER2, NDEC2>& dec2)
	//{		
	//	return dec1.getAsFloat() / dec2.getAsFloat();
	//}

	//template<typename THOLDER1, int NDEC1>
	//double operator / (int lhs, const Decimal<THOLDER1, NDEC1>& dec1)
	//{
	//	//TODO: Poboljsati
	//	return lhs / dec1.getAsFloat();
	//}

	typedef Decimal<td::LINT8, 0> Decimal0;
	typedef Decimal<td::LINT8, 1> Decimal1;
	typedef Decimal<td::LINT8, 2> Decimal2;
	typedef Decimal<td::LINT8, 3> Decimal3;
	typedef Decimal<td::LINT8, 4> Decimal4;	

	typedef Decimal<td::INT4, 0> SmallDecimal0;
	typedef Decimal<td::INT4, 1> SmallDecimal1;
	typedef Decimal<td::INT4, 2> SmallDecimal2;
	typedef Decimal<td::INT4, 3> SmallDecimal3;
	typedef Decimal<td::INT4, 4> SmallDecimal4;	

	inline DataType getType(const Decimal0&) {return decimal0;}
	inline DataType getType(const Decimal1&) {return decimal1;}
	inline DataType getType(const Decimal2&) {return decimal2;}
	inline DataType getType(const Decimal3&) {return decimal3;}
	inline DataType getType(const Decimal4&) {return decimal4;}

	inline DataType getType(const SmallDecimal0&) {return smallDecimal0;}
	inline DataType getType(const SmallDecimal1&) {return smallDecimal1;}
	inline DataType getType(const SmallDecimal2&) {return smallDecimal2;}
	inline DataType getType(const SmallDecimal3&) {return smallDecimal3;}
	inline DataType getType(const SmallDecimal4&) {return smallDecimal4;}
};
