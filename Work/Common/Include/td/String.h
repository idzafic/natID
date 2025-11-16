// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cstdarg>
#include <td/Types.h>
#include <mu/UTFConverter.h>
#include <mu/Markers.h>
#include <mu/Regionals.h>
#include <td/BufferString.h>
#include <set>
#include <cmath>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cctype>
#include <td/Conv.h>
#include <tuple>
#include <mem/Utils.h>
#include <filesystem>

//forward declarations for friend classes
#pragma region ForwardDecls
namespace xml
{
	template <class TBUFF, unsigned int NODE_AND_ATTRIB_HASH_SIZE, bool FROM_MEMORY>
	class DOMParser;

	template <unsigned int NODE_AND_ATTRIB_HASH_SIZE>
	class ConfigReaderT;
}

namespace arch
{
	class ArchiveIn;
}
namespace mu
{
	class EnumSerializer;
}

namespace cnt
{
	template <class TUSEDSTRING, td::UINT4 MINBUFFLEN>
	class StringBuilder;
}

namespace mem
{
	template <class TUSEDSTRING, td::UINT4 SINGLEBUFFLEN>
	class StringMemoryManager;
}


class QString;
class QByteArray;

#pragma endregion ForwardDecls

namespace td
{	
#define TD_STR_SHIFT_REF 28
#define TD_STR_EXTRACT_REF 0xF0000000
#define TD_STR_CLEAN_REF   0x0FFFFFFF
#define TD_STR_MAX_REF     15 


	//template<class TSTR>
	//class Ref
	//{
	//public:
	//	bool addReference()
	//	{
	//		return static_cast<TSTR>(this)->addRef();
	//	}

	//	bool releaseReference()
	//	{
	//		return static_cast<TSTR>(this)->relRef();
	//	}
	//};
	class Variant;
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	class StringBase //: public Ref<StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR> >
	{
	protected:
		typedef StringBase<UTF8, td::EncodingUTF8, 4, true> String8Ext;
		typedef StringBase<UTF8, td::EncodingUTF8, 4, false> String8Int;
		typedef StringBase<UTF16, td::EncodingUTF16, 4, false> String16Int;
		typedef StringBase<UTF32, td::EncodingUTF32, 4, false> String32Int;

#pragma region ForwardDeclsIn
		template <class TUSEDSTRING, td::UINT4 MINBUFFLEN> 
		friend class cnt::StringBuilder;

		friend class Variant;

		template <class TBUFF, unsigned int NODE_AND_ATTRIB_HASH_SIZE, bool FROM_MEMORY>
		friend class xml::DOMParser;

		friend class arch::ArchiveIn;

		template <unsigned int NODE_AND_ATTRIB_HASH_SIZE>
		friend class xml::ConfigReaderT;

		friend void UpdatePtr(void* pValue, DataType dataType, const char* strValue, int lenValue, const mu::EnumSerializer* enumSerializer);

		template <class TUSEDSTRING, td::UINT4 SINGLEBUFFLEN>
		friend class mem::StringMemoryManager;

		template <typename T_CHARIN, StringEncoding EncodingIN, int SPACE_FOR_SIZE_IN, bool EXTERN_ALLOCATOR_IN>
		friend class StringBase;
		//friend class Date;
		//friend class Time;
		//friend class DateTime;
#pragma region ForwardDeclsIn

		char* buffer = nullptr;
	public:
		using iterator = T_CHAR*;
		using const_iterator = const T_CHAR*;
		using CHAR_TYPE = T_CHAR;

        constexpr bool isExtern() const
        {
            return EXTERN_ALLOCATOR;
        }
        
		inline iterator begin()
		{
			return (iterator) getBegin();
		}

		inline const_iterator begin() const
		{
			return (const_iterator) getBegin();
		}

		inline const_iterator end() const
		{
			return (const_iterator) getEnd();
		}

		inline td::UINT4 getNumberOfRefs() const
		{
            if constexpr (SPACE_FOR_SIZE != 4)
				return 0;

			if (!buffer)
				return 0;
			td::UINT4* pUINT4 = (td::UINT4*) buffer;
			td::UINT4 refs = (*pUINT4) >> TD_STR_SHIFT_REF;
			return refs;
		}

	protected:	

		void initObjectOnAllocator(T_CHAR* pBuff, size_t byteLen, const char* pInStr = nullptr)
		{
		
			assert(byteLen > sizeof(T_CHAR));

			buffer = pBuff + sizeof(char*);
			
			size_t nToCopy = byteLen - sizeof(T_CHAR);

			resetSize(nToCopy);

			if (pInStr)
			{
				char* pBeg = getBegin();
				
				mem::fastCopy(pBeg, pInStr, nToCopy);
			}
			setTrailingZero(byteLen + SPACE_FOR_SIZE);						
		}

		void initDataHolderOnAllocator(size_t byteLen, const char* pInStr = nullptr)
		{
			//static_assert(EXTERN_ALLOCATOR, "Cannot call this method on normal String");
			assert(EXTERN_ALLOCATOR);
				
			assert(buffer);
			assert(byteLen > sizeof(T_CHAR));

			size_t nToCopy = byteLen - sizeof(T_CHAR);

			resetSize(nToCopy);

			if (pInStr)
			{
				char* pBeg = getBegin();

				mem::fastCopy(pBeg, pInStr, nToCopy);
			}
			setTrailingZero(byteLen + SPACE_FOR_SIZE);			
		}
        
        void initDataHolderOnAllocator(size_t byteLen, const char* pInStr, char chLead)
        {
            //static_assert(EXTERN_ALLOCATOR, "Cannot call this method on normal String");
            assert(EXTERN_ALLOCATOR);
                
            assert(buffer);
            assert(byteLen > sizeof(T_CHAR));

            size_t nToCopy = byteLen - sizeof(T_CHAR);

            resetSize(nToCopy);

            if (pInStr)
            {
                char* pBeg = getBegin();
                pBeg[0] = chLead;
                mem::fastCopy(pBeg+1, pInStr, nToCopy);
            }
            setTrailingZero(byteLen + SPACE_FOR_SIZE);
        }
        
        inline void setRefsToMaximum()
        {
            if constexpr (EXTERN_ALLOCATOR)
                return;
            
            if constexpr (SPACE_FOR_SIZE != 4)
                return; //has to copy
            
            if (!buffer)
                return;
            
            td::UINT4* pUINT4 = (td::UINT4*) buffer;
            td::UINT4 nRefs = (*pUINT4) >> TD_STR_SHIFT_REF;
            if (nRefs == TD_STR_MAX_REF)
                return; //string has to be copied
            nRefs = TD_STR_MAX_REF;
            assert(nRefs <= 15);
            nRefs <<= TD_STR_SHIFT_REF;
            *pUINT4 &= TD_STR_CLEAN_REF;
            *pUINT4 |= nRefs;
        }

		//if false, string has to be copied
		inline bool addRef()
		{
			if constexpr (EXTERN_ALLOCATOR)
				return true;
            
			if constexpr (SPACE_FOR_SIZE != 4)
				return false; //has to copy
			
			if (!buffer)
				return true;

			td::UINT4* pUINT4 = (td::UINT4*) buffer;
			td::UINT4 nRefs = (*pUINT4) >> TD_STR_SHIFT_REF;
			if (nRefs == TD_STR_MAX_REF)
				return false; //string has to be copied
			++nRefs;
			assert(nRefs <= 15);
			nRefs <<= TD_STR_SHIFT_REF;			
			*pUINT4 &= TD_STR_CLEAN_REF;
			*pUINT4 |= nRefs;
			return true;
		}

		inline td::UINT4 getNoOfRefs() const
		{
			if (!buffer)
				return 0;

			td::UINT4* pUINT4 = (td::UINT4*) buffer;
			td::UINT4 nRefs = (*pUINT4) >> TD_STR_SHIFT_REF;
			return nRefs;
		}

		//if true, string can be deleted
		inline bool relRef()
		{
			if constexpr (EXTERN_ALLOCATOR)
				return false;

			if constexpr (SPACE_FOR_SIZE != 4)
				return true; //has to delete

			if (!buffer)
				return false;

			td::UINT4* pUINT4 = (td::UINT4*) buffer;
			td::UINT4 nRefs = (*pUINT4) >> TD_STR_SHIFT_REF;
			if (nRefs == 0)
				return true; //delete
			--nRefs;
			nRefs <<= TD_STR_SHIFT_REF;
			*pUINT4 &= TD_STR_CLEAN_REF;
			*pUINT4 |= nRefs;

			return false;
		}

		inline void cleanBuffer()
		{
			if constexpr (!EXTERN_ALLOCATOR)
			{
				if (buffer)
				{
					if (relRef())
					{
						delete[] buffer;
					}
					buffer = nullptr;
				}
			}			
		}
	public:

		T_CHAR getLastChar() const
		{
            td::UINT4 len = unitLength();
			if (len > 0)
				return getAt(len - 1);
			return (T_CHAR)0;
		}

		inline void duplicate()
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);

			td::UINT4 nRefs = getNumberOfRefs();

			if (nRefs == 0)
				return;

			relRef();

			const T_CHAR* pOldStr = c_str();
			int len = unitLength();
			buffer = 0;
			setString(pOldStr, len);
		}

		void replicate(T_CHAR ch, size_t nReplications)
		{
			reserve(nReplications);
			T_CHAR* pStr = begin();
			for (size_t i=0; i < nReplications; ++i)			
				pStr[i] = ch;
		}

		static inline T_CHAR toUpper(T_CHAR chIn)
		{			
			if ((chIn >= 97) && (chIn <= 122))
			{
				return chIn - 32;
			}
			return chIn;
		}

		static inline T_CHAR toLower(T_CHAR chIn)
		{			
			if ((chIn >= 65) && (chIn <= 90))
			{
				return chIn + 32;
			}
			return chIn;
		}

		bool isInUpperCase() const
		{
            td::UINT4 nLen = unitLength();
			if (nLen == 0)
				return false;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch <'A' || ch > 'Z')
					return false;
			}
			return true;
		}

		bool isInLowerCase() const
		{
            td::UINT4 nLen = unitLength();
			if (nLen == 0)
				return false;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch <'a' || ch > 'z')
					return false;
			}
			return true;
		}

		td::UINT4 isAnyUpperCase() const
		{
            td::UINT4 nLen = unitLength();
			if (nLen == 0)
				return false;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch >'A' && ch < 'Z')
					return true;
			}
			return false;
		}

		td::UINT4 isAnyLowerCase() const
		{
            td::UINT4 nLen = unitLength();
			if (nLen == 0)
				return false;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch >'a' && ch < 'z')
					return true;
			}
			return false;
		}

		inline bool isNull() const
		{
			return (length() == 0);
		}

		bool isNumeric() const
		{
            td::UINT4 nLen = unitLength();
			if (nLen == 0)
				return false;

			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch < '0' || ch > '9')
					return false;
			}
			return true;
		}
	protected:

		inline char* createTMPBuffer(int size) 
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);

			//if (size > 0x0000FFFF)
			//{
			//	int i = 10;                        
			//	++i;
			//}
			assert(size <= 0x0FFFFFFF);

			if (size > 0)
			{
				int nToReserve = size + SPACE_FOR_SIZE;

				if constexpr (Encoding == EncodingUTF8)
				{
					nToReserve += 1;
				}
				else if constexpr (Encoding == EncodingUTF16)
				{
					nToReserve += 2;
				}
				else //if (Encoding == EncodingUTF32)
				{
					nToReserve += 4;
				}				

				char* pBuffer = new char[nToReserve];

				if constexpr (Encoding == EncodingUTF8)
				{
					char* pZero = pBuffer + nToReserve - 1;
					*pZero = (char)0;
				}
				else if constexpr (Encoding == EncodingUTF16)
				{
					char* pZero = pBuffer + nToReserve - 2;
					td::WORD* pZ = (td::WORD*) pZero;
					*pZ = (td::WORD)0;					
				}
				else if constexpr (Encoding == EncodingUTF32)
				{
					//32
					char* pZero = pBuffer + nToReserve - 4;
					td::UINT4* pZ = (td::UINT4*) pZero;
					*pZ = (td::UINT4)0;					
				}

				if constexpr (SPACE_FOR_SIZE == 2)
				{
					td::WORD* pBegin = (td::WORD*) pBuffer;
					*pBegin = (td::WORD) size;
				}
				else if constexpr (SPACE_FOR_SIZE == 4)
				{
					td::UINT4* pBegin = (td::UINT4*) pBuffer;
					*pBegin = (td::UINT4) size;
				}

				return pBuffer;
			}
			return 0;
		}

		inline void resetSize(size_t size)
		{
            if (!buffer)
            {
                if (size == 0)
                    return;
            }
            
			assert(buffer);

			if constexpr (SPACE_FOR_SIZE == 2)
			{
				td::WORD* pBegin = (td::WORD*) buffer;
				td::WORD wsize = (td::WORD) size;
				*pBegin = wsize;
			}
			else if constexpr(SPACE_FOR_SIZE == 4)
			{
                td::UINT4* pUINT4 = (td::UINT4*) buffer;
                td::UINT4 nRefs = (*pUINT4) >> TD_STR_SHIFT_REF;
                assert(nRefs <= 15);
                
                if (nRefs == 0)
                {
                    td::UINT4 sz = (td::UINT4) size;
                    sz &= TD_STR_CLEAN_REF;
                    *pUINT4 = sz;
                    return;
                }
                nRefs <<= TD_STR_SHIFT_REF;
                *pUINT4 &= TD_STR_EXTRACT_REF;
                td::UINT4 sz = (td::UINT4) size;
                sz &= TD_STR_CLEAN_REF;
                *pUINT4 |= sz;
			}
		}
		public:
			bool reduceSize(size_t deltaSize)
			{
				if (getNoOfRefs() != 0)
					return false;				
                size_t currSize = unitLength();
				if (currSize < deltaSize)
					return false;
				currSize -= deltaSize;
				resetSize(currSize);
				T_CHAR* pData = begin();
				pData[currSize] = 0;
				return true;
			}

			bool resetSize(size_t newSize, size_t maxSize)
			{
				if (newSize > maxSize)
					return false;
				resetSize(newSize);
				return true;
			}
		protected:

		inline void setTrailingZero(size_t nToReserve)
		{
			if constexpr (Encoding == EncodingUTF8)
			{
				char* pZero = buffer + nToReserve - 1;
				*pZero = (char)0;
			}
			else if constexpr (Encoding == EncodingUTF16)
			{
				char* pZero = buffer + nToReserve - 2;
				td::WORD* pZ = (td::WORD*) pZero;
				*pZ = (td::WORD)0;
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				//32
				char* pZero = buffer + nToReserve - 4;
				td::UINT4* pZ = (td::UINT4*) pZero;
				*pZ = (td::UINT4)0;
			}
		}
		//input is only space required for characters without zero ch and space for size
		inline bool prepareBuffer(int size) 
		{			
			//assert(size <= 0x0000FFFF);
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();

			if (size <= 0)
				return false;

			if (size > 0)
			{
				size_t nToReserve = size + SPACE_FOR_SIZE;

				//add space for zero character
				if constexpr (Encoding == EncodingUTF8)
				{
					nToReserve += 1;
				}
				else if constexpr (Encoding == EncodingUTF16)
				{
					nToReserve += 2;
				}
				else if constexpr (Encoding == EncodingUTF32)
				{
					nToReserve += 4;
				}				

				buffer = new char[nToReserve];
                
                if constexpr (SPACE_FOR_SIZE == 4)
                {
                    td::UINT4* pUint4 = (td::UINT4*) buffer;
                    *pUint4 = 0;
                }
                else if constexpr (SPACE_FOR_SIZE == 2)
                {
                    td::UINT2* pUint2 = (td::UINT2*) buffer;
                    *pUint2 = 0;
                }

				setTrailingZero(nToReserve);

				resetSize(size);

				return true;
			}
			return false;
		}

        char* getBegin() const
		{
			if (buffer)
			{
				return buffer + SPACE_FOR_SIZE;				
			}
			return 0;
		}

        char* getEnd() const
		{
			if (buffer)
			{
				int size = length();
				//if (Encoding == EncodingUTF8)
				//{
				//	size -= 1;
				//}
				//else if (Encoding == EncodingUTF16)
				//{
				//	size -= 2;
				//}
				//else
				//{
				//	size -= 4;
				//}			

				return buffer + SPACE_FOR_SIZE + size;

			}
			return 0;
		}

		template <typename TINPUTENCODING>
		inline void appendString(const TINPUTENCODING* pStr, int inputLen)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);

			duplicate();
			

			int nOldLen = length();
			if (nOldLen == 0)
			{
				setString(pStr, inputLen);
				return;
			}

			int bytesNeeded;
			bool bDeleteString;
			const char* pTmp = createTmpString(pStr, inputLen, bytesNeeded, bDeleteString);
			if (pTmp)
			{				
				int nTotalSize = bytesNeeded + nOldLen;
				char* pFinal = createTMPBuffer(nTotalSize);

				int sizeOffset = SPACE_FOR_SIZE;

				if (pFinal)
				{
					memcpy((void*) (pFinal + sizeOffset), (void*) (buffer + sizeOffset), nOldLen);
					memcpy((void*) (pFinal + nOldLen + sizeOffset), (void*) pTmp, bytesNeeded);
					if (buffer)
					{
						delete [] buffer;
						buffer = pFinal;
					}
				}
				if (bDeleteString)
				{
					delete [] pTmp;
				}
			}
		}

		//funkcija je sada template tako da radi
		//inline void appendString(const wchar_t* pStr, int inputLen)
		//{
		//	int nOldLen = length();
		//	if (nOldLen == 0)
		//	{
		//		setString(pStr, inputLen);
		//		return;
		//	}
		//	
		//	int bytesNeeded;
		//	bool bDeleteString;
		//	const char* pTmp = createTmpString(pStr, inputLen, bytesNeeded, bDeleteString);
		//	if (pTmp)
		//	{				
		//		int nTotalSize = bytesNeeded + nOldLen;
		//		char* pFinal = createTMPBuffer(nTotalSize);

		//		int sizeOffset = 0;
		//		if (sizeInside)
		//			sizeOffset = SPACE_FOR_SIZE;

		//		if (pFinal)
		//		{
		//			memcpy((void*) (pFinal + sizeOffset), (void*) (buffer + sizeOffset), nOldLen);
		//			memcpy((void*) (pFinal + nOldLen + sizeOffset), (void*) pTmp, bytesNeeded);
		//			if (buffer)
		//			{
		//				delete [] buffer;
		//				buffer = pFinal;
		//			}
		//		}
		//		if (bDeleteString)
		//		{
		//			delete [] pTmp;
		//		}
		//	}
		//}

#ifdef WINDOWS_UNICODE
		static inline int getStrLength(const UTF32* pStr)
		{
			if (!pStr)
				return 0;
			const UTF32* pStrTmp = pStr; //iLen = 0;
			while (*pStrTmp != 0) //iLen < 0x0000FFFF 				
				++pStrTmp;
			return pStrTmp - pStr;
		}

		inline void copyString(UTF32* pStrDestination, const UTF32* pStr, int destinationLen) const
		{
			int i = 0;
			while ( (i < destinationLen) && (*pStr != 0) ) //iLen < 0x0000FFFF 				
			{
				*pStrDestination = *pStr;
				++pStrDestination;
				++pStr;
				++i;
			}			
		}

		inline void setString(const UTF32* pStr, int inputLen)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (Encoding == EncodingUTF16)
			{
				//32 -> 16
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF32toUTF16(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF16* pBeg = (mu::UTF16*)getBegin();
					mu::UTF16* pEnd = (mu::UTF16*)getEnd();
					mu::UTFConverter::convertUTF32toUTF16(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				// 32 -> 32
                int nLen = inputLen * 4;
				if (prepareBuffer(nLen))
				{
					UTF32* pBeg = (UTF32*)getBegin();
					copyString(pBeg, pStr, inputLen);
				}
			}
			else if constexpr (Encoding == EncodingUTF8) //UTF8
			{
				//32 -> 8
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF32toUTF8(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF8* pBeg = (mu::UTF8*)getBegin();
					mu::UTF8* pEnd = (mu::UTF8*)getEnd();
					mu::UTFConverter::convertUTF32toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
            
            assert(false);
//#ifdef _DEBUG
//			txt = (T_CHAR*) getBegin();
//			txtUnitLen = unitLength();
//#endif
		}

//#ifdef _DEBUG
//		void initDebugInfo()
//		{
//			txt = (T_CHAR*) getBegin();
//			txtUnitLen = unitLength();
//		}
//#endif

		inline const char* createTmpString(const UTF32* pStr, int inputLen, int& bytesNeeded, bool& bDeleteString)
		{
			if constexpr (Encoding == EncodingUTF16)
			{
				//32 -> 16
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF32toUTF16(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 2];
					mu::UTF16* pBeg = (mu::UTF16*) pTmp;
					mu::UTF16* pEnd = (mu::UTF16*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF32toUTF16(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				// 32 -> 32
				bytesNeeded = inputLen * 4;
				if (bytesNeeded > 0)
				{
					bDeleteString = false;
					return (const char*) pStr;
				}
				return 0;
			}
			else if constexpr (Encoding == EncodingUTF8)
			{
				//32 -> 8
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF32toUTF8(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 2];
					mu::UTF8* pBeg = (mu::UTF8*)pTmp;
					mu::UTF8* pEnd = (mu::UTF8*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF32toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
            assert(false);
            return 0;
		}
#else
		int getStrLength(const UTF16* pStr) const
		{
			if (!pStr)
				return 0;

			const UTF16* pStrTmp = pStr; //iLen = 0;
			while (*pStrTmp != 0) //iLen < 0x0000FFFF 				
				++pStrTmp;
			return int(pStrTmp - pStr);
		}

		inline void copyString(UTF16* pStrDestination, const UTF16* pStr, int destinationLen) const
		{			
			int i = 0;
			while ((i < destinationLen) && (*pStr != 0)) //iLen < 0x0000FFFF 				
			{
				*pStrDestination = *pStr;
				++pStrDestination;
				++pStr;
				++i;
			}
		}

		inline const char* createTmpString(const UTF16* pStr, int inputLen, int& bytesNeeded, bool& bDeleteString)
		{
            if constexpr (Encoding == EncodingUTF16)
			{
				// 16 -> 16				
                bytesNeeded = inputLen * 2;
				if (bytesNeeded > 0)
				{	
					bDeleteString = false;
					return (const char*)pStr;
				}
				return 0;
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				//16 -> 32			
                const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF16toUTF32(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 4];
                    mu::UTF32* pBeg = (mu::UTF32*) pTmp;
                    mu::UTF32* pEnd = (mu::UTF32*) (pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF16toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else  if constexpr (Encoding == EncodingUTF8)
			{
				//16 -> 8
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF16toUTF8(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 1];
                    mu::UTF8* pBeg = (mu::UTF8*)pTmp;
                    mu::UTF8* pEnd = (mu::UTF8*) (pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF16toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
            assert(false);
            return 0;
		}

		inline void setString(const UTF16* pStr, int inputLen)
		{
			if constexpr (Encoding == EncodingUTF16)
			{
				// 16 -> 16
				int nLen = inputLen * 2;
				if (prepareBuffer(nLen))
				{
					td::UTF16* pBeg = (td::UTF16*)getBegin();
					copyString(pBeg, pStr, inputLen);
				}				
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				//16 -> 32			
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF16toUTF32(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
                    mu::UTF32* pBeg = (mu::UTF32*)getBegin();
                    mu::UTF32* pEnd = (mu::UTF32*)getEnd();
					mu::UTFConverter::convertUTF16toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr (Encoding == EncodingUTF8)
			{
				//16 -> 8
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF16toUTF8(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
                    mu::UTF8* pBeg = (mu::UTF8*)getBegin();
                    mu::UTF8* pEnd = (mu::UTF8*)getEnd();
					mu::UTFConverter::convertUTF16toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
            
            assert(false);
//#ifdef _DEBUG
//			txt = (T_CHAR*) getBegin();
//			txtUnitLen = unitLength();
//#endif
		}
#endif
		inline const char* createTmpString(const char* pStr, int inputLen, int& bytesNeeded, bool& bDeleteString)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);

			if constexpr (Encoding == EncodingUTF16)
			{
				//8->16
				const mu::UTF8* sourceStart = (const mu::UTF8*) pStr;
				const mu::UTF8* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF8toUTF16(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 2];
					mu::UTF16* pBeg = (mu::UTF16*) pTmp;
					mu::UTF16* pEnd = (mu::UTF16*) (pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF8toUTF16(sourceStart, sourceEnd, pBeg, pEnd);					
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr(Encoding == EncodingUTF32)
			{
				//8->32
				const mu::UTF8* sourceStart = reinterpret_cast<const mu::UTF8*>(pStr);
				const mu::UTF8* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF8toUTF32(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 4];
					mu::UTF32* pBeg = (mu::UTF32*) pTmp;
					mu::UTF32* pEnd = (mu::UTF32*) (pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF8toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr(Encoding == EncodingUTF8)
			{
				//8->8
				bytesNeeded = inputLen;
				if (bytesNeeded > 0)
				{
					bDeleteString = false;
					return pStr;
				}
				return 0;
			}
            assert(false);
            return 0;
		}

		inline void setString(const char* pStr, size_t inputLen)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (Encoding == EncodingUTF16)
			{
				const mu::UTF8* sourceStart = reinterpret_cast<const mu::UTF8*>(pStr);
				const mu::UTF8* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF8toUTF16(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF16* pBeg = (mu::UTF16*)getBegin();
					mu::UTF16* pEnd = (mu::UTF16*)getEnd();
					mu::UTFConverter::convertUTF8toUTF16(sourceStart, sourceEnd, pBeg, pEnd);					
				}
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				const mu::UTF8* sourceStart = reinterpret_cast<const mu::UTF8*>(pStr);
				const mu::UTF8* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF8toUTF32(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF32* pBeg = (mu::UTF32*)getBegin();
					mu::UTF32* pEnd = (mu::UTF32*)getEnd();
					mu::UTFConverter::convertUTF8toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr (Encoding == EncodingUTF8)
			{
				// UTF8 -> UTF8
				if (prepareBuffer((int) inputLen))
				{
					char* pBeg = getBegin();
					//strcpy(pBeg, pStr);
					memcpy((void*)pBeg, (void*)pStr, inputLen);
				}
			}

//#ifdef _DEBUG
//			txt = (T_CHAR*) getBegin();
//			txtUnitLen = unitLength();
//#endif
		}

        inline const char* createTmpString(const wchar_t* pStr, int inputLen, int& bytesNeeded, bool& bDeleteString)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
            if constexpr ((Encoding == EncodingUTF8) && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//16 -> 8
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF16toUTF8(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 1];
					mu::UTF8* pBeg = (mu::UTF8*)pTmp;
					mu::UTF8* pEnd = (mu::UTF8*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF16toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr ((Encoding == EncodingUTF8) && (_G_SIZE_OF_WCHAR_ == 4))
			{
				// 32 -> 8
				// linux
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF32toUTF8(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 1];
					mu::UTF8* pBeg = (mu::UTF8*)pTmp;
					mu::UTF8* pEnd = (mu::UTF8*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF32toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr ( (Encoding == EncodingUTF16) && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//windows
				// 16 -> 16
				bytesNeeded = inputLen * _G_SIZE_OF_WCHAR_;
				if (bytesNeeded > 0)
				{
					bDeleteString = false;
					return (const char*)pStr;
				}
				return 0;
			}
			else if constexpr ( (Encoding == EncodingUTF32)  && (_G_SIZE_OF_WCHAR_ == 4))
			{
				//32 -> 32
				//linux & OSX
				bytesNeeded = inputLen * _G_SIZE_OF_WCHAR_;
				if (bytesNeeded > 0)
				{
					bDeleteString = false;
					return (const char*) pStr;
				}
				return 0;
			}			
			else if constexpr ( (Encoding == EncodingUTF32)  && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//16 -> 32
				//Windows
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF16toUTF32(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 4];
					mu::UTF32* pBeg = (mu::UTF32*)pTmp;
					mu::UTF32* pEnd = (mu::UTF32*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF16toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
			else if constexpr ( (Encoding == EncodingUTF16) && (_G_SIZE_OF_WCHAR_ == 4))
			{
				//linux
				//32 -> 16
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				bytesNeeded = mu::UTFConverter::measureUTF32toUTF16(sourceStart, sourceEnd);

				if (bytesNeeded > 0)
				{
					char* pTmp = new char[bytesNeeded + 2];
					mu::UTF16* pBeg = (mu::UTF16*)pTmp;
					mu::UTF16* pEnd = (mu::UTF16*)(pTmp + bytesNeeded);
					mu::UTFConverter::convertUTF32toUTF16(sourceStart, sourceEnd, pBeg, pEnd);
					*pEnd = 0;
					bDeleteString = true;
					return pTmp;
				}
				return 0;
			}
            
            assert(false);
            return 0;
		}

		inline void setString(const wchar_t* pStr, int inputLen)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr ((Encoding == EncodingUTF8) && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//16 -> 8
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF16toUTF8(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF8* pBeg = (mu::UTF8*)getBegin();
					mu::UTF8* pEnd = (mu::UTF8*)getEnd();
					mu::UTFConverter::convertUTF16toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr ((Encoding == EncodingUTF8) && (_G_SIZE_OF_WCHAR_ == 4))
			{
				// 32 -> 8
				// linux
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF32toUTF8(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF8* pBeg = (mu::UTF8*)getBegin();
					mu::UTF8* pEnd = (mu::UTF8*)getEnd();
					mu::UTFConverter::convertUTF32toUTF8(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr ( (Encoding == EncodingUTF16) && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//windows
				// 16 -> 16
				int nLen = inputLen * _G_SIZE_OF_WCHAR_;
				if (prepareBuffer(nLen))
				{
					wchar_t* pBeg = (wchar_t*)getBegin();
					WMEMCPYS(pBeg, inputLen+1, pStr, inputLen); //OVO JE OK
				}
			}
			else if constexpr ( (Encoding == EncodingUTF32)  && (_G_SIZE_OF_WCHAR_ == 4))
			{
				//32 -> 32
				//linux & OSX
				int nLen = inputLen * _G_SIZE_OF_WCHAR_;
				if (prepareBuffer(nLen))
				{
					wchar_t* pBeg = (wchar_t*)getBegin();
					WMEMCPYS(pBeg, inputLen+1, pStr, inputLen);
				}
			}
			else if constexpr ( (Encoding == EncodingUTF32)  && (_G_SIZE_OF_WCHAR_ == 2))
			{
				//16 -> 32
				//Windows
				const mu::UTF16* sourceStart = reinterpret_cast<const mu::UTF16*>(pStr);
				const mu::UTF16* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF16toUTF32(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF32* pBeg = (mu::UTF32*)getBegin();
					mu::UTF32* pEnd = (mu::UTF32*)getEnd();
					mu::UTFConverter::convertUTF16toUTF32(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else if constexpr ( (Encoding == EncodingUTF16) && (_G_SIZE_OF_WCHAR_ == 4))
			{
				//linux
				//32 ->16
				const mu::UTF32* sourceStart = reinterpret_cast<const mu::UTF32*>(pStr);
				const mu::UTF32* sourceEnd = sourceStart + inputLen;

				int nLen = mu::UTFConverter::measureUTF32toUTF16(sourceStart, sourceEnd);

				if (prepareBuffer(nLen))
				{
					mu::UTF16* pBeg = (mu::UTF16*)getBegin();
					mu::UTF16* pEnd = (mu::UTF16*)getEnd();
					mu::UTFConverter::convertUTF32toUTF16(sourceStart, sourceEnd, pBeg, pEnd);
				}
			}
			else
			{
				assert(false);
			}
            
            //assert(false);
//#ifdef _DEBUG
//			txt = (T_CHAR*) getBegin();
//			txtUnitLen = unitLength();
//#endif
		}

		template <typename TCH>
		static inline int compareStrings(const TCH* pMy, const TCH*pOut, int nLen)
		{			
			while (nLen > 0)
			{
				if (*pMy < *pOut)
					return -1;

				if (*pMy > *pOut)
					return 1;

				++pMy;
				++pOut;

				--nLen;
			}
			return 0;
		}
	public:
		~StringBase()			
		{
			if constexpr (!EXTERN_ALLOCATOR)
				cleanBuffer();
		}

		inline StringBase()
			: buffer(nullptr)
		{
		}


		inline StringBase(const char* pStr)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				if (pStr != nullptr)
				{
					size_t inputLen = strlen(pStr);
					setString(pStr, inputLen);
				}
			}						
		}

		inline StringBase(const char* pStr, size_t inputLen)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				if (pStr != nullptr)
				{				
					setString(pStr, inputLen);
				}
			}
		}

		inline StringBase(const std::string& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				
				int inputLen = (int)str.length();
				if (inputLen > 0)
					setString(str.c_str(), inputLen);				
			}
		}

		inline StringBase(const std::wstring& str)
			: buffer(0)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				//if (pStr != nullptr)
				{
					int inputLen = (int)str.length();
					setString(str.c_str(), inputLen);
				}
			}
		}
        
        inline StringBase(const std::filesystem::path& path)
            : buffer(nullptr)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
            assert(!EXTERN_ALLOCATOR);
            if constexpr (!EXTERN_ALLOCATOR)
            {
                auto str = path.string();
                auto strLen = str.length();
                if (strLen > 0)
                    setString(str.c_str(), strLen);
            }
        }

		inline StringBase(const QString& str);
		//	: buffer(0)
		//{
		//	static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
		//	assert(!EXTERN_ALLOCATOR);
		//	if (!EXTERN_ALLOCATOR)
		//	{	
		//		QByteArray str8(str.toUtf8());
		//		size_t inputLen = str8.length();
		//		if (inputLen != 0)
		//		{					
		//			setString(str8.data(), inputLen);					
		//		}
		//	}
		//}


		
		inline StringBase(const FileString8& buf)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				fromKnownString(buf.c_str(), buf.length());
			}
		}

		inline StringBase(const FileBuffer1k& buf)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				fromKnownString(buf.c_str(), buf.length());
			}
		}

		inline StringBase(const FileBuffer4k& buf)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				fromKnownString(buf.c_str(), buf.length());
			}
		}

		inline StringBase(const FileBuffer16k& buf)
			: buffer(0)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				fromKnownString(buf.c_str(), buf.length());
			}
		}

		inline StringBase(const wchar_t* pStr)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				int inputLen = (int)wcslen(pStr);
				setString(pStr, inputLen);
			}
		}

#ifdef WINDOWS_UNICODE
		inline StringBase(const UTF32* pStr)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				int inputLen = getStrLength(pStr);
				setString(pStr, inputLen);
			}
		}

		

#else
		inline StringBase(const UTF16* pStr)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (!EXTERN_ALLOCATOR)
			{
				int inputLen = getStrLength(pStr);
				setString(pStr, inputLen);
			}
		}
#endif

		//#pragma region Copy constructors

		template <bool EXTERN_ALLOCATOR_INSTR>
		void clone(const StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR_INSTR>& inStr)
		{	
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			fromKnownString(inStr.c_str(), inStr.unitLength());
		}
        
        [[nodiscard]] StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> clone() const
        {
            StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> toRet;
            toRet.clone(*this);
            return toRet;
        }

		inline const char* getBuffer() const
		{
			return buffer;
		}


		inline StringBase(const String8Int& str)
			: buffer(nullptr)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

            if constexpr (EXTERN_ALLOCATOR)
            {
                buffer = str.buffer;
            }
            
			if constexpr (!EXTERN_ALLOCATOR)
			{
				if constexpr (Encoding == td::EncodingUTF8)
				{
					//StringBase<UTF8, td::EncodingUTF8, 4>& str2 = const_cast<StringBase<UTF8, td::EncodingUTF8, 4>&> (str);
					buffer = const_cast<char*> (str.getBuffer());
					if (addRef())
					{
						return;
					}
					buffer = nullptr;
				}
				const UTF8* pTxt = str.c_str();
				int inputLen = str.unitLength();
				setString(pTxt, inputLen);
			}
		}
				
		inline StringBase(const String8Ext& str)
			: buffer(nullptr)
		{
			//IDz: ovaj se moze zvati svakako
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");			
			if constexpr (!EXTERN_ALLOCATOR)
			{
				if constexpr (Encoding == td::EncodingUTF8)
				{
					StringBase strTmp;
					strTmp.fromKnownString(str.c_str(), str.length());
					strTmp.addRef();					
					buffer = strTmp.buffer;
				}
				const UTF8* pTxt = str.c_str();
				int inputLen = str.unitLength();
				setString(pTxt, inputLen);
			}
			else
				buffer = const_cast<char*>(str.getBuffer());
		}

		//inline StringBase(const StringBase* pStr)
		//	: buffer(nullptr)
		//{
		//	takeFromAlloc(pStr);
		//}
        
        // Move constructor
        inline StringBase(StringBase&& str) noexcept
            : buffer(str.buffer)
        {
            if (!str.isExtern())
                str.buffer = nullptr;
        }
        
        // Move assignment operator
        inline StringBase& operator=(StringBase&& str) noexcept
        {
            if (this != &str)
            {
                relRef();
                buffer = str.buffer;
                if (!str.isExtern())
                    str.buffer = nullptr;
            }
            return *this;
        }
        
		inline StringBase(const String16Int& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);

			if constexpr (Encoding == td::EncodingUTF16)
			{
				buffer = const_cast<char*> (str.getBuffer());
				if (addRef())
				{
					return;
				}
				buffer = nullptr;
			}
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const String32Int& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if constexpr (Encoding == td::EncodingUTF32)
			{
				buffer = const_cast<char*> (str.getBuffer());
				if (addRef())
				{
					return;
				}
				buffer = nullptr;
			}

			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF8, td::EncodingUTF8, 2, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF16, td::EncodingUTF16, 2, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF32, td::EncodingUTF32, 2, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF8, td::EncodingUTF8, 0, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF16, td::EncodingUTF16, 0, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}

		inline StringBase(const StringBase<UTF32, td::EncodingUTF32, 0, false>& str)
			: buffer(nullptr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
		}
		//#pragma endregion

		//#pragma region Assignment

		template <typename TOUTSTR>
		inline void fromKnownString(const TOUTSTR* str, size_t inputUnitLen)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			if (inputUnitLen < 0)
				inputUnitLen = 0;

			setString(str, (int) inputUnitLen);
		}

		//CT- string known at compile time (CT)
		template <typename TINCH, size_t size>
		inline void fromCTString(const TINCH(&ctStrIn)[size])
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			setString(ctStrIn, (int)size - 1);
		}


		template <typename TSTRARR, typename TSTRBUILDER>
		inline void fromStringArray(const TSTRARR& arr, TSTRBUILDER& strBuilder, const T_CHAR* pStrSep = ", ", bool appendEmpty = false)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			strBuilder.reset();
			strBuilder.appendStringArray(arr, pStrSep, appendEmpty);			
			strBuilder.getString(*this);
		}
		
		//void fromUTF8(const td::UTF8* pStr, int nLen=-1)
		//{
		//	if (nLen < 0)
		//	{
		//		if (pStr)
		//		{
		//			nLen = (int) strlen(pStr);
		//		}
		//		else 
		//			nLen = 0;
		//	}
		//	setString(pStr, nLen);
		//}

		//void fromUTF16(const td::UTF16* pStr, int nLen)
		//{
		//	setString(pStr, nLen);
		//}

		//void fromUTF16(const td::UTF32* pStr, int nLen)
		//{
		//	setString(pStr, nLen);
		//}

		inline int toInt() const
		{
			return atoi(c_str());
		}

		inline float toFloat() const
		{
			float toRet = (float)mu::toDouble(c_str()); 
			return toRet;
		}

		inline double toDouble() const
		{
			double toRet = mu::toDouble(c_str()); 
			return toRet;
		}	

		inline td::cmplxf toCmplxF() const
		{
			std::istringstream ss(c_str());
			ss.imbue(std::locale("C"));
			td::cmplxf val;
			ss >> val;
			return val;
		}

		inline td::cmplx toCmplx() const
		{
			std::istringstream ss(c_str());
			ss.imbue(std::locale("C"));
			td::cmplx val;
			ss >> val;
			return val;
		}

		inline td::LUINT8 toUint8() const
		{
			return td::toUint8( c_str() );
		}

		inline td::LUINT8 toInt8() const
		{
			return td::toInt8( c_str() );
		}

		inline QString toQString() const;		

		inline void fromNumber(int val)
		{
			cleanBuffer();
			char tmp[16];
            int nLen = SNPRINTF(tmp, 16, _TRUNCATE, "%d", val);
			fromKnownString(tmp, nLen);
		}

		inline void fromNumber(unsigned int val)
		{
			cleanBuffer();
			char tmp[16];
            int nLen = SNPRINTF(tmp, 16, _TRUNCATE, "%u", val);
			fromKnownString(tmp, nLen);
		}

		inline void fromNumber(float val)
		{
			cleanBuffer();
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"
			char tmp[16];
            int nLen = SNPRINTF(tmp, 16, _TRUNCATE, "%f", val);
			fromKnownString(tmp, nLen);
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
		}

		inline void fromNumber(double val)
		{
			cleanBuffer();
			char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
			setlocale(LC_NUMERIC, "C");  // Set locale to "C"
			char tmp[24];
            int nLen = SNPRINTF(tmp, 24, _TRUNCATE,"%g", val);
			fromKnownString(tmp, nLen);
			setlocale(LC_NUMERIC, old_locale);  // Restore original locale
		}

		inline void fromNumber(td::LUINT8 val)
		{
			cleanBuffer();
			char tmp[24];
            int nLen = SNPRINTF(tmp, 24, _TRUNCATE, FMT_LUINT8, val);
			fromKnownString(tmp, nLen);
		}

		inline void fromNumber(td::LINT8 val)
		{
			cleanBuffer();
			char tmp[24];
            int nLen = SNPRINTF(tmp, 24, _TRUNCATE, FMT_LUINT8, val);
			fromKnownString(tmp, nLen);
		}

        inline StringBase& operator = (const std::filesystem::path& path)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
            cleanBuffer();

            return operator = (path.string());
        }
        
		inline StringBase& operator = (const char* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();

			int nLen = 0;
			if (pStr)
				nLen = (int) strlen(pStr);
			setString(pStr, nLen);
			return *this;
		}

		template <size_t size>
		inline StringBase& operator = (const char(&message)[size])
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();

			int nLen = (int) size - 1;
			setString(&message[0], nLen);
			return *this;
		}
		
		inline StringBase& operator = (const wchar_t* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();
			int nLen = 0;
			if (pStr)
				nLen = (int) wcslen(pStr);			
			setString(pStr, nLen);
			return *this;
		}

		template <size_t size>
		inline StringBase& operator = (const wchar_t(&message)[size])
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();

			int nLen = (int)size - 1;
			setString(&message[0], nLen);
			return *this;
		}

		inline StringBase& operator += (const char* pStr)
		{				
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			if (!pStr)
				return *this;
			duplicate();

			int inputLen = (int) strlen(pStr);
			appendString(pStr, inputLen);
			return *this;
		}

		inline StringBase& operator += (const wchar_t* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			if (!pStr)
				return *this;
			duplicate();
			int inputLen = (int) wcslen(pStr);
			appendString(pStr, inputLen);
			return *this;
		}
        
        template <class TSTRIN>
        inline void append(const TSTRIN& strIn)
        {
            this->operator += (strIn);
        }

#ifdef WINDOWS_UNICODE
		inline StringBase& operator = (const UTF32* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();
			int inputLen = getStrLength(pStr);
			setString(pStr, inputLen);
			return *this;
		}

		inline void operator += (const UTF32* pStr)
		{
			duplicate();
			int inputLen = getStrLength(pStr);
			appendString(pStr, inputLen);
		}
#else

		inline void operator = (const UTF16* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			ssize_t inputLen = getStrLength(pStr);
			setString(pStr, (int) inputLen);
		}

		inline void operator += (const UTF16* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			int inputLen = getStrLength(pStr);
			appendString(pStr, inputLen);
		}
#endif

		inline float toSingle() const
		{
			float toRet = (float)mu::toDouble(c_str()); 
			return toRet;
		}

		inline td::INT4 toINT4() const
		{
			return (td::INT4) atoi(c_str());
		}

		inline td::UINT4 toUINT4() const
		{
			return (td::UINT4) atoi(c_str());
		}

		inline td::WORD toWORD() const
		{
			return (td::WORD) atoi(c_str());
		}

		inline td::BYTE toBYTE() const
		{
			return (td::BYTE) atoi(c_str());
		}

		inline td::INT2 toINT2() const
		{
			return (td::INT2) atoi(c_str());
		}

		inline td::UINT4 HexToDec() const
		{
			const char* str = c_str();
			td::UINT4 res = 0;
			while (*str != '\0')
			{
				char ch = *str;
				td::UINT4 hexDigit = 0;
				if ( (ch >= '0') && (ch <= '9') )
				{
					hexDigit = ch - '0';
				}
				else if ( (ch >= 'a') && (ch <= 'f') )
				{
					hexDigit = ch - 'a' + 10;
				}
				else
					return res;
				res = res << 4;
				res |= hexDigit;
				++str;
			}
			return res;
		}

		inline bool toBoolean() const
		{
			//if (cCompareNoCase("true") == 0)
				//return true;
			return td::toBoolean(c_str(), (int) length());
			//return false;
		}
				
		inline StringBase& operator = (const String8Int& str)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			if constexpr (!EXTERN_ALLOCATOR)
			{
                if (buffer == str.buffer)
                    return *this;
				cleanBuffer();
				if constexpr (Encoding == td::EncodingUTF8)
				{
					//StringBase<UTF8, td::EncodingUTF8, 4>& str2 = const_cast<StringBase<UTF8, td::EncodingUTF8, 4>&> (str);
					buffer = const_cast<char*> (str.getBuffer());
					if (addRef())
					{
						return *this;
					}
					buffer = nullptr;
				}
				const UTF8* pTxt = str.c_str();
				int inputLen = str.unitLength();
				setString(pTxt, inputLen);
			}
			else
            {
                buffer = str.buffer;
            }
			return *this;
		}

		inline StringBase& operator = (const String8Ext& str)
		{			
			if constexpr (!EXTERN_ALLOCATOR)
			{
				cleanBuffer();
				if constexpr (Encoding == td::EncodingUTF8)
				{
					StringBase strTmp;
					strTmp.fromKnownString(str.c_str(), str.length());
					strTmp.addRef();
					cleanBuffer();
					buffer = strTmp.buffer;
				}
				const UTF8* pTxt = str.c_str();
				int inputLen = str.unitLength();
				setString(pTxt, inputLen);
			}
			else
				buffer = const_cast<char*>(str.getBuffer());
			return *this;
		}

		//template <bool EXTERN_ALLOCATOR_INSTR>
		//inline void operator = (const StringBase<UTF8, td::EncodingUTF8, 4, EXTERN_ALLOCATOR_INSTR>& str)
		//{			
		//	if (EXTERN_ALLOCATOR == EXTERN_ALLOCATOR_INSTR)
		//	{
		//		if (!EXTERN_ALLOCATOR)					
		//		{
		//			cleanBuffer();
		//			if (Encoding == td::EncodingUTF8)
		//			{
		//				//StringBase<UTF8, td::EncodingUTF8, 4>& str2 = const_cast<StringBase<UTF8, td::EncodingUTF8, 4>&> (str);
		//				buffer = const_cast<char*> (str.getBuffer());
		//				if (addRef())
		//				{
		//					return;
		//				}
		//				buffer = nullptr;
		//			}
		//			const UTF8* pTxt = str.c_str();
		//			int inputLen = str.unitLength();
		//			setString(pTxt, inputLen);
		//		}
		//		else
		//		{
		//			//both are with extern allocator, no need to keep track on reference
		//			buffer = const_cast<char*>(str.getBuffer());
		//		}
		//	}
		//	else if (EXTERN_ALLOCATOR_INSTR)
		//	{
		//		//this is not with extern allocator
		//		StringBase strTmp;
		//		strTmp.fromKnownString(str.c_str(), str.length());
		//		strTmp.addRef();
		//		cleanBuffer();
		//		buffer = strTmp.buffer;
		//	}
		//	else
		//		assert(false);
		//}

		inline StringBase& operator = (const StringBase<UTF16, td::EncodingUTF16, 4, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			if constexpr (Encoding == td::EncodingUTF16)
			{
				//StringBase<UTF8, td::EncodingUTF8, 4>& str2 = const_cast<StringBase<UTF8, td::EncodingUTF8, 4>&> (str);
				buffer = const_cast<char*> (str.getBuffer());
				if (addRef())
				{
					return *this;
				}
				buffer = nullptr;
			}
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF32, td::EncodingUTF32, 4, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			if constexpr (Encoding == td::EncodingUTF32)
			{
				//StringBase<UTF8, td::EncodingUTF8, 4>& str2 = const_cast<StringBase<UTF8, td::EncodingUTF8, 4>&> (str);
				buffer = const_cast<char*> (str.getBuffer());
				if (addRef())
				{
					return *this;
				}
				buffer = nullptr;
			}
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF8, td::EncodingUTF8, 2, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF16, td::EncodingUTF16, 2, false>& str)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF32, td::EncodingUTF32, 2, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF8, td::EncodingUTF8, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF16, td::EncodingUTF16, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator = (const StringBase<UTF32, td::EncodingUTF32, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			setString(pTxt, inputLen);
			return*this;
		}


		inline StringBase& operator = (const StringBase<UTF8, td::EncodingUTF8, 4, true>* pStr)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			const UTF8* pTxt = pStr->c_str();
			int inputLen = pStr->unitLength();
			setString(pTxt, inputLen);
			return *this;
		}
		//IDz: Fix takeFromAlloc
		//inline StringBase& operator = (const StringBase<UTF8, td::EncodingUTF8, 4, true>* pStr)
		//{
		//	static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
		//	assert(!EXTERN_ALLOCATOR);
		//	duplicate();
		//	takeFromAlloc(pStr);
		//	return *this;
		//}

		template <bool EXTERN_ALLOCATOR_INSTR>
		inline StringBase& operator += (const StringBase<UTF8, td::EncodingUTF8, 4, EXTERN_ALLOCATOR_INSTR>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF16, td::EncodingUTF16, 4, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF16* pTxt = str.c_str();
            int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF32, td::EncodingUTF32, 4, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF8, td::EncodingUTF8, 2, false>& str)
		{
			//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF16, td::EncodingUTF16, 2, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF32, td::EncodingUTF32, 2, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF8, td::EncodingUTF8, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF8* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF16, td::EncodingUTF16, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF16* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		inline StringBase& operator += (const StringBase<UTF32, td::EncodingUTF32, 0, false>& str)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			const UTF32* pTxt = str.c_str();
			int inputLen = str.unitLength();
			appendString(pTxt, inputLen);
			return *this;
		}

		//#pragma endregion

		//#pragma region Comparison operators
		template <class TSTR>
		inline bool operator == (const TSTR& str) const
		{
            td::UINT4 nLen = length();
            td::UINT4 extLen = td::UINT4(str.length());

			if (extLen != nLen)
				return false;

			void* pMy = (void*) getBegin();
			void* pOut = (void*) str.c_str();

			if (0 == memcmp(pMy, pOut, nLen))
				return true;

			return false;
		}


		inline bool isEqual(const T_CHAR* pStr, int len) const
		{
            if (len <= 0)
                return false;
            if ((td::UINT4)len != unitLength())
				return false;
			return mem::fastCompare(pStr, c_str(), len);
		}

		template <class TSTR>
		inline bool operator != (const TSTR& str) const
		{
			int nLen = length();

			if (str.length() != nLen)
				return true;

			void* pMy = (void*) getBegin();
			void* pOut = (void*) str.c_str();

			if (0 != memcmp(pMy, pOut, nLen))
				return true;

			return false;
		}

		inline int cCompareNoCase(const char* pStr) const
		{
			//assert(pStr);
			if (pStr == 0)
				return 1;

			assert(Encoding == EncodingUTF8);
			const char* s1 = c_str();
			return td::compareNoCase(s1, pStr);
		}

		inline int cCompareSubStrNoCase(const char* pSubStr, size_t startPos) const
		{
			//assert(pStr);
			if (pSubStr == nullptr)
				return 1;			

			assert(Encoding == EncodingUTF8);
			const char* s1 = c_str();		
			if (s1 == nullptr)
				return -1;
			if (startPos >= length())
				return -1;
			return td::compareSubStrNoCase(s1 + startPos, pSubStr);
		}

		inline int cCompare(const char* pStr) const
		{
			assert(pStr);
			assert(Encoding == EncodingUTF8);
			return strcmp(c_str(), pStr);
		}

		inline bool compare(const char* pStr, int nLen) const
		{
			assert(pStr);
			assert(Encoding == EncodingUTF8);
			if (length() != nLen)
				return false;
			if (strncmp(c_str(), pStr, nLen) == 0)
				return true;
			return false;
		}

		template <size_t nLen>
		inline bool compareConstStr(const char(&str)[nLen]) const
		{
			assert(Encoding == EncodingUTF8);
			if (length() != nLen - 1)
				return false;
			if (strcmp(c_str(), str) == 0)
				return true;
			return false;
		}
        
        template <size_t nLen>
        inline bool compareConstStr(const char(&str)[nLen], size_t afterPos) const
        {
            assert(Encoding == EncodingUTF8);
            if (length() != nLen - 1 + afterPos)
                return false;
            if (strcmp(c_str() + afterPos, str) == 0)
                return true;
            return false;
        }

		inline int cCompare(const wchar_t* pStr) const
		{
			assert(pStr);

			if constexpr (_G_SIZE_OF_WCHAR_ == 2)
				assert(Encoding == EncodingUTF16);
			else if constexpr (_G_SIZE_OF_WCHAR_ == 4)
				assert(Encoding == EncodingUTF32);

			return wcscmp(c_str(), pStr);
		}

		template <class TSTR>
		inline int compare(const TSTR& str) const
		{
			int nLenMy = length();
			int nLenOut = str.length();
			int nLen;
			if (nLenMy < nLenOut)
			{
				nLen = nLenMy;

				if (nLen == 0)
					return -1;
			}
			else if (nLenMy > nLenOut)
			{
				nLen = nLenOut;
				if (nLen  == 0)
					return 1;
			}
			else
			{
				nLen = nLenMy;
				if (nLen  == 0)
					return 0;
			}

			int toRet;
			if constexpr (Encoding == EncodingUTF8)
			{
				const void* pMy = (const void*) getBegin();
				const void* pOut = (const void*) str.c_str();

				toRet = memcmp(pMy, pOut, nLen);
			}
			else if constexpr (Encoding == EncodingUTF16)
			{
				if constexpr (_G_SIZE_OF_WCHAR_ == 2)
				{
					const wchar_t* pMy = (const wchar_t*) getBegin();
					const wchar_t* pOut = (const wchar_t*) str.c_str();

					toRet = wmemcmp(pMy, pOut, nLen);
				}
				else
				{
					const WORD* pMy = (const WORD*) getBegin();
					const WORD* pOut = (const WORD*) str.c_str();

					toRet = compareStrings(pMy, pOut, nLen / 2);
				}
			}
			else if constexpr (Encoding == EncodingUTF32)
			{
				if constexpr (_G_SIZE_OF_WCHAR_ == 4)
				{
					//linux
					const wchar_t* pMy = (const wchar_t*) getBegin();
					const wchar_t* pOut = (const wchar_t*) str.c_str();

					toRet = wmemcmp(pMy, pOut, nLen);
				}
				else
				{
					const UINT4* pMy = (const UINT4*) getBegin();
					const UINT4* pOut = (const UINT4*) str.c_str();

					toRet = compareStrings(pMy, pOut, nLen / 4);
				}
			}

			if ((toRet == 0) && (nLenMy < nLenOut))
				return -1;

			if ((toRet == 0) && (nLenMy > nLenOut))
				return 1;

			return toRet;
		}

		template <class TSTR>
		inline bool operator > (const TSTR& str) const
		{
			int comp = compare(str);
			if (comp > 0)
				return true;
			return false;
		}

		template <class TSTR>
		inline bool operator < (const TSTR& str) const
		{
			int comp = compare(str);
			if (comp < 0)
				return true;
			return false;
		}

		template <class TSTR>
		inline bool operator >= (const TSTR& str) const
		{
			int comp = compare(str);
			if (comp >= 0)
				return true;
			return false;
		}

		template <class TSTR>
		inline bool operator <= (const TSTR& str) const
		{
			int comp = compare(str);
			if (comp <= 0)
				return true;
			return false;
		}

		//#pragma endregion		
		inline void left(int len, StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>& str) const
		{
			int nLen = length();

			if (len > nLen)
				len = nLen;

			if (len <= 0)
				return;
			str.clean();
			str.setString(c_str(), len);
		}

		inline td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> left(int len) const
		{
			int nLen = unitLength();

			if (len > nLen)
				len = nLen;

			if (len <= 0)
				return td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>();

			td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str;
			str.setString(c_str(), len);
			return str;
		}

		inline td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> right(int len) const
		{
			int nLen = unitLength();

			int nToSkeep = 0;
			if (len > nLen)
				len = nLen;
			else if (len < nLen)
				nToSkeep = nLen - len;

			if (len <= 0)
				return td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>();

			td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str;
			str.setString(c_str() + nToSkeep, len);
			return str;
		}


		inline bool replace(int fromPosition, const char* pStrToPlace, int lenOfStrToPlace)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if (fromPosition < 0)
				return false;
			
			int len = length();
			if (len - lenOfStrToPlace - fromPosition < 0)
				return false;

			duplicate();
			//memcpy(begin()+fromPosition, pStrToPlace, lenOfStrToPlace);
			mem::fastCopy(begin() + fromPosition, pStrToPlace, lenOfStrToPlace);
			return true;
		}		

		inline bool reserve(size_t size)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			if (length() == size)
			{
				if (getNoOfRefs() == 0)
					return true; //no need to reserve in this case
			}
				

			if (size <= 0)
				return false;
			cleanBuffer();
			if (prepareBuffer( (int) size))
			{
				char* pBeg = (char*) getBegin();
				pBeg[size] = 0;
				return true;
			}
			return false;			
		}

		inline bool reserveInUnits(size_t size)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			size *= sizeof(T_CHAR);
			if (length() == size)
			{
				if (getNoOfRefs() == 0)
					return true; //no need to reserve in this case
			}


			if (size <= 0)
				return false;
			cleanBuffer();
			if (prepareBuffer((int) size))
			{
				char* pBeg = (char*)getBegin();
				pBeg[size] = 0;
				return true;
			}
			return false;
		}

		inline const T_CHAR* c_str() const
		{
			if (buffer)			
				return (const T_CHAR*) (buffer + SPACE_FOR_SIZE);			
			//return 0;
			return (const T_CHAR*) mu::__zeroBuffer;
		}

		inline void clean()
		{			
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			cleanBuffer();
		}

		inline td::UINT4 getCount(T_CHAR ch) const
		{
			td::UINT4 nChars = unitLength();
            if (nChars == 0)
                return 0;
			td::UINT4 toRet = 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				if (pStr[i] == ch)
					++toRet;
			}
			return toRet;
		}

		inline bool hasAny(T_CHAR ch) const
		{
			td::UINT4 nChars = length();
            if (nChars == 0)
                return 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				if (pStr[i] == ch)
					return true;
			}
			return false;
		}

		int getNoOfUpper() const
		{
			td::UINT4 nChars = length();
            if (nChars == 0)
                return 0;
            
			td::UINT4 toRet = 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				if (pStr[i] >= 'A' && pStr[i] <= 'Z')
					++toRet;
			}
			return toRet;
		}
        
        int getNoOfLeading(T_CHAR ch) const
        {
            if (length() == 0)
                return 0;
            
            int nCh = 0;
            const T_CHAR* pStr = c_str();
            while (pStr)
            {
                if (*pStr == ch)
                    ++nCh;
                else
                    return nCh;
                ++pStr;
            }
            return nCh;
        }
        
		int getNoOfNonAscii() const
		{
			td::UINT4 nChars = length();
            if (nChars == 0)
                return 0;
			td::UINT4 toRet = 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				if (pStr[i] < 0 || pStr[i] > 127)
					++toRet;
			}
			return toRet;
		}

		bool hasAnyNonAscii() const
		{
			td::UINT4 nChars = length();
            if (nChars == 0)
                return 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				if (pStr[i] < 0 || pStr[i] > 127)
					return true;
			}
			return false;
		}

		inline td::UINT4 length() const
		{
			if (buffer)
			{
				if constexpr (SPACE_FOR_SIZE == 4)
				{
					td::UINT4* pSize = (td::UINT4*)buffer;
					td::UINT4 len = *pSize;

					len &= TD_STR_CLEAN_REF;

					return len;
				}
				else if constexpr (SPACE_FOR_SIZE == 2)
				{
					td::WORD* pSize = (td::WORD*)buffer;
					td::WORD size = *pSize;
					return size;
				}				
				else
				{
					if constexpr (Encoding == td::EncodingUTF8)
						return (td::UINT4)strlen(buffer);
					else
					{
						return (td::UINT4)td::strLen(buffer);
					}
				}
			}
			return 0;
		}

		inline td::UINT4 unitLength() const
		{
			if constexpr (Encoding == EncodingUTF16)
				return length() / 2;
			else if constexpr (Encoding == EncodingUTF32)
				return length() / 4;
			return length();
		}

		inline bool isEmpty() const
		{
			return (length() == 0);
		}

    protected:
        inline td::UINT4 glyphLengthLocal(td::UTF8, td::UINT4 len) const
        {
            td::UINT4 count = 0;
            td::UINT4 i = 0;
            
            const td::UTF8* pStr = c_str();
            
            while (i < len)
            {
                td::UTF8 c = pStr[i];
                
                if ((c & 0x80) == 0)
                {
                    // ASCII (1-byte)
                    i += 1;
                }
                else if ((c & 0xE0) == 0xC0)
                {
                    // 2-byte UTF-8
                    i += 2;
                }
                else if ((c & 0xF0) == 0xE0)
                {
                    // 3-byte UTF-8
                    i += 3;
                }
                else if ((c & 0xF8) == 0xF0)
                {
                    // 4-byte UTF-8
                    i += 4;
                }
                else
                {
                    // Invalid UTF-8, skip
                    i += 1;
                    assert(false);
                }
                count++;
            }
            
            return count;
        }
        
        inline td::UINT4 glyphLengthLocal(td::UTF16, td::UINT4 len) const
        {
            td::UINT4 count = 0;
            td::UINT4 i = 0;
            
            const td::UTF16* pStr = c_str();
            
            while (i < len)
            {
                td::UTF16 c = pStr[i];
                
                // Check if the current character is a high surrogate (0xD800 - 0xDBFF)
                if (c >= 0xD800 && c <= 0xDBFF) 
                {
                    // Make sure the next character exists and is a low surrogate (0xDC00 - 0xDFFF)
                    if (i + 1 < len && pStr[i + 1] >= 0xDC00 && pStr[i + 1] <= 0xDFFF) 
                    {
                        // This is a valid surrogate pair, skip the next character
                        ++i;
                    }
                }
                count++;
            }
            return count;
        }
        
        inline td::UINT4 glyphLengthLocal(td::UTF32, td::UINT4 len) const
        {
            return len;
        }
        
    public:
        
        inline td::UINT4 glyphLength() const
        {
            td::UINT4 len = unitLength();
            if (len == 0)
                return 0;
            
            T_CHAR ch=0;
            return glyphLengthLocal(ch, len);
        }

		inline StringEncoding getEncoding() const
		{
			return Encoding;
		}

		inline constexpr bool isSizeEmbeded() const
		{
			return (SPACE_FOR_SIZE == 0);
		}

        inline constexpr int spaceForSize() const
		{
			return SPACE_FOR_SIZE;			
		}

		bool constexpr isExternAllocatorUsed() const
		{
			return EXTERN_ALLOCATOR;
		}

		inline void format(const char* format, ...)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			//mu::Regionals* pRegional = mu::getMainThreadRegionals();
			//assert(pRegional->formatBuffer);
			va_list argptr;	
			{
				va_start(argptr, format);
				char buffer[1024];
				int nLen = vsnprintf(buffer, 1024, format, argptr);
				va_end(argptr);

				if (nLen > 0)
				{
					cleanBuffer();
					setString(&buffer[0], nLen);
					return;
				}
			}

			{	
				int nLen = 0;
#ifdef MU_WINDOWS
				va_start(argptr, format);
				nLen = _vscprintf(format, argptr);
				va_end(argptr);
#else
                va_start(argptr, format);
                nLen = vsnprintf(nullptr, 0, format, argptr);
                va_end(argptr);
#endif

				if ((nLen > 0) && (nLen <= 0x0000FFFF) ) //maximum string length for format is 64 kB
				{
					char* tmp = new char[nLen + 1];
					va_start(argptr, format);
					nLen = SNPRINTF(tmp, nLen, _TRUNCATE, format, argptr);
					va_end(argptr);
					if (nLen > 0)
					{
						//tmpBuff[nLen] = 0;
						setString(tmp, nLen);
						delete [] tmp;
						return;
					}
					delete [] tmp;
				}
                else if (nLen > 0)
                {
                    assert(false && "String is too long for format!");
                }
			}
			cleanBuffer();
		}

		inline T_CHAR getAt(size_t pos) const
		{			
			assert(pos < length());	
			T_CHAR* pStr = (T_CHAR*) getBegin();
			return pStr[pos];
		}
        
        inline T_CHAR at(size_t pos) const
        {
            assert(pos < unitLength());
            T_CHAR* pStr = (T_CHAR*) getBegin();
            return pStr[pos];
        }

		inline void setAt(size_t pos, T_CHAR ch)
		{
            td::UINT4 len = unitLength();
			assert(pos < len);
            
			if (pos < len)
			{
				T_CHAR* pStr = (T_CHAR*)getBegin();
				pStr[pos] = ch;
			}			
		}

		template <class TPUSHABLE>
		void split(T_CHAR ch, TPUSHABLE& cnt, bool trimSplits = true, bool resetCnt = true) const
		{
			if (resetCnt)
				cnt.reset();

			int nSplits = getCount(ch);
			if (nSplits == 0)
			{
				StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str(*this);
				if (trimSplits)
					cnt.push_back(str.trim());
				else
					cnt.push_back(str);
				return;
			}
			int iPos = -1;
			for (int i = 0; i < nSplits+1; ++i)
			{
				int iPos2 = find(ch, iPos+1);

				StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str;
				if (iPos2 > iPos + 1)
					str = subStr(iPos + 1, iPos2 - 1);
				else if (iPos2 < 0)
				{
					str = subStr(iPos + 1, -1);
				}

				if (trimSplits)
					cnt.push_back(str.trim());
				else
					cnt.push_back(str);

				if (iPos2 < 0)
					return;
				iPos = iPos2;
			}
		}
	protected:
		bool isChInSeparator(T_CHAR ch, const T_CHAR* separators) const
		{
			const T_CHAR* ptr = separators;
			while (ptr && (*ptr != 0) )
			{
				if (ch == *ptr)
					return true;
				++ptr;
			}
			return false;
		}
	public:

		inline int findOneOf(const T_CHAR* pOneOfToFind, td::UINT4 searchStartPos = 0) const
		{
            td::UINT4 nLen = unitLength();
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = searchStartPos; i < nLen; ++i)
			{
				T_CHAR myCh = pStr[i];
				//if (ch == myCh)
				if (isChInSeparator(myCh, pOneOfToFind))
					return i;
			}
			return -1;
		}

		inline td::UINT4 getChCounts(const T_CHAR* pCharsToFind) const
		{
            td::UINT4 nChars = unitLength();
			td::UINT4 toRet = 0;
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = 0; i < nChars; ++i)
			{
				//if (pStr[i] == ch)
				if (isChInSeparator(pStr[i], pCharsToFind))
					++toRet;
			}
			return toRet;
		}

		template <class TPUSHABLE>
		void split(const T_CHAR* separators, TPUSHABLE& cnt, bool trimSplits = true, bool resetCnt = true) const
		{
			if (resetCnt)
				cnt.reset();

			int nSplits = getChCounts(separators);
			if (nSplits == 0)
			{
				StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str(*this);
				if (trimSplits)
					cnt.push_back(str.trim());
				else
					cnt.push_back(str);
				return;
			}
			int iPos = -1;
			for (int i = 0; i < nSplits+1; ++i)
			{				
				int iPos2 = findOneOf(separators, iPos + 1);

				StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str;
				if (iPos2 > iPos+1)
					str = subStr(iPos+1, iPos2 - 1);
				else if (iPos2 < 0)
				{
					str = subStr(iPos + 1, -1);
				}					

				if (trimSplits)
					cnt.push_back(str.trim());
				else
					cnt.push_back(str);

				if (iPos2 < 0)
					return;
				iPos = iPos2;
			}
			//StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> str(subStr(iPos, -1));
			//if (trimSplits)
			//	cnt.push_back(str.trim());
			//else
			//	cnt.push_back(str);
		}

		inline void toUpper()
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			if (buffer == nullptr)
				return;

			if constexpr (!EXTERN_ALLOCATOR)
				duplicate();

			T_CHAR* pStr = (T_CHAR*) getBegin();

			while (*pStr)
			{
				*pStr = toUpper(*pStr);
				++pStr;
			}
		}

		inline void toSelectiveUpper(char apostrof)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

			if (buffer == nullptr)
				return;

			assert(!EXTERN_ALLOCATOR);

			assert(typeid(T_CHAR) == typeid(char));

			duplicate();

			T_CHAR* pStr = (T_CHAR*) getBegin();

			bool convert = true;
			while (*pStr)
			{
				if (*pStr == apostrof)
				{
					convert = !convert;
				}
				else
				{				
					if (convert)
						*pStr = toUpper(*pStr);
				}
				++pStr;
			}
		}

		inline void toLower()
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

			if (buffer == nullptr)
				return;

			assert(!EXTERN_ALLOCATOR);

			if constexpr (!EXTERN_ALLOCATOR)
				duplicate();

			T_CHAR* pStr = (T_CHAR*) getBegin();

			while (*pStr)
			{
				*pStr = toLower(*pStr);
				++pStr;
			}
		}
		inline void revertCase()
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

			if (buffer == nullptr)
				return;

			assert(!EXTERN_ALLOCATOR);

			if constexpr (!EXTERN_ALLOCATOR)
				duplicate();

			T_CHAR* pStr = (T_CHAR*)getBegin();

			while (*pStr)
			{
				T_CHAR ch = *pStr;				
				if (ch >= 'a' && ch <= 'z')
					*pStr = toUpper(ch);
				else if (ch >= 'A' && ch <= 'Z')
					*pStr = toLower(ch);
				
				++pStr;
			}
		}

		inline void toSelectiveLower(char apostrof)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

			if (buffer == nullptr)
				return;

			assert(!EXTERN_ALLOCATOR);

			assert(typeid(T_CHAR) == typeid(char));

			T_CHAR* pStr = (T_CHAR*) getBegin();

			bool convert = true;
			while (*pStr)
			{
				if (*pStr == apostrof)
				{
					convert = !convert;
				}
				else
				{				
					if (convert)
						*pStr = toLower(*pStr);
				}
				++pStr;
			}
		}

		template <class TSTRIN>
		inline bool beginsWith(const TSTRIN& str) const
		{			
			td::UINT4 nLen2 = str.length();
			if (length() < nLen2)
				return false;		

			const T_CHAR* pCh1 = c_str();
			const T_CHAR* pCh2 = str.c_str();
			while ((*pCh2) && (*pCh1 == *pCh2))
			{
				++pCh1;
				++pCh2;
				--nLen2;
			}
			if (0 != nLen2)
				return false;
			return true;
		}
				
		inline bool beginsWith(const T_CHAR* pStr, int fromPos = 0) const
		{
			td::UINT4  nLen2 = (td::UINT4) strlen(pStr);
			if (length() < nLen2 + fromPos)
				return false;

			const T_CHAR* pCh1 = c_str();
			pCh1 += fromPos;
			const T_CHAR* pCh2 = pStr;
			while ((*pCh2) && (*pCh1 == *pCh2))
			{
				++pCh1;
				++pCh2;
				--nLen2;
			}
			if (0 != nLen2)
				return false;
			return true;
		}

		template <class TSTRIN>
		inline bool beginsWithCI(const TSTRIN& str) const
		{
			td::UINT4  nLen2 = str.length();
			if (length() < nLen2)
				return false;

			const T_CHAR* pCh1 = c_str();
			const T_CHAR* pCh2 = str.c_str();
			while ((*pCh2) && (td::toLower(*pCh1) == td::toLower(*pCh2)))
			{
				++pCh1;
				++pCh2;
				--nLen2;
			}
			if (0 != nLen2)
				return false;
			return true;
		}
				
		inline bool beginsWithCI(const T_CHAR* pStr) const
		{
			td::UINT4  nLen2 = (td::UINT4) strlen(pStr);
			if (length() < nLen2)
				return false;

			const T_CHAR* pCh1 = c_str();
			const T_CHAR* pCh2 = pStr;
			while ((*pCh2) && (td::toLower(*pCh1) == td::toLower(*pCh2)))
			{
				++pCh1;
				++pCh2;
				--nLen2;
			}
			if (0 != nLen2)
				return false;
			return true;
		}

		inline bool endsWith(const T_CHAR* pStr, size_t len) const
		{
            size_t myLen = unitLength();
			if (len > myLen)
				return false;
			const T_CHAR* pMy = getEnd();
			pMy = pMy - len;
			if (memcmp(pStr, pMy, len * sizeof(T_CHAR)) == 0)
				return true;
			return false;
		}
        
        inline bool endsWithCI(const T_CHAR* pStr, size_t len) const
        {
            size_t myLen = unitLength();
            if (len > myLen)
                return false;
            const T_CHAR* pMy = getEnd();
            pMy = pMy - len;
            for (size_t i=0; i<len; ++i)
            {
                if (td::toUpper(pMy[i]) != td::toUpper(pStr[i]))
                    return false;
            }
            return true;
        }

		inline bool endsWith(const StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str) const
		{
            return endsWith(str.c_str(), str.length());
		}

		inline bool endsWith(const T_CHAR* pStr) const
		{
			size_t len = td::strLen(pStr);
			return endsWith(pStr, len);
		}

		template <size_t size>
		inline bool endsWith(const T_CHAR(&message)[size]) const
		{	
			return endsWith(&message[0], size);			
		}
        
        inline td::UINT4 countAppearance(T_CHAR chToCount) const
        {
            td::UINT4 nLen = unitLength();
            const T_CHAR* pStr = c_str();
            td::UINT4 nCount = 0;
            for (td::UINT4 i = 0; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if (ch == chToCount)
                    ++nCount;
            }
            return nCount;
        }

		inline td::UINT4 countAppearance(T_CHAR chRangeFrom, T_CHAR chRangeTo) const
		{
			td::UINT4 nLen = unitLength();
			const T_CHAR* pStr = c_str();
			td::UINT4 nCount = 0;
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if ((ch >= chRangeFrom) && (ch <= chRangeTo))
					++nCount;
			}
			return nCount;
		}

        inline td::UINT4 countLeading(T_CHAR chToCount) const
        {
            td::UINT4 nLen = unitLength();
            const T_CHAR* pStr = c_str();
            td::UINT4 nCount = 0;
            for (td::UINT4 i = 0; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if (ch == chToCount)
                    ++nCount;
                else
                    break;
            }
            return nCount;
        }
        
		inline int find(T_CHAR ch, td::UINT4 searchStartPos = 0) const
		{
			td::UINT4  nLen = unitLength();
			const T_CHAR* pStr = c_str();
			for (td::UINT4 i = searchStartPos; i < nLen; ++i)
			{
				T_CHAR myCh = pStr[i];
				if (ch == myCh)
					return i;
			}
			return -1;
		}
        
        
        inline int findNewLinePosition(td::UINT4 searchStartPos = 0) const
        {
            td::UINT4  nLen = unitLength();
            const T_CHAR* pStr = c_str();
            for (td::UINT4 i = searchStartPos; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if ((ch == (T_CHAR) '\r') || (ch == (T_CHAR) '\n'))
                {
                    return i;
                }
            }
            return -1;
        }
        
        inline int findNextNewLinePosition(td::UINT4 searchStartPos = 0) const
        {
            td::UINT4  nLen = unitLength();
            const T_CHAR* pStr = c_str();
            for (td::UINT4 i = searchStartPos; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if ((ch == (T_CHAR) '\r') || (ch == (T_CHAR) '\n'))
                {
                    if (ch == (T_CHAR) '\r')
                    {
                        ch = pStr[i+1]; //check if next one is new line (Windows CR/LF)
                        if (ch == (T_CHAR)'\n')
                            return i+1;
                    }
                    return i;
                }
            }
            return -1;
        }
        
        
        
        template <bool GoOverNewLines>
        inline int findNonWhiteSpace(td::UINT4 searchStartPos = 0) const
        {
            td::UINT4  nLen = unitLength();
            const T_CHAR* pStr = c_str();
            for (td::UINT4 i = searchStartPos; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if ((ch == (T_CHAR) ' ') || (ch == (T_CHAR) '\t'))
                    continue;
                if constexpr (GoOverNewLines)
                {
                    if ((ch == (T_CHAR) '\r') || (ch == (T_CHAR) '\n'))
                        continue;
                }
                return i;
            }
            return -1;
        }
        
        template <bool ConsiderNewLineAsWhiteSpace>
        inline int findWhiteSpace(td::UINT4 searchStartPos = 0) const
        {
            td::UINT4  nLen = unitLength();
            const T_CHAR* pStr = c_str();
            for (td::UINT4 i = searchStartPos; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if ((ch == (T_CHAR) ' ') || (ch == (T_CHAR) '\t'))
                    return i;
                if constexpr (ConsiderNewLineAsWhiteSpace)
                {
                    if ((ch == (T_CHAR) '\r') || (ch == (T_CHAR) '\n'))
                        return i;
                }
            }
            return -1;
        }
        
        inline int findNonLetter(td::UINT4 searchStartPos = 0) const
        {
            td::UINT4  nLen = unitLength();
            const T_CHAR* pStr = c_str();
            for (td::UINT4 i = searchStartPos; i < nLen; ++i)
            {
                T_CHAR ch = pStr[i];
                if ((ch >= (T_CHAR) 'A') && (ch <= (T_CHAR) 'Z'))
                    continue;
                if ((ch >= (T_CHAR) 'a') && (ch <= (T_CHAR) 'z'))
                    continue;
                return i;
            }
            return -1;
        }


		//ODAVDE - MODELEXP Corrections
		inline int findFromRight(T_CHAR ch, int searchStartPos = -1) const
		{
			int  nLen = (int) unitLength();

			if (nLen == 0)
				return -1;
			
			if (searchStartPos < 0)
				searchStartPos = nLen - 1;

			if (searchStartPos >= nLen)
				searchStartPos = nLen - 1;
			
			const T_CHAR* pStr = c_str();
			for (int i = searchStartPos; i >= 0; --i)
			{
				T_CHAR myCh = pStr[i];
				if (ch == myCh)
					return i;				
			}
			return -1;
		}
        
        inline const T_CHAR* getExtension() const
        {
            int dotPos = findFromRight('.');
            if (dotPos > 0)
            {
                const T_CHAR* pExt = c_str() + dotPos;
                return pExt;
            }
            return nullptr;
        }
        
        inline bool hasExtension(const T_CHAR* pExtWithDot) const
        {
            const T_CHAR* pExt = getExtension();
            if (!pExt)
                return false;
            if (std::strcmp(pExt, pExtWithDot) == 0)
                return true;
            return false;
        }
        
        inline int findWhiteSpaceFromRight(int searchStartPos = -1) const
        {
            int  nLen = (int) unitLength();

            if (nLen == 0)
                return -1;
            
            if (searchStartPos < 0)
                searchStartPos = nLen - 1;

            if (searchStartPos >= nLen)
                searchStartPos = nLen - 1;
            
            const T_CHAR* pStr = c_str();
            for (int i = searchStartPos; i >= 0; --i)
            {
                T_CHAR myCh = pStr[i];
                if (isWhiteSpace(myCh))
                    return i;
            }
            return -1;
        }
        
        inline int findNonWhiteSpaceFromRight(int searchStartPos = -1) const
        {
            int  nLen = (int) unitLength();

            if (nLen == 0)
                return -1;
            
            if (searchStartPos < 0)
                searchStartPos = nLen - 1;

            if (searchStartPos >= nLen)
                searchStartPos = nLen - 1;
            
            const T_CHAR* pStr = c_str();
            for (int i = searchStartPos; i >= 0; --i)
            {
                T_CHAR myCh = pStr[i];
                if (!isWhiteSpace(myCh))
                    return i;
            }
            return -1;
        }


		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> replaceSystemVariables(const T_CHAR sysChMarker = '%') const
		{
			int startPos = find(sysChMarker);
			if (startPos < 0)
				return *this;

			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> strToRet;

			if (startPos > 0)
			{
				strToRet = subStr(0, startPos);
			}

			int n = unitLength();
			int prev = 0;
			for (int i = startPos+1; i < n; ++i)
			{
				T_CHAR ch = getAt(i);
				if (td::isAlpha(ch) || td::isNumeric(ch))
					continue;

				td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> sysVariable = subStr(startPos+1, i-1);
				char *pEnvVar = nullptr;
				size_t len = 0;
				auto err = DUPENV(&pEnvVar, &len, sysVariable.c_str());

				//char* pEnvVar = ::getenv();
				if (!err && pEnvVar)
					strToRet += pEnvVar;

				if (pEnvVar)
				{
					free(pEnvVar);
					pEnvVar = nullptr;
				}
					

				if ((ch == '%') || (ch == ':'))
					startPos = i + 1;
				else
					startPos = i;
				
				prev = startPos;

				startPos = find(sysChMarker, startPos);
				if (startPos < 0)
				{
					strToRet += subStr(prev, -1);
					return strToRet;
				}
				else
				{
					strToRet += subStr(prev, startPos-1);
				}
				i = startPos;
			}

			if (prev != startPos)
			{
				td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> sysVariable = subStr(startPos + 1, -1);
				//char* pEnvVar = ::getenv(sysVariable.c_str());
				char *pEnvVar = nullptr;
				size_t len = 0;
				auto err = DUPENV(&pEnvVar, &len, sysVariable.c_str());
				if (!err && pEnvVar)
					strToRet += pEnvVar;
				if (pEnvVar)
				{
					free(pEnvVar);
					pEnvVar = nullptr;
				}
			}
			return strToRet;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> subStr(int startPos, int endPos) const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> strRet;
			int nLen = unitLength();
			if (nLen == 0)
				return strRet;
			
			if (startPos < 0)
				startPos = 0;
			if (endPos < 0)
				endPos = nLen - 1;
			if (endPos >= nLen)
				endPos = nLen - 1;			
			if (startPos >= nLen)
				return strRet;

			int toCopy = endPos - startPos;
			if (toCopy < 0)
				return strRet;
			const T_CHAR* pBeg = c_str();
			pBeg += startPos;
			strRet.fromKnownString(pBeg, toCopy+1);
			return strRet;
		}

		//DOVDE MODELEXP Corrections

	

		//returns first position from searchStartPos where pPattern appears
		inline int find(const T_CHAR* pPattern, td::UINT4  searchStartPos = 0) const
		{
			td::UINT4 lengthOfPattern = (td::UINT4) td::strLen(pPattern);
            if (lengthOfPattern == 0)
                return -1;

			td::UINT4 nLen = unitLength();
			td::UINT4 iMatch = 0;

			td::UINT4 nFound = searchStartPos;
			for (td::UINT4 i = searchStartPos; i<nLen; ++i)
			{
				T_CHAR ch = getAt(i);
				if (ch == pPattern[iMatch])
				{
					if (iMatch == 0)
						nFound = i;

					++iMatch;

					if (iMatch == lengthOfPattern)
					{
						return nFound;
					}
				}
				else
				{
					iMatch = 0;					
				}
			}

			return -1;
		}

		//returns first position from searchStartPos where pPattern appears
		inline int findCI(const T_CHAR* pPattern, td::UINT4  searchStartPos = 0) const
		{
			StringBase strTmp(pPattern);
			int lengthOfPattern = strTmp.length();

			td::UINT4 nLen = unitLength();
			td::UINT4 iMatch = 0;

			td::UINT4 nFound = searchStartPos;
			for (td::UINT4 i = searchStartPos; i < nLen; ++i)
			{
				T_CHAR ch = td::toLower(getAt(i));
				if (ch == td::toLower(pPattern[iMatch]))
				{
					if (iMatch == 0)
						nFound = i;

					++iMatch;

					if (iMatch == lengthOfPattern)
					{
						return nFound;
					}
				}
				else
				{
					iMatch = 0;
				}
			}

			return -1;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> replace(const T_CHAR* pPattern, const T_CHAR* pToPlace) const
		{			
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> strTmp(pPattern);
			int lengthOfPattern = strTmp.unitLength();
			
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> strToPlace(pToPlace);
			int nToPlace =strToPlace.unitLength();

			BufferString<T_CHAR, 512, 512> buff;
			int nLen = unitLength();
			int iMatch = 0;
			for (int i=0; i<nLen; ++i)
			{
				T_CHAR ch = getAt(i);
				if (ch == pPattern[iMatch])
				{
					++iMatch;

					if (iMatch == lengthOfPattern)
					{
						if (pToPlace)
							buff.append(nToPlace, pToPlace);

						iMatch = 0;
					}
				}
				else
				{					
					if (iMatch > 0)
					{
						buff.append(iMatch, pPattern);
						iMatch = 0;
					}

					buff += ch;
				}
			}

			if (iMatch > 0)
			{
				buff.append(iMatch, pPattern);				
			}

			//StringBase strToRet(buff.c_str());

			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> strRet;
			strRet.fromKnownString(buff.c_str(), buff.length());
			return strRet;
		}
        
        inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> replace(const T_CHAR* pPattern, const StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>& strToPlace) const
        {
            return replace(pPattern, strToPlace.c_str());
        }

		inline bool replace(T_CHAR toSearch, T_CHAR toPlace) 
		{	
			int iStartPos = find(toSearch);
			if (iStartPos >= 0)
				duplicate();
			else
				return false;

			td::UINT4 nLen = unitLength();
			T_CHAR* pStr = (T_CHAR*)getBegin();

			//bool bReplaced = false;
			for (td::UINT4 i = (td::UINT4) iStartPos; i<nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch == toSearch)
				{
					pStr[i] = toPlace;
					///bReplaced = true;
				}				
			}
			return true;
		}

		inline bool replace(td::UINT4 startingPos, T_CHAR toSearch, T_CHAR toPlace)
		{
			int iStartPos = find(toSearch, startingPos);
			if (iStartPos >= 0)
				duplicate();
			else
				return false;

			td::UINT4 nLen = unitLength();
			//if (startingPos >= nLen)
				//return false;
			T_CHAR* pStr = (T_CHAR*)getBegin();
			pStr += iStartPos;
			nLen -= iStartPos;

			//bool replaced = false;
			for (td::UINT4 i =0; i<nLen; ++i)
			{
				T_CHAR ch = pStr[i];
				if (ch == toSearch)
				{
					pStr[i] = toPlace;					
				}
			}
			return true;
		}

		inline void prepareTrimChars(const T_CHAR* toTrim, std::set<T_CHAR>& trimChars) const
		{
			while (*toTrim)
			{
				trimChars.insert(*toTrim);
				++toTrim;
			}
		}				

		inline void removeWhiteSpaces(const T_CHAR* toTrim, T_CHAR apostrof)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			//duplicate uradjen dole
			std::set<T_CHAR> trimChars;
			prepareTrimChars(toTrim, trimChars);
			removeWhiteSpaces<false, false, false>(trimChars, apostrof, ' ');
		}


		template <bool CONV_TO_UPPER, bool ELIMANTE_C_COMMENTS, bool PUT_SINGLE_WHITESPACE_CHAR = true>
		inline bool removeWhiteSpaces(const std::set<T_CHAR> & setToSkip, T_CHAR apostrof, T_CHAR whiteSpace, bool bStatusCComment = false)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");

			assert(!EXTERN_ALLOCATOR);

			int nLen = unitLength();
			if (nLen <= 0)
			{
				return bStatusCComment;
			}

			duplicate();

			T_CHAR commentCh1 = 47; // '/'
			T_CHAR commentCh2 = 42; // '*'                                        

			typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip from the beginning
			T_CHAR* pOut = (T_CHAR*) getBegin();
			const T_CHAR* pIn = pOut;

			bool putWhiteSpace = false;
			bool replaceSpaces = true;

			int i = 0;
			int j = 0;

			T_CHAR prevCh = 0;

			T_CHAR chIn = pIn[i];
			while (chIn)
			{
				//proces C comments
				if (ELIMANTE_C_COMMENTS)
				{
					if (bStatusCComment)
					{
						if ( (chIn == commentCh2) && ( pIn[i+1] == commentCh1) )
						{
							bStatusCComment = false;
							++i;
						}
						chIn = pIn[++i];
						continue;
					}
					else
					{
						if ( (chIn == commentCh1) && ( pIn[i+1] == commentCh2) )
						{
							bStatusCComment = true;
							++i;
							chIn = pIn[++i];
							continue;
						}
					}
				}

				//check string separator
				if  (chIn == apostrof)
					replaceSpaces = !replaceSpaces;

				//do the job
				if (replaceSpaces)
				{
					typename std::set<T_CHAR>::const_iterator it = setToSkip.find(chIn);
					if (it != itEnd)
					{
						chIn = pIn[++i];

						if (j > 0)
							putWhiteSpace = true;

						continue;
					}
					else
					{
						if (PUT_SINGLE_WHITESPACE_CHAR && putWhiteSpace)
						{
							putWhiteSpace = false;
							pOut[j] = whiteSpace;
							++j;
						}
						else
							putWhiteSpace = false;

						if (CONV_TO_UPPER)
							pOut[j] = toUpper(chIn);
						else
							pOut[j] = chIn;
						++j;
					}
				}
				else
				{
					if (PUT_SINGLE_WHITESPACE_CHAR && putWhiteSpace)
					{
						pOut[j] = whiteSpace;
						++j;
						putWhiteSpace = false;
					}
					else
						putWhiteSpace = false;

					pOut[j] = chIn;
					++j;
				}
				chIn = pIn[++i];
			}

			if (j == 0)
			{
				cleanBuffer();
			}
			else
			{
				pOut[j] = 0;
				resetSize(j);
			}
			return bStatusCComment;
		}
		
		td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false> fromISO8859_1ToUTF8() const
		{
			auto nLen = unitLength();
			if (nLen == 0)
				return td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false>();

			auto nSpec = getNoOfNonAscii();
			if (nSpec == 0)
				return *this;
			//unsigned char* pIn = (unsigned char*) begin();
			//td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false> strOut;
			//
			//strOut.reserve(nLen + nSpec);
			//unsigned char* pOut = (unsigned char*) strOut.begin();
			//			
			//while (*pIn)
			//{
			//	if (*pIn < 128)
			//		*pOut++ = *pIn++;
			//	else
			//	{
			//		*pOut++ = 0xc2 + (*pIn > 0xbf);
			//		*pOut++ = (*pIn++ & 0x3f) + 0x80;
			//	}
			//}
			//pOut[nLen + nSpec - 1] = 0;
			//		
			//return strOut;

			//auto nLen = length();

			td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false> strOut;
			strOut.reserve(nLen + nSpec);
			mu::UTF8* pOut = (mu::UTF8*)strOut.begin();
			size_t iOut = 0;
			for (td::UINT4 i = 0; i < nLen; ++i)
			{
                mu::UTF8 ch = (mu::UTF8) getAt(i);
				if (ch > 0 && ch < 0x80)
				{
					//strOut.push_back(ch);
					pOut[iOut++] = ch;
				}
				else
				{
					//strOut.push_back(0xc0 | ch >> 6);
					//strOut.push_back(0x80 | (ch & 0x3f));
					pOut[iOut++] = (0xc0 | ch >> 6);
					pOut[iOut++] = (0x80 | (ch & 0x3f));
				}
			}
			pOut[nLen + nSpec - 1] = 0;
			return strOut;
		}

		template <class MAP, class STRBUILD>
		td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false> convertCPUsingWordMap(const MAP& map, STRBUILD& build)
		{
			auto nLen = unitLength();
			if (nLen == 0)
				return td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false>();

			auto nSpec = getNoOfNonAscii();
			if (nSpec == 0)
				return *this;
			build.reset();

			
			unsigned char* pIn = (unsigned char*) begin();
				
			auto itE = map.cend();

			for (td::UINT4 i = 0; i < nLen; ++i)
			{
				unsigned char ch = pIn[i];

				if (ch > 127)
				{
					auto it = map.find(ch);
					if (it != itE)
					{
						build << it->second;
					}
					else
					{
						//nisu svi karakteri u mapi
						assert(false);
					}
				}
				else
				{
					build << (char)ch;
				}
			}
			
			td::StringBase<td::UTF8, Encoding, SPACE_FOR_SIZE, false> strOut;
			build.getString(strOut);
			return strOut;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trim(const T_CHAR* toTrim) const
		{			
			std::set<T_CHAR> trimChars;
			prepareTrimChars(toTrim, trimChars);
			return trim(trimChars);
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimLeft(const T_CHAR* toTrim) const
		{
			std::set<T_CHAR> trimChars;
			prepareTrimChars(toTrim, trimChars);
			return trimLeft(trimChars);
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimRight(const T_CHAR* toTrim) const
		{			
			std::set<T_CHAR> trimChars;
			prepareTrimChars(toTrim, trimChars);
			return trimRight(trimChars);
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trim(const std::set<T_CHAR>& setToSkip) const
		{			
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip from the beginning
			const T_CHAR* pCh = c_str();
			int i = 0;
			while (pCh[i])
			{
				typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[i]);
				if (it != itEnd)
					++i;
				else
					break;
			}

			//check how much to skip at the end
			int j = 0;
			int pos = nLen - 1;
			while (pos >= 0)
			{
				typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[pos]);
				if (it != itEnd)
				{
					--pos;
					++j;
				}
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - i - j;
			if (!outString.prepareBuffer(nNewLen))
				return outString;

			if (nNewLen > 0)
			{
				memcpy(outString.getBegin(), pCh + i, nNewLen * sizeof(T_CHAR));
			}

			return outString;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimLeft(const std::set<T_CHAR>& setToSkip) const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip from the beginning
			const T_CHAR* pCh = c_str();
			int i = 0;
			while (pCh[i])
			{
				typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[i]);
				if (it != itEnd)
					++i;
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - i;
			if (!outString.prepareBuffer(nNewLen))
				return outString;

			if (nNewLen > 0)
			{
				memcpy(outString.getBegin(), pCh + i, nNewLen * sizeof(T_CHAR));
			}
			return outString;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimRight(const std::set<T_CHAR>& setToSkip) const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip at the end
			int j = 0;
			int pos = nLen - 1;
			const T_CHAR* pCh = c_str();
			while (pos >= 0)
			{
				typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[pos]);
				if (it != itEnd)
				{
					--pos;
					++j;
				}
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - j;
			if (!outString.prepareBuffer(nNewLen))
				return outString;

			if (nNewLen > 0)
			{
				memcpy(outString.getBegin(), pCh, nNewLen * sizeof(T_CHAR));
			}
			return outString;
		}

		//all white space

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trim() const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			//typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip from the beginning
			const T_CHAR* pCh = c_str();
			int i = 0;
			while (pCh[i])
			{
				//typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[i]);

				T_CHAR ch = pCh[i];
				if (td::isWhiteSpace(ch))
					++i;
				else
					break;
			}

			//check how much to skip at the end
			int j = 0;
			int pos = nLen - 1;
			while (pos >= 0)
			{
				//typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[pos]);
				//if (it != itEnd)
				T_CHAR ch = pCh[pos];
				if (td::isWhiteSpace(ch))
				{
					--pos;
					++j;
				}
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - i - j;					

			if (nNewLen > 0)
			{
				if (!outString.prepareBuffer(nNewLen))
					return outString;
				memcpy(outString.getBegin(), pCh + i, nNewLen * sizeof(T_CHAR));
			}
			//else
			//{
			//	assert(nNewLen >= 0);
			//	nNewLen++;
			//}

			return outString;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimLeft() const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			//typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip from the beginning
			const T_CHAR* pCh = c_str();
			int i = 0;
			while (pCh[i])
			{
				//typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[i]);
				if (std::isspace(pCh[i]))
					++i;
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - i;
			if (!outString.prepareBuffer(nNewLen))
				return outString;

			if (nNewLen > 0)
			{
				memcpy(outString.getBegin(), pCh + i, nNewLen * sizeof(T_CHAR));
			}
			return outString;
		}

		inline StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> trimRight() const
		{
			StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false> outString;
			int nLen = unitLength();
			if (nLen <= 0)
			{
				//outString.cleanBuffer();
				return outString;
			}

			//typename std::set<T_CHAR>::const_iterator itEnd = setToSkip.end();

			//check how much to skip at the end
			int j = 0;
			int pos = nLen - 1;
			const T_CHAR* pCh = c_str();
			while (pos >= 0)
			{
				//typename std::set<T_CHAR>::const_iterator it = setToSkip.find(pCh[pos]);
				if (std::isspace(pCh[pos]))
				{
					--pos;
					++j;
				}
				else
					break;
			}

			//copy the rest in outString
			int nNewLen = nLen - j;
			if (!outString.prepareBuffer(nNewLen))
				return outString;

			if (nNewLen > 0)
			{
				memcpy(outString.getBegin(), pCh, nNewLen * sizeof(T_CHAR));
			}
			return outString;
		}
        
        int getFirstNonWhiteSpacePosition(int startingPos = 0) const
        {
            auto len = unitLength();

            if (startingPos >= unitLength())
                return -1;
            
            for (td::UINT4 pos = startingPos; pos<len; ++pos)
            {
                auto ch = getAt(pos);
                if (!td::isWhiteSpace(ch))
                    return pos;
            }
            return -1;
        }
        
        int getFirstWhiteSpacePosition(int startingPos = 0) const
        {
            auto len = unitLength();

            if (startingPos >= unitLength())
                return -1;
            
            for (td::UINT4 pos = startingPos; pos<len; ++pos)
            {
                auto ch = getAt(pos);
                if (td::isWhiteSpace(ch))
                    return pos;
            }
            return -1;
        }

		template <class ISTREAM>
		inline void fromStream(ISTREAM& is, int nLen)
		{		
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			nLen *= sizeof(T_CHAR);

			cleanBuffer();
			if (prepareBuffer(nLen))
			{
				is.read(getBegin(), nLen);
			}
		}

		inline StringBase& operator = (const std::wstring& txt)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int inputLen = (int) wcslen(txt.c_str());
			setString(txt.c_str(), inputLen);
			return *this;
		}

		inline StringBase& operator = (const std::string& txt)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int inputLen = (int) txt.length();
			setString(txt.c_str(), inputLen);
			return *this;
		}

		inline StringBase& operator = (const QString& qStr);
		//{
		//	static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
		//	assert(!EXTERN_ALLOCATOR);			
		//	cleanBuffer();
		//	QByteArray byteArray = qStr.toUtf8();
		//	const char* pStr = byteArray.data(); //(const char*)qStr.toUtf8();
		//	int inputLen = (int) strLen(pStr);
		//	setString(pStr, inputLen);
		//	return *this;
		//}


		inline StringBase& operator = (const td::Variant& var);
		//{
		//	static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
		//	assert(!EXTERN_ALLOCATOR);
		//	td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str = *this;
		//	var.template getValue<td::StringBase< T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR> > (str);
		//	return *this;
		//}

		inline StringBase& operator += (const std::wstring& txt)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			int inputLen = (int) wcslen(txt.c_str());
			appendString(txt.c_str(), inputLen);
			return *this;
		}

		inline StringBase& operator += (const std::string& txt)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			duplicate();
			int inputLen = (int) txt.length();
			appendString(txt.c_str(), inputLen);
			return *this;
		}

		//#pragma region DateTime Formats


		inline void format(const td::Date& d, td::Date::Format df = td::Date::Format::ShortY4)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			format(mu::getMainThreadRegionals(), d, df);
		}

		inline void format(mu::Regionals* pRegionals, const td::Date& d, td::Date::Format df = td::Date::Format::ShortY4)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen =pRegionals->format(df, d);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegionals->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		

		inline void format(mu::Regionals* pRegionals, const td::Time& t, td::Time::Format tf = td::Time::Format::ShortHMMSSu)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen =pRegionals->format(tf, t);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegionals->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		inline void format(const td::Time& t, td::Time::Format tf = td::Time::Format::ShortHMMSSu)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			format(mu::getMainThreadRegionals(), t, tf);
		}

		inline void format(mu::Regionals* pRegionals, const td::DateTime& dt, td::Date::Format df = td::Date::Format::ShortY4, td::Time::Format tf = td::Time::Format::ShortHMMSSu)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen =pRegionals->format(df, tf, dt);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegionals->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		inline void format(const td::DateTime& dt, td::Date::Format df = td::Date::Format::ShortY4, td::Time::Format tf = td::Time::Format::ShortHMMSSu)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			format(mu::getMainThreadRegionals(), dt, df, tf);
		}
		//#pragma endregion

		//#pragma region Number Formats
		template <typename TFLOAT>
		inline void formatFloat(mu::Regionals* pRegional, TFLOAT val, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen = pRegional->formatFloat(val, nDec, setThSeparator);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
				//cleanBuffer();
		}

		template <typename TFLOAT>
		inline void formatFloat(TFLOAT val, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatFloat(mu::getMainThreadRegionals(), val, nDec, setThSeparator);
		}
		
		inline void format(float val, int nDec, bool setThSeparator = true)
		{
			formatFloat<float>(mu::getMainThreadRegionals(), val, nDec, setThSeparator);
		}

		inline void format(double val, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatFloat<double>(mu::getMainThreadRegionals(), val, nDec, setThSeparator);
		}

		template <typename TINT>
		inline void formatInt(mu::Regionals* pRegional, TINT val, const wchar_t* pFormat, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen = pRegional->formatInt(val, pFormat, setThSeparator);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
				//cleanBuffer();
		}

		template <typename TINT>
		inline void formatInt(TINT val, const wchar_t* pFormat, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatInt(mu::getMainThreadRegionals(), val, pFormat, setThSeparator);
		}

		template <typename TINT>
		inline void formatInt(mu::Regionals* pRegional, TINT val, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int nLen = pRegional->formatInt(val, setThSeparator);
			assert(nLen >= 0);
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TINT>
		inline void formatInt(TINT val, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatInt(mu::getMainThreadRegionals(), val, setThSeparator);
		}
		
		inline void format(td::INT4 val, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatInt<td::INT4>(mu::getMainThreadRegionals(), val, setThSeparator);
		}

		inline void format(td::INT2 val, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatInt<td::INT2>(mu::getMainThreadRegionals(), val, setThSeparator);
		}

		inline void format(td::LINT8 val, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			formatInt<td::LINT8>(mu::getMainThreadRegionals(), val, setThSeparator);
		}


		template <typename TFloat>
		inline void uniformFltBuild5(mu::Regionals* pRegional, TFloat val1, TFloat val2, TFloat val3, TFloat val4, TFloat val5, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int nLen = pRegional->buildFloat5<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, val5, nDec, setThSeparator);
			assert(nLen >= 0);
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TFloat>
		inline void uniformFltBuild5(TFloat val1, TFloat val2, TFloat val3, TFloat val4, TFloat val5, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			uniformFltBuild5(mu::getMainThreadRegionals(), val1, val2, val3, val4, val5, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline void uniformFltBuild4(mu::Regionals* pRegional, TFloat val1, TFloat val2, TFloat val3, TFloat val4, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int nLen = pRegional->buildFloat4<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, setThSeparator);
			assert(nLen >= 0);
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TFloat>
		inline void uniformFltBuild4(TFloat val1, TFloat val2, TFloat val3, TFloat val4, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			uniformFltBuild4(mu::getMainThreadRegionals(), val1, val2, val3, val4, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline void uniformFltBuild3(mu::Regionals* pRegional, TFloat val1, TFloat val2, TFloat val3, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			int nLen = pRegional->buildFloat3<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, setThSeparator);
			assert(nLen >= 0);
			cleanBuffer();
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TFloat>
		inline void uniformFltBuild3(TFloat val1, TFloat val2, TFloat val3, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			uniformFltBuild3(mu::getMainThreadRegionals(), val1, val2, val3, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline void uniformFltBuild2(mu::Regionals* pRegional, TFloat val1, TFloat val2, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int nLen = pRegional->buildFloat2<TFloat, '\n'>(val1, nDec, val2, nDec, setThSeparator);
			assert(nLen >= 0);
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TFloat>
		inline void uniformFltBuild2(TFloat val1, TFloat val2, int nDec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			uniformFltBuild2(mu::getMainThreadRegionals(), val1, val2, nDec, setThSeparator);
		}

		template <typename TDEC, int NLEN>
		inline void format(mu::Regionals* pRegional, const td::Decimal<TDEC, NLEN>& dec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			cleanBuffer();
			int nLen = pRegional->format(dec, setThSeparator);
			assert(nLen >= 0);
			if (nLen > 0)
				setString(pRegional->formatBuffer, nLen);
			//else
			//	cleanBuffer();
		}

		template <typename TDEC, int NLEN>
		inline void format(const td::Decimal<TDEC, NLEN>& dec, bool setThSeparator = true)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			format(mu::getMainThreadRegionals(), dec, setThSeparator);
		}

                td::UINT4 calcHashNo() const; //include td/StringConverter.h
//		{
//			return mu::Utils::calcHashNo(c_str());
//		}

		template <typename THASHVAL>
		THASHVAL calcHashValue() const
		{
			THASHVAL hashNo = 0;
			const T_CHAR* pStr = c_str();

			while (*pStr)
			{
				hashNo ^= (hashNo << 5) + (hashNo >> 2) + *pStr;
				++pStr;
			}
			return hashNo;
		}
        
        template <size_t NCHBUFF>
        size_t toAnsiCNumber(char (&buff)[NCHBUFF], char decPoint, char thSep)
        {
            static_assert(typeid(T_CHAR) == typeid(td::UTF8), "This method works with UTF8 only!");
            const char* pStr = c_str();
            size_t nLen = length();
            return toAnsiCNumber(pStr, nLen, buff, decPoint, thSep);
        }
        
        template <size_t NCHBUFF>
        std::tuple<size_t, bool> extractNumbers(td::DataType /*dt*/, char (&buff)[NCHBUFF], char decPoint, char thSep, bool sci= false)
        {
            static_assert(typeid(T_CHAR) == typeid(td::UTF8), "This method works with UTF8 only!");
            const char* pStr = c_str();
            size_t nLen = length();
            return td::extractNumbers(pStr, nLen, buff, decPoint, thSep, sci);
        }

                
        size_t hash_value() const; //include StringConverter.h

	protected:		
		//input from istream
		template<class CharT, class Traits>
		friend std::basic_istream<CharT, Traits>&
			operator>>(std::basic_istream<CharT, Traits>& is,  StringBase& rhs)
		{		
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
			assert(!EXTERN_ALLOCATOR);
			std::string str;
			is >> str;
			rhs = str;
			return is;
		}

		//output to ostream
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os,  const StringBase& rhs)
		{
            if (rhs.length() == 0)
                return os;
			os << rhs.c_str();
			return os;
		}
	};

    typedef StringBase<UTF8, td::EncodingUTF8, 0, false> ZStringUTF8;
	typedef StringBase<UTF16, td::EncodingUTF16, 0, false> ZStringUTF16;
	typedef StringBase<UTF32, td::EncodingUTF32, 0, false> ZStringUTF32;

	typedef StringBase<UTF8, td::EncodingUTF8, 2, false> MidStringUTF8;
	typedef StringBase<UTF16, td::EncodingUTF16, 2, false> MidStringUTF16;
	typedef StringBase<UTF32, td::EncodingUTF32, 2, false> MidStringUTF32;

	//typedef StringBase<UTF8, td::EncodingUTF8, 4, false> StringUTF8;
    typedef StringBase<UTF16, td::EncodingUTF16, 4, false> StringUTF16;
	typedef StringBase<UTF32, td::EncodingUTF32, 4, false> StringUTF32;
	
    typedef StringBase<UTF8, td::EncodingUTF8, 0, false> ZString;
    typedef StringBase<UTF8, td::EncodingUTF8, 4, false> String;
	typedef StringBase<UTF8, td::EncodingUTF8, 4, true> StringExt;

	inline DataType getType(const String&) {return string8;}
	inline DataType getType(const StringUTF16&) {return string16;}

	inline DataType getType(const StringExt&) { return string8; }
		
	//#pragma endregion
	template <typename T>
	inline td::String toHex(T& val)
	{
		size_t inpLen = sizeof(T);
		td::String strOut;
		strOut.reserve(inpLen * 2);
		char* outStr = strOut.begin();
		td::toHex(val, outStr);
		return strOut;
	}

} //namespace td

namespace mu
{
extern MAINUTILS_API const td::String __emptyString;
MAINUTILS_API td::String getEnvironmentVariable(const char* variableName);

MAINUTILS_API td::String getHomeFolderName();
}

namespace std
{
	template <typename T_CHAR, td::StringEncoding Encoding, int SIZEINSIDE, bool EXTERN_ALLOCATOR>
	struct hash<td::StringBase<T_CHAR, Encoding, SIZEINSIDE, EXTERN_ALLOCATOR>> //: public unary_function<td::StringBase<T_CHAR, Encoding, SIZEINSIDE, EXTERN_ALLOCATOR>, size_t>
	{       
		size_t operator()(const td::StringBase<T_CHAR, Encoding, SIZEINSIDE, EXTERN_ALLOCATOR>& v) const
		{           
			return (size_t) v.calcHashNo(); // my hash algorithm
		}   
	};
}
