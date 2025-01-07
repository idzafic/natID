// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <td/BinaryString.h>
#include <cnt/List.h>
#include <mu/Utils.h>

namespace no
{
	namespace sock
	{
		class Communication;
	}
}

namespace cnt
{
	class reset_StringBuilder {};
	constexpr reset_StringBuilder clear;

	class newline_StringBuilder {};
	constexpr newline_StringBuilder endl;

	
	template <class TUSEDSTRING = td::String, td::UINT4 SINGLEBUFFLEN = 1024 * 64>
	class StringBuilder : public cnt::List<typename TUSEDSTRING::CHAR_TYPE[SINGLEBUFFLEN], false>
	{
		friend class no::sock::Communication;
	protected:
		typedef typename TUSEDSTRING::CHAR_TYPE CH;
		typedef cnt::List<CH[SINGLEBUFFLEN], false> BASE;
		typedef typename BASE::iterator BASE_ITER;
		typedef typename BASE::const_iterator CONST_BASE_ITER;
		td::UINT4 _nChInCurrent;
		td::UINT4 _nTotalCh;
		td::UINT4 _nTotalChReserved;
		BASE_ITER _currentChunk;
		//CH* _currentBuffer;
		mu::Utils _conv;
		//cnt::ListSL<CH[BUFFLEN]> _data;	

		inline CH* getDataPtr(BASE_ITER& it) const
		{
			return (CH*)(&((*it)[0]));
		}

		inline const CH* getDataPtr(CONST_BASE_ITER& it) const
		{
			return (const CH*)(&((*it)[0]));
		}

		inline td::UINT4 getLastLen() const
		{
			return _nChInCurrent;
		}

		inline td::UINT4 getBuffLen() const
		{
			return SINGLEBUFFLEN;
		}

		inline void putStartingFromNewChunk(const CH* pStr, td::UINT4 len)
		{
			td::UINT4 nSteps = len / SINGLEBUFFLEN;
			//first put full blocks
			for (td::UINT4 i = 0; i < nSteps; ++i)
			{
				++_currentChunk;				 
				if (_currentChunk == BASE::end())
				{
					_currentChunk = BASE::push_back();
					_nTotalChReserved += SINGLEBUFFLEN;					
				}
				
				CH* buff = getDataPtr(_currentChunk); // (CH*)_currentChunk.data;

				mem::fastCopy(buff, pStr, SINGLEBUFFLEN);
				pStr += SINGLEBUFFLEN;
			}

			//put remaining
			_nChInCurrent = len % SINGLEBUFFLEN;

			if (_nChInCurrent == 0 && nSteps != 0)
			{
				_nChInCurrent = SINGLEBUFFLEN;
				return;
			}

			++_currentChunk;
			
			if (_currentChunk == BASE::end())
			{
				_currentChunk = BASE::push_back();
				_nTotalChReserved += SINGLEBUFFLEN;				
			}
			
			CH* buff = getDataPtr(_currentChunk); //buff = (CH*)_currentChunk.data;

            mem::fastCopy(buff, pStr, _nChInCurrent);

		}

		inline void putString(const CH* pStr, td::UINT4 nLen)
		{
			if (nLen <= SINGLEBUFFLEN - _nChInCurrent)
			{
				//CH* currentBuffer = (CH*) _currentChunk.data;
				CH* buff = getDataPtr(_currentChunk);
                mem::fastCopy(buff + _nChInCurrent, pStr, nLen);
				_nChInCurrent += nLen;
			}
			else if (_nChInCurrent < SINGLEBUFFLEN)
			{
				//CH* currentBuffer = (CH*)_currentChunk.data;
				CH* buff = getDataPtr(_currentChunk);
				td::UINT4 diff = SINGLEBUFFLEN - _nChInCurrent;
                mem::fastCopy(buff + _nChInCurrent, pStr, diff);
				td::UINT4 nRem = nLen - diff;

				if (nRem > 0)
					putStartingFromNewChunk(pStr + diff, nRem);
			}
			else
			{
				putStartingFromNewChunk(pStr, nLen);
			}

			_nTotalCh += nLen;
		}
	public:
		StringBuilder(bool doInitialAlloc = true)
			: BASE()
			, _nChInCurrent(0)
			, _nTotalCh(0)
			, _nTotalChReserved(0)
		{
			if (doInitialAlloc)
				initialAlloc();
			else
			{
				_currentChunk = nullptr;
			}			
		}		


		inline void reset()
		{
			_nChInCurrent = 0;
			_nTotalCh = 0;			
			_currentChunk = BASE::begin();
		}

		void initialAlloc()
		{	
			reset();

			if (BASE::isEmpty())
			{
				BASE::push();
				_currentChunk = BASE::begin();
				_nTotalChReserved += SINGLEBUFFLEN;
			}
			
		}
		//brise sve buffer osim prvog
		void reduceMemToOneBuffer()
		{			
			typename BASE::tEntry* pRoot = BASE::_root;

			typename BASE::tEntry* pToRem = BASE::_root->next;
			pRoot->next = nullptr;
			while (pToRem)
			{
				typename BASE::tEntry* pEntry = pToRem->next;
				delete pToRem;
				pToRem = pEntry;
			}
			//BASE::_last = pRoot;

			reset();
			if (pRoot)
				_nTotalChReserved = SINGLEBUFFLEN;						
		}

		inline td::UINT4 length() const
		{
			return _nTotalCh;
		}

		inline td::UINT4 capacity() const
		{
			return _nTotalChReserved;
		}

		template <bool EXTERN_ALLOCATOR>
		inline void appendString(const td::StringBase<CH, td::EncodingUTF8, 4, EXTERN_ALLOCATOR>& str)
		{
			if (str.length() <= 0)
				return;

			putString(str.c_str(), (td::UINT4) str.length());
		}

		template <size_t size>
		inline void appendCString(const char(&strDestination)[size])
		{
			td::UINT4 nLen = (td::UINT4)size - 1;
			//appendString(strDestination, nLen);
			putString(strDestination, nLen);
		}

		inline void appendString(size_t nLen, const CH* pStr)
		{
			if (pStr == nullptr)
				return;

			if (nLen == 0)
				return;
			
			putString(pStr, (td::UINT4) nLen);
		}

		//template <typename TINSTR>
		inline void appendString(const CH* pStr, int nLen = -1)
		{
			if (pStr == nullptr)
				return;

			if (nLen < 0)
				nLen = (int)td::strLen(pStr);

			if (nLen <= 0)
				return;

			putString(pStr, (td::UINT4) nLen);
		}

		template <class TSTRARR>
		void appendStringArray(const TSTRARR& arr, const char* pStrSep = ", ", bool appendEmpty=false)
		{
			auto it = arr.cbegin();
			auto itEnd = arr.cend();
			int i = 0;
			while (it != itEnd)
			{
				if (appendEmpty || (it->length() > 0) )
				{
					if (i > 0)
						appendString(pStrSep);
					appendString(*it);
				}
				++it;
				++i;
			}
		}

		//for binary data holder
		inline void appendBytes(const td::BYTE* pStr, td::UINT4 nLen)
		{
			if (pStr == nullptr)
				return;
			if (nLen == 0)
				return;

			putString((CH*)pStr, nLen);
		}

		inline StringBuilder& operator << (reset_StringBuilder)
		{
			this->reset();
			return *this;
		}

		inline StringBuilder& operator << (newline_StringBuilder)
		{
			char c = '\n';
			putString(&c, 1);
			return *this;
		}

		template <size_t size>		
		inline StringBuilder& operator << (const char (&strDestination)[size])
		{
			appendCString(strDestination);
			return *this;
		}

		inline StringBuilder& operator << (const CH* pStr)
		{
			appendString(pStr);
			return *this;
		}

		inline StringBuilder& operator << (const CH& ch)
		{
			putString(&ch, 1);
			return *this;
		}

		inline StringBuilder& operator << (const std::string& str)
		{
			if (str.length() <= 0)
				return *this;
			assert(typeid(CH) == typeid(char));
			putString(str.c_str(), (td::UINT4) str.length());
			return *this;
		}

		inline StringBuilder& operator << (const TUSEDSTRING& str)
		{
			appendString(str);
			return *this;
		}

		inline StringBuilder& operator << (const StringBuilder& val)
		{			
			return operator << (val.toString());
		}


        template <typename TVAL>
		inline StringBuilder& operator << (const TVAL& val)
		{
            appendString(_conv.c_str(val));
			return *this;
		}

//                template <typename TVAL>
//                inline StringBuilder& operator << (const TVAL* val)
//                {
//                        appendString(_conv.c_str(val));
//                        return *this;
//                }

		//template <class TSTR>

		inline bool storeToPreallocated(TUSEDSTRING& str) const
		{	
			assert(_nTotalCh == str.length());

			if (_nTotalCh == 0)
				return true;
			
			if (_nTotalCh != str.length())
				return false;		

			CONST_BASE_ITER it(BASE::begin());
			CH* pStr = const_cast<CH*>(str.c_str());
			while (it != _currentChunk)
			{
				const CH* pCurr = getDataPtr(it);
                mem::fastCopy(pStr, pCurr, SINGLEBUFFLEN);
				pStr += SINGLEBUFFLEN;
				++it;
			}

			if (_nChInCurrent != 0)
			{
				CONST_BASE_ITER itC(_currentChunk);
				const CH* pCurr = getDataPtr(itC);
                mem::fastCopy(pStr, pCurr, _nChInCurrent);
			}
			//set trailing zero
			return true;
		}

		inline bool getString(TUSEDSTRING& str, bool trim = false) const
		{
			str.clean();
			if (_nTotalCh == 0)
				return true;
				

			if (!trim)
			{
				if (!str.prepareBuffer(_nTotalCh))
					return false;

				return storeToPreallocated(str);
			}				
			else
			{
				TUSEDSTRING strTmp;
				if (!strTmp.prepareBuffer(_nTotalCh))
					return false;

				if (storeToPreallocated(strTmp))
				{
					str = strTmp.trim();
					return true;
				}				
			}
			return false;
		}

		inline td::String toString() const
		{
			td::String strRet;
			getString(strRet);
			return strRet;
		}

		template <class TOS>
		inline TOS& serialize(TOS& f) const
		{
			CONST_BASE_ITER it(BASE::begin());
			while (it != _currentChunk)
			{
				f.write(getDataPtr(it), SINGLEBUFFLEN);
				++it;
			}
			if (_nChInCurrent != 0)
			{
				CONST_BASE_ITER itC(_currentChunk);
				const CH* pCurr = getDataPtr(itC);
				f.write(pCurr, _nChInCurrent);
			}
				
			return f;
		}

		//output to ostream
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const StringBuilder& rhs)
		{
			rhs.serialize(os);
			return os;
		}
		//template <class TSTR>
		//void getString(TSTR& str, const char linSeparator) const
		//{
		//	str.clean();
		//	typename cnt::ListSL<TUSEDSTRING>::tEntry* pItem = cnt::ListSL<TUSEDSTRING>::_root;
		//	//get the size
		//	int totalSize = 0;
		//	int i = 0;
		//	while (pItem)
		//	{
		//		if (i > 0)
		//			++totalSize;

		//		++i;
		//		totalSize += pItem->data.length();
		//		pItem = pItem->next;
		//	}

		//	if (str.prepareBuffer(totalSize))
		//	{
		//		char* pBeg = str.getBegin();
		//		pItem = cnt::ListSL<TUSEDSTRING>::_root;
		//		while (pItem)
		//		{
		//			int nLen = pItem->data.length();
		//			td::fastCopyMem((char*)pBeg, (const char*)pItem->data.getBegin(), nLen);
		//			//memcpy((void*) pBeg,  (void*) pItem->data.getBegin(), nLen);
		//			pBeg += nLen;
		//			pItem = pItem->next;
		//			if (pItem)
		//			{
		//				*pBeg = linSeparator;
		//				++pBeg;
		//			}
		//		}
		//	}
		//}
	};

	typedef StringBuilder<td::String, 1024> StringBuilderSmall;
	typedef StringBuilder<td::String, 4*1024> StringBuilderNormal;
	typedef StringBuilder<td::String, 64*1024> StringBuilderLarge;
}
