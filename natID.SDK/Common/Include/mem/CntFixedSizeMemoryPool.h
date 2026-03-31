// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cassert>
#include <cnt/List.h>
#include <cnt/ListSL.h>
#include <cnt/SafeFullVector.h>


namespace mem
{	
	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>	
	template <class CNT>
	class CntFixedSizeMemoryPool
	{
	protected:
        typedef typename CNT::tEntry T_UNIT;
		typedef typename CNT::T_DATA T_DATA;
		typedef cnt::SafeFullVector<T_UNIT> TCNTDATA;
        //cnt::SafeFullVector<T_UNIT> _data;
		TCNTDATA _data;

        size_t _pos = 0;
	public:

		inline CntFixedSizeMemoryPool()
		{
		}

        void reserve(size_t nElems)
        {
            _data.reserve(nElems);
        }

		inline T_DATA* push_back(cnt::ListSL<T_DATA, true>& lst)
		{
            T_UNIT& entry = _data[_pos++];
			lst.push_back(&entry);
			T_DATA* toRet = &(entry.data);
			return toRet;
		}
        
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst)
        {
            T_UNIT& entry = _data[_pos++];
            lst.push_back(&entry);
            T_DATA* toRet = &(entry.data);
            return toRet;
        }
	};
}
