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
#include <cnt/ListSL.h>
#include <cnt/List.h>
#include <cnt/SafeFullVector.h>


namespace mem
{	
	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>	
	template <class CNT, td::UINT4 SINGLEBUFFLEN = 1024 * 4>
	class CntMemoryManager : public cnt::ListSL< typename CNT::tEntry[SINGLEBUFFLEN], false >
	{
	protected:				
		typedef typename CNT::T_DATA T_DATA;
		//typedef typename T_UNIT[SINGLEBUFFLEN] CHUNK;
		typedef cnt::ListSL<typename CNT::tEntry[SINGLEBUFFLEN], false> BASE;
		typedef typename CNT::iterator CNT_ITER;
		typedef typename BASE::const_iterator CONST_BASE_ITER;		
		typedef typename CNT::tEntry T_UNIT;

		T_UNIT* _firstAvailableUnit = nullptr;
		T_UNIT* _lasttAvailableUnit = nullptr;
		//BASE_ITER _currentChunk;
	protected:

		inline void allocateBuffer()
		{			
			BASE::push();			
			_firstAvailableUnit = (T_UNIT*)(BASE::_last);
			_lasttAvailableUnit = _firstAvailableUnit + SINGLEBUFFLEN;
		}		

	public:

        void clean()
        {
            _firstAvailableUnit = nullptr;
            _lasttAvailableUnit = nullptr;
            BASE::clean();
        }
		inline CntMemoryManager()
			: _firstAvailableUnit(nullptr)
			, _lasttAvailableUnit(nullptr)
		{
			//BASE::push();
			//_currentChunk = BASE::begin();
			//_firstAvailableUnit = (T_UNIT*)(BASE::_root);
			//_lasttAvailableUnit = _firstAvailableUnit + SINGLEBUFFLEN;
		}
		
		/*inline CNT_ITER push_back(cnt::ListSL<T_DATA, true>& lst)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);			
			++_firstAvailableUnit;
			CNT_ITER it(lst.lastIter());
			return it;
		}*/

		inline T_UNIT* alloc()
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			//lst.push_back(_firstAvailableUnit);
			T_UNIT* toRet = _firstAvailableUnit;
			++_firstAvailableUnit;
			return toRet;
		}

		inline T_DATA* push_back(cnt::ListSL<T_DATA, true>& lst)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);
			T_DATA* toRet = &_firstAvailableUnit->data;
			++_firstAvailableUnit;			
			return toRet;
		}

		inline void push_back(cnt::ListSL<T_DATA, true>& lst, const T_DATA& val)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);
			_firstAvailableUnit->data = val;
			++_firstAvailableUnit;						
		}

		inline T_DATA* push_back(cnt::SafeFullVector<T_DATA, true>& vect)
		{
			size_t nToCopy = 0;
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				nToCopy = vect._size;
				allocateBuffer();
			}

			if (vect._size == 0)
			{
				//first time
				T_DATA* pNew = _firstAvailableUnit;
				vect._size = 1;
				vect._data = pNew;
				++_firstAvailableUnit;
				return pNew;
			}
			else if (nToCopy == 0)
			{
				//executes in most cases
				size_t pos = vect._size;
				vect._size++;				
				++_firstAvailableUnit;
				return vect._data + pos;
			}

			//new vector entry cannot be placed on the previous chunk
			//executes only when vector cannot be allocated on prev. chunk
			
			assert(nToCopy < SINGLEBUFFLEN);
						
			T_DATA* pNew = _firstAvailableUnit;
			mem::fastCopy(pNew, vect._data, nToCopy);
			vect._size++;
			vect._data = pNew;			
			_firstAvailableUnit += vect._size;
			return vect._data + nToCopy;
		}

		inline void push_back(cnt::SafeFullVector<T_DATA, true>& vect, const T_DATA& val)
		{
			T_DATA* pVal = push_back(vect);
			*pVal = val;
		}
        
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst)
        {
            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            lst.push_back(_firstAvailableUnit);
            T_DATA* toRet = &_firstAvailableUnit->data;
            ++_firstAvailableUnit;
            return toRet;
        }
        
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst, T_UNIT* pPrev)
        {
            if (!pPrev)
                return push_back(lst);
            
            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            pPrev->next = _firstAvailableUnit;
            T_DATA* toRet = &_firstAvailableUnit->data;
            ++_firstAvailableUnit;
            return toRet;
        }

        inline void push_back(cnt::List<T_DATA, true>& lst, const T_DATA& val)
        {
            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            lst.push_back(_firstAvailableUnit);
            _firstAvailableUnit->data = val;
            ++_firstAvailableUnit;
        }
	};
}
