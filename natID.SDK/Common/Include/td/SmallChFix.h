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
#include <cstring>

namespace td
{
	class Variant;

	template <size_t N>
	class SmallChFix
	{	
		union
		{
			char _chVal[8]; //N < 8, zero terminated
			td::LUINT8 _lVal;
		};
		
	public:

		SmallChFix()			
		{
			static_assert (N <= 7, "Wrong size of SmallChFix! It should be less than 8!!");
			_lVal = 0;
		}

		char getAt(size_t pos) const
		{
			assert(pos < N);
			return _chVal[pos];
		}

		td::LUINT8 lu8Val() const
		{
			return _lVal;
		}

		SmallChFix(const char* val)
		{
			operator = (val);
		}

		SmallChFix(const SmallChFix& val)
		{
			operator = (val);
		}

		const char* c_str() const
		{
			return &_chVal[0];
		}
		

		bool isZero() const
		{
			return _chVal[0] == 0;
		}

		void operator = (const char ch)
		{
			static_assert (N == 1, "Char assignment is allowed for SmallChFix<1> only!!");
			_chVal[0] = ch;
		}

		void operator = (const char* val)
		{
			size_t len = strlen(val);
			if (len == 0)
				_chVal[0] = 0;
			else
			{
				if (len > N)
					len = N; //trim
				memcpy(_chVal, val, len);
				_chVal[len] = 0;
			}
		}

		void operator = (const SmallChFix& val)
		{
			//operator = (val.c_str());
			_lVal = val._lVal;
			_chVal[N] = 0;
		}
				
		inline SmallChFix& operator = (const Variant& var);
		//{
		//	SmallChFix& chFix = *this;
		//	var.getValue(chFix);
		//	return *this;
		//}

		bool operator == (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x == 0;
		}

		bool operator < (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x < 0;
		}
		
		bool operator <= (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x <= 0;
		}

		bool operator > (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x > 0;
		}
		
		bool operator >= (const char* val) const
		{
			int x = strcmp(&_chVal[0], val);
			return x >= 0;
		}

		bool operator == (const SmallChFix& val) const
		{
			return operator == (val.c_str());
		}

		bool operator < (const SmallChFix& val) const
		{
			return operator < (val.c_str());
		}
		
		bool operator <= (const SmallChFix& val) const
		{
			return operator <= (val.c_str());
		}

		bool operator > (const SmallChFix& val) const
		{
			return operator > (val.c_str());
		}
		
		bool operator >= (const SmallChFix& val) const
		{
			return operator >= (val.c_str());
		}


		constexpr size_t getFixLen() const
		{
			return N;
		}

		
		size_t length() const
		{
			return strlen(c_str());
		}

		constexpr size_t getBuffLen() const
		{
			return N+1;
		}

		template <typename TSTREAM>
		inline void saveToBinaryStream(TSTREAM& s) const
		{
			s.put(&_chVal[0], N);
		}

		template <typename TSTREAM>
		inline bool loadFromBinaryStream(TSTREAM& s)
		{
			s.get(&_chVal[0], N);
			_chVal[N] = 0;
			return true;
		}

		void fromString(const char* str)
		{
			operator = (str);
		}


	protected:
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const SmallChFix& rhs)
		{
			os << rhs.c_str();
			return os;
		}
	};

	template <size_t NCH>
	DataType getType(SmallChFix<NCH>) 
	{ 
		return td::ch7; 
	}
		
	typedef SmallChFix<7> ChFix7;
}
