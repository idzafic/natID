// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StringMemoryManager.h
    @brief Arena-style memory manager that allocates string objects from pre-allocated buffers. */
#pragma once
#include <cnt/List.h>
#include <cnt/ListSL.h>
#include <td/String.h>

#include <assert.h>

namespace mem
{
	/// @brief Descriptor for a single allocated character buffer chunk.
	//template <typename T_CHAR>
	class TStrAllocBuff
	{
	public:
		char* buff;        ///< Pointer to the raw character buffer.
        td::UINT4 len;     ///< Length of the buffer in bytes.
//        td::UINT4 capacity;
	};

	/// @brief Arena allocator that manages string objects inside large pre-allocated buffers.
	/// @tparam TUSEDSTRING  String type to allocate (defaults to td::StringExt).
	/// @tparam SINGLEBUFFLEN Minimum size in bytes of each internally allocated buffer chunk (defaults to 64 KB).
	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>
	template <class TUSEDSTRING = td::StringExt, td::UINT4 SINGLEBUFFLEN = 1024 * 64>
	class StringMemoryManager : public cnt::ListSL< TStrAllocBuff, false >
	{
	protected:
		typedef cnt::ListSL<TStrAllocBuff, false> BASE;
		typedef typename BASE::iterator BASE_ITER;
		typedef typename BASE::const_iterator CONST_BASE_ITER;
		typedef typename TUSEDSTRING::CHAR_TYPE T_CHAR;

		/// @brief Intrusive linked-list node embedded at the start of each free chunk.
		typedef struct _tChunk
		{
            struct _tChunk* next; ///< Pointer to the next available chunk in the free list.
			td::UINT4 len;        ///< Remaining usable bytes in this chunk.
		}ChunkLink;

		ChunkLink* _firstAvailableChunk; ///< Head of the singly-linked list of free chunks.
		TUSEDSTRING _zeroLenStr;         ///< Sentinel empty-string instance used for size calculations.
		const int SPACE_FOR_SIZE;        ///< Bytes required to store the string length prefix, derived from the string type.

	protected:
		/// @brief Allocates a new raw buffer of at least minLen bytes and links it into the chunk list.
		/// @param minLen  Minimum number of bytes required.
		/// @param pPrevLink Optional predecessor chunk whose next pointer will be updated.
		/// @return Pointer to the new ChunkLink header embedded in the allocated buffer.
		inline ChunkLink* allocateBuffer(td::UINT4 minLen, ChunkLink* pPrevLink = nullptr)
		{
			if (minLen < SINGLEBUFFLEN)
				minLen = SINGLEBUFFLEN;
			//TStrAllocBuff  = new T_CHAR[minLen];
			//_currentLast = _current + minLen;
			TStrAllocBuff& chunk(BASE::push());
            chunk.buff = new char[minLen];
			chunk.len = minLen;
//            chunk.capacity = minLen;

			ChunkLink* chunkLink = (ChunkLink*)chunk.buff;
			chunkLink->len = chunk.len;
			chunkLink->next = nullptr;
			if (pPrevLink)
				pPrevLink->next = chunkLink;

			return chunkLink;
		}

		/// @brief Walks the free-chunk list to find the first chunk large enough for len bytes.
		///        Allocates a new chunk if none is suitable.
		/// @param len Number of bytes needed.
		/// @return Pointer to the start of a suitable region within a free chunk.
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

		/// @brief Default constructor; initialises the allocator with no pre-allocated buffers.
		StringMemoryManager()
			: _firstAvailableChunk(nullptr)
			, _zeroLenStr()
			, SPACE_FOR_SIZE(_zeroLenStr.spaceForSize())
		{
		}

		/// @brief Allocates a new TUSEDSTRING object of nChars characters from the arena.
		/// @param nChars   Number of characters the string must hold (excluding null terminator).
		/// @param pInStr   Optional source characters to copy into the new string.
		/// @return Pointer to the newly allocated string object, or nullptr if nChars is zero.
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

		/// @brief Allocates a new TUSEDSTRING object from an existing td::String.
		/// @param str Source string whose content is copied into the newly allocated object.
		/// @return Pointer to the newly allocated string object.
        inline TUSEDSTRING* alloc(const td::String& str)
        {
            return allocObject(str.length(), str.c_str());
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

		/// @brief Allocates arena storage for str and initialises it with up to nChars characters from pInStr.
		/// @param str     Output string whose internal buffer pointer is set to the allocated region.
		/// @param nChars  Number of characters to accommodate.
		/// @param pInStr  Optional source characters to copy into the allocated buffer.
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

			TUSEDSTRING& retStr = reinterpret_cast<TUSEDSTRING&>(pBuff);
			//retStr.initDataHolderOnAllocator(pBuff, byteDataLen, pInStr);
			retStr.initDataHolderOnAllocator(byteDataLen, pInStr);

			str.buffer = pBuff;
			//return retStr;
		}

		/// @brief Allocates arena storage for str, prepending chLead before the nChars characters from pInStr.
		/// @param str     Output string whose internal buffer pointer is set to the allocated region.
		/// @param nChars  Number of characters from pInStr to accommodate (chLead adds one extra).
		/// @param pInStr  Source characters to copy after the leading character.
		/// @param chLead  Leading character prepended to the allocated string content.
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

            TUSEDSTRING& retStr = reinterpret_cast<TUSEDSTRING&>(pBuff);
            //retStr.initDataHolderOnAllocator(pBuff, byteDataLen, pInStr);
            retStr.initDataHolderOnAllocator(byteDataLen, pInStr, chLead);

            str.buffer = pBuff;
            //return retStr;
        }

		/// @brief Destructor; releases all allocated buffer memory.
		~StringMemoryManager()
		{
			clean();
		}

//        void reset()
//        {
//            _firstAvailableChunk = nullptr;
//            TStrAllocBuff* firstBuff = BASE::firstPtr();
//            if (firstBuff)
//            {
//                _firstAvailableChunk = firstBuff->buff;
//                _firstAvailableChunk->len = firstBuff->capacity;
//            }
//        }

		/// @brief Frees all allocated buffers and resets the allocator to its initial empty state.
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
            _firstAvailableChunk = nullptr;
		}
	};
}
