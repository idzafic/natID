// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cnt/List.h>
#include <td/String.h>

#include <assert.h>

namespace mem
{	
	//template <typename T_CHAR>
	class TStrAllocBuff
	{
	public:
		td::UINT4 len;
		char* buff;
	};

	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>	
	template <class TUSEDSTRING = td::StringExt, td::UINT4 SINGLEBUFFLEN = 1024 * 4>
	class StringMemoryManager : public cnt::ListSL< TStrAllocBuff, false >
	{
	protected:		
		typedef cnt::ListSL<TStrAllocBuff, false> BASE;
		typedef typename BASE::iterator BASE_ITER;
		typedef typename BASE::const_iterator CONST_BASE_ITER;
		typedef typename TUSEDSTRING::CHAR_TYPE T_CHAR;
		
		typedef struct _tChunk
		{
			td::UINT4 len;
			struct _tChunk* next;			
		}ChunkLink;		

		ChunkLink* _firstAvailableChunk;
		TUSEDSTRING _zeroLenStr;
		const int SPACE_FOR_SIZE;
			
	protected:
		inline ChunkLink* allocateBuffer(td::UINT4 minLen, ChunkLink* pPrevLink = nullptr)
		{
			if (minLen < SINGLEBUFFLEN)
				minLen = SINGLEBUFFLEN;
			//TStrAllocBuff  = new T_CHAR[minLen];
			//_currentLast = _current + minLen;
			TStrAllocBuff& chunk(BASE::push());
			chunk.len = minLen;
			chunk.buff = new char[minLen];
			ChunkLink* chunkLink = (ChunkLink*)chunk.buff;
			chunkLink->len = chunk.len;
			chunkLink->next = nullptr;
			if (pPrevLink)
			{
				pPrevLink->next = chunkLink;
			}
			return chunkLink;
		}
		

		inline char* findFirstSuitableChunk(td::UINT4 len)
		{
			if (!_firstAvailableChunk)
			{
				_firstAvailableChunk = allocateBuffer(len);				
			}
			
			ChunkLink* pChunk = _firstAvailableChunk;
			ChunkLink* pPrev = nullptr;
			while (pChunk && (pChunk->len < len))
			{
				pPrev = pChunk;
				pChunk = pChunk->next;
			}

			if (!pChunk)
			{
				pChunk = allocateBuffer(len, pPrev);
			}
			
			assert(pChunk);
			
			char* pReqCh = (char*)pChunk;

			td::UINT4 remainingInChunk = pChunk->len - len;
			if (remainingInChunk < 32)
			{
				//this chunk cannot hold more data
				if (pPrev)
					pPrev->next = pChunk->next;
				else //if (_firstAvailableChunk == pChunk)
				{
					//check first
					assert(_firstAvailableChunk == pChunk);
					_firstAvailableChunk = _firstAvailableChunk->next;
				}				
			}				
			else
			{
				ChunkLink* pNextChunk = pChunk->next;
				char* pAfterThisString = pReqCh + len;
				ChunkLink* pChunkAfter = (ChunkLink*) pAfterThisString;
				pChunkAfter->len = remainingInChunk;
				pChunkAfter->next = pNextChunk;

				if (pPrev)
					pPrev->next = pChunk->next;
				else 
				{
					//check first
					assert(_firstAvailableChunk == pChunk);
					_firstAvailableChunk = pChunkAfter;
				}
			}

			return pReqCh;
		}

	public:

		StringMemoryManager()
			: _firstAvailableChunk(nullptr)
			, _zeroLenStr()
			, SPACE_FOR_SIZE(_zeroLenStr.spaceForSize())
		{
		}

		inline TUSEDSTRING* allocObject(td::UINT4 nChars, const T_CHAR* pInStr = nullptr)
		{
			if (nChars == 0)
				return nullptr;

			td::UINT4 byteDataLen = (nChars + 1) * sizeof(T_CHAR);
			td::UINT4 nBuffLen = byteDataLen + SPACE_FOR_SIZE + sizeof(TUSEDSTRING);
			if (nBuffLen % STRING_BASE_ADDRESS)
			{
				//allign it properly
				td::UINT4 nMult = nBuffLen / STRING_BASE_ADDRESS;
				nBuffLen = (nMult + 1) * STRING_BASE_ADDRESS;
			}

			char* pBuff = findFirstSuitableChunk(nBuffLen);

			TUSEDSTRING* retStr = (TUSEDSTRING*)(pBuff);
			retStr->initObjectOnAllocator(pBuff, byteDataLen, pInStr);
			
			return retStr;
		}

		////string se ne moze smjesti u string referencu konzumera, samo u objekat string
		//inline TUSEDSTRING& allocDataHolder(td::UINT4 nChars, const T_CHAR* pInStr = nullptr)
		//{
		//	if (nChars == 0)
		//		return _zeroLenStr;	

		//	td::INT4 byteDataLen = (nChars+1) * sizeof(T_CHAR);
		//	td::UINT4 nBuffLen = byteDataLen + SPACE_FOR_SIZE;
		//	if (nBuffLen % STRING_BASE_ADDRESS)
		//	{
		//		//allign it properly
		//		td::UINT4 nMult = nBuffLen / STRING_BASE_ADDRESS;
		//		nBuffLen = (nMult + 1) * STRING_BASE_ADDRESS;
		//	}			
		//	
		//	char* pBuff = findFirstSuitableChunk(nBuffLen);
		//	
		//	TUSEDSTRING& retStr = (TUSEDSTRING&)(pBuff);
		//	//retStr.initDataHolderOnAllocator(pBuff, byteDataLen, pInStr);
		//	retStr.initDataHolderOnAllocator(byteDataLen, pInStr);
		//	return retStr;
		//}

		inline void allocDataHolder(TUSEDSTRING& str, td::UINT4 nChars, const T_CHAR* pInStr = nullptr)
		{
			str.cleanBuffer();
			if (nChars == 0)
			{				
				return;
			}
				//return _zeroLenStr;

			td::UINT4 byteDataLen = (nChars + 1) * sizeof(T_CHAR);
			td::UINT4 nBuffLen = byteDataLen + SPACE_FOR_SIZE;
			if (nBuffLen % STRING_BASE_ADDRESS)
			{
				//allign it properly
				td::UINT4 nMult = nBuffLen / STRING_BASE_ADDRESS;
				nBuffLen = (nMult + 1) * STRING_BASE_ADDRESS;
			}

			char* pBuff = findFirstSuitableChunk(nBuffLen);

			TUSEDSTRING& retStr = (TUSEDSTRING&)(pBuff);
			//retStr.initDataHolderOnAllocator(pBuff, byteDataLen, pInStr);
			retStr.initDataHolderOnAllocator(byteDataLen, pInStr);
			
			str.buffer = pBuff;			
			//return retStr;
		}
        
        inline void allocDataHolder(TUSEDSTRING& str, td::UINT4 nChars, const T_CHAR* pInStr, T_CHAR chLead)
        {
            str.cleanBuffer();
            if (nChars == 0)
            {
                return;
            }
                //return _zeroLenStr;

            td::UINT4 byteDataLen = (nChars + 2) * sizeof(T_CHAR);
            td::UINT4 nBuffLen = byteDataLen + SPACE_FOR_SIZE;
            if (nBuffLen % STRING_BASE_ADDRESS)
            {
                //allign it properly
                td::UINT4 nMult = nBuffLen / STRING_BASE_ADDRESS;
                nBuffLen = (nMult + 1) * STRING_BASE_ADDRESS;
            }

            char* pBuff = findFirstSuitableChunk(nBuffLen);

            TUSEDSTRING& retStr = (TUSEDSTRING&)(pBuff);
            //retStr.initDataHolderOnAllocator(pBuff, byteDataLen, pInStr);
            retStr.initDataHolderOnAllocator(byteDataLen, pInStr, chLead);
            
            str.buffer = pBuff;
            //return retStr;
        }

		~StringMemoryManager()
		{			
			clean();
		}

		void clean()
		{
			BASE_ITER it(BASE::begin());
			CONST_BASE_ITER itEnd(BASE::end());
			while (it != itEnd)
			{
				delete[] it->buff;
				++it;
			}
			BASE::clean();
		}
	};
}
