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

namespace td
{
	class BoolCh
	{	
		char _bVal[2]; //'Y', 'N'
    protected:
        template<class CharT, class Traits>
        friend std::basic_ostream<CharT, Traits>&
            operator << (std::basic_ostream<CharT, Traits>& os,  const BoolCh& rhs)
        {
            os << rhs._bVal;
            return os;
        }
        
        template<class CharT, class Traits>
        friend std::basic_istream<CharT, Traits>&
            operator>>(std::basic_istream<CharT, Traits>& is, BoolCh& rhs)
        {
            char ch;
            is >> ch;
            if (ch == 'Y' || ch == 'y')
                ch = 'Y';
            else
                ch = 'N';
            rhs._bVal[0] = ch;
            rhs._bVal[1] = 0;
            return is;
        }
	public: 

		BoolCh()		
			: _bVal{'N', 0}
		{
			//_bVal[0] = 'N';
			//_bVal[1] = 0;
		}

		BoolCh(bool val)
			: _bVal{ 'N', 0 }
		{
			if (val)
				_bVal[0] = 'Y';
		}

		//could be implicit conversion instead ??
		//operator bool() const
		//{
		//	return (_bVal == 'Y');
		//}

		bool operator() () const
		{
			return (_bVal[0] == 'Y');
		}

		const char* c_str() const
		{
			if (_bVal[0] == 'Y')
				return td::boolToStr(true);
			return td::boolToStr(false);
		}

		const char* yesno_str() const
		{
			return &_bVal[0];
		}

		BoolCh& operator = (bool val)
		{
			if (val)
				_bVal[0] = 'Y';
			else
				_bVal[0] = 'N';
			return *this;
		}

		BoolCh& operator = (BoolCh val)
		{			
			_bVal[0] = val._bVal[0];
			return *this;
		}
        
        inline bool operator == (const BoolCh& val) const
        {
            return (_bVal[0] == val._bVal[0]);
        }

		template <class TVAR>
		inline BoolCh& operator = (const TVAR& var)
		{
			BoolCh& bch = *this;
			var.getValue(bch);
			return *this;
		}

		bool operator == (bool val) const
		{
			if (val)
				return (_bVal[0] == 'Y');
			
			return (_bVal[0] == 'N');
		}

		bool operator != (bool val) const
		{
			if (val)
				return (_bVal[0] != 'Y');

			return (_bVal[0] != 'N');
		}

		bool operator < (bool val) const
		{
			if (val)
				return (_bVal[0] == 'N');

			return false;
		}

		bool operator <= (bool val) const
		{
			if (val)
				return (_bVal[0] == 'N');

			return false;
		}

		bool operator > (bool val) const
		{
			if (val)
				return false; 

			return (_bVal[0] == 'Y');
		}

		bool operator >= (bool val) const
		{
			if (val)
				return false;

			return (_bVal[0] == 'Y');
		}

		int formc_str(char* buffer, int bufLen) const
		{			
			assert(buffer);
			assert(bufLen >= 2);

			if (bufLen < 2)
				return 0;

			buffer[0] = 'N';
			buffer[1] = 0;

			if (_bVal[0] == 'Y')
				buffer[0] = 'Y';

			return 2;
		}
   
	};

	inline DataType getType(td::BoolCh) { return boolean; }
}
