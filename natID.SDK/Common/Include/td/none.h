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

	class none
	{
		const td::LUINT8 val;
	public:
		none(): val(0) {}
		const none& operator = (const none&) const
		{
			return *this;
		}

		bool operator == (const none&) const
		{
			
			return false;
		}
		bool operator != (const none& ) const
		{
			return false;
		}
		bool operator < (const none&) const
		{
			return false;
		}
		bool operator <= (const none&) const
		{
			return false;
		}
		bool operator > (const none&) const
		{
			return false;
		}
		bool operator >= (const none&) const
		{
			return false;
		}

		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const none& rhs)
		{
			os << "TD_NONE";
			return os;
		}
	};
}
