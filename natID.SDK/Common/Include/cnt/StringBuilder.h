// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StringBuilder.h
 @brief Chunked string builder that accumulates character data across multiple fixed-size buffers. */
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
	/// @brief Tag type used as a stream manipulator to reset a StringBuilder.
	class reset_StringBuilder {};
	constexpr reset_StringBuilder clear; ///< Manipulator instance that resets a StringBuilder when streamed.

	/// @brief Tag type used as a stream manipulator to append a newline to a StringBuilder.
	class newline_StringBuilder {};
	constexpr newline_StringBuilder endl; ///< Manipulator instance that appends a newline when streamed.


	/// @brief Accumulates text across a linked list of fixed-size character buffers.
	/// @tparam TUSEDSTRING  String type used when converting accumulated content.
	/// @tparam SINGLEBUFFLEN Size in characters of each internal chunk buffer.
	template <class TUSEDSTRING = td::String, td::UINT4 SINGLEBUFFLEN = 1024 * 64>
	class StringBuilder : public cnt::List<typename TUSEDSTRING::CHAR_TYPE[SINGLEBUFFLEN], false>
	{
		friend class no::sock::Communication;
	protected:
		typedef typename TUSEDSTRING::CHAR_TYPE CH; ///< Character type derived from the string type.
		typedef cnt::List<CH[SINGLEBUFFLEN], false> BASE; ///< Base list type holding character buffers.
		typedef typename BASE::iterator BASE_ITER; ///< Mutable iterator over the chunk list.
		typedef typename BASE::const_iterator CONST_BASE_ITER; ///< Const iterator over the chunk list.
		td::UINT4 _nChInCurrent;       ///< Number of characters written into the current chunk.
		td::UINT4 _nTotalCh;           ///< Total number of characters accumulated so far.
		td::UINT4 _nTotalChReserved;   ///< Total number of characters allocated across all chunks.
		BASE_ITER _currentChunk;       ///< Iterator pointing to the chunk currently being written.
		//CH* _currentBuffer;
		mu::Utils _conv; ///< Utility object used for converting numeric values to strings.
		//cnt::ListSL<CH[BUFFLEN]> _data;

		/// @brief Returns a pointer to the character data of the chunk referenced by the given iterator.
		/// @param it Iterator pointing to the chunk.
		/// @return Pointer to the chunk's character buffer.
		inline CH* getDataPtr(BASE_ITER& it) const
		{
			return (CH*)(&((*it)[0]));
		}

		/// @brief Returns a const pointer to the character data of the chunk referenced by the given iterator.
		/// @param it Const iterator pointing to the chunk.
		/// @return Const pointer to the chunk's character buffer.
		inline const CH* getDataPtr(CONST_BASE_ITER& it) const
		{
			return (const CH*)(&((*it)[0]));
		}

		/// @brief Returns the number of characters written into the current chunk.
		/// @return Character count in the current chunk.
		inline td::UINT4 getLastLen() const
		{
			return _nChInCurrent;
		}

		/// @brief Returns the capacity in characters of each chunk buffer.
		/// @return Buffer length per chunk.
		inline td::UINT4 getBuffLen() const
		{
			return SINGLEBUFFLEN;
		}

		/// @brief Copies len characters from pStr into new chunks, starting from the next available chunk.
		/// @param pStr Pointer to the source characters.
		/// @param len  Number of characters to copy.
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

		/// @brief Appends nLen characters from pStr, spanning chunks as needed.
		/// @param pStr Pointer to the source characters.
		/// @param nLen Number of characters to append.
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
		/// @brief Constructs a StringBuilder, optionally performing the initial chunk allocation.
		/// @param doInitialAlloc When true, allocates the first chunk immediately.
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


		/// @brief Resets the write position to the beginning without freeing chunk buffers.
		inline void reset()
		{
			_nChInCurrent = 0;
			_nTotalCh = 0;
			_currentChunk = BASE::begin();
		}

		/// @brief Ensures the first chunk exists and resets the write position to the beginning.
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
		/// @brief Releases all chunks except the first, then resets the write position.
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

		/// @brief Returns the total number of characters accumulated.
		/// @return Character count.
		inline td::UINT4 length() const
		{
			return _nTotalCh;
		}

		/// @brief Returns the total number of characters that can be held in allocated chunks.
		/// @return Total allocated character capacity.
		inline td::UINT4 capacity() const
		{
			return _nTotalChReserved;
		}

		/// @brief Appends a UTF-8 string object.
		/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
		/// @param  str The string to append.
		template <bool EXTERN_ALLOCATOR>
		inline void appendString(const td::StringBase<CH, td::EncodingUTF8, 4, EXTERN_ALLOCATOR>& str)
		{
			if (str.length() <= 0)
				return;

			putString(str.c_str(), (td::UINT4) str.length());
		}

		/// @brief Appends a compile-time C-string literal.
		/// @tparam size Compile-time array size (includes null terminator).
		/// @param  strDestination The C-string literal to append.
		template <size_t size>
		inline void appendCString(const char(&strDestination)[size])
		{
			td::UINT4 nLen = (td::UINT4)size - 1;
			//appendString(strDestination, nLen);
			putString(strDestination, nLen);
		}

		/// @brief Appends nLen characters from the raw character pointer pStr.
		/// @param nLen Number of characters to append.
		/// @param pStr Pointer to the character data; no-op if nullptr.
		inline void appendString(size_t nLen, const CH* pStr)
		{
			if (pStr == nullptr)
				return;

			if (nLen == 0)
				return;

			putString(pStr, (td::UINT4) nLen);
		}

		//template <typename TINSTR>
		/// @brief Appends a null-terminated string, optionally using a provided length.
		/// @param pStr Pointer to the null-terminated string; no-op if nullptr.
		/// @param nLen Number of characters to append; computed from pStr if negative.
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

		/// @brief Appends all non-empty strings from an array, separated by pStrSep.
		/// @tparam TSTRARR  Container type with cbegin/cend iterators.
		/// @param  arr       Container of strings to append.
		/// @param  pStrSep   Separator string inserted between elements.
		/// @param  appendEmpty When true, empty strings are included.
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
		/// @brief Appends raw binary bytes.
		/// @param pStr Pointer to the byte data; no-op if nullptr.
		/// @param nLen Number of bytes to append.
		inline void appendBytes(const td::BYTE* pStr, td::UINT4 nLen)
		{
			if (pStr == nullptr)
				return;
			if (nLen == 0)
				return;

			putString((CH*)pStr, nLen);
		}

		/// @brief Resets the builder when the reset manipulator is streamed.
		/// @param  (unnamed) reset_StringBuilder tag.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (reset_StringBuilder)
		{
			this->reset();
			return *this;
		}

		/// @brief Appends a newline character when the endl manipulator is streamed.
		/// @param  (unnamed) newline_StringBuilder tag.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (newline_StringBuilder)
		{
			char c = '\n';
			putString(&c, 1);
			return *this;
		}

		/// @brief Appends a compile-time C-string literal.
		/// @tparam size Array size including the null terminator.
		/// @param  strDestination The literal to append.
		/// @return Reference to this builder.
		template <size_t size>
		inline StringBuilder& operator << (const char (&strDestination)[size])
		{
			appendCString(strDestination);
			return *this;
		}

		/// @brief Appends a null-terminated character pointer.
		/// @param pStr The string to append.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (const CH* pStr)
		{
			appendString(pStr);
			return *this;
		}

		/// @brief Appends a single character.
		/// @param ch The character to append.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (const CH& ch)
		{
			putString(&ch, 1);
			return *this;
		}

		/// @brief Appends a std::string.
		/// @param str The std::string to append.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (const std::string& str)
		{
			if (str.length() <= 0)
				return *this;
			assert(typeid(CH) == typeid(char));
			putString(str.c_str(), (td::UINT4) str.length());
			return *this;
		}

		/// @brief Appends a TUSEDSTRING instance.
		/// @param str The string to append.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (const TUSEDSTRING& str)
		{
			appendString(str);
			return *this;
		}

		/// @brief Appends the contents of another StringBuilder.
		/// @param val The source StringBuilder.
		/// @return Reference to this builder.
		inline StringBuilder& operator << (const StringBuilder& val)
		{
			return operator << (val.toString());
		}


        /// @brief Appends a generic value by converting it to a string via mu::Utils.
        /// @tparam TVAL  Value type convertible by mu::Utils::c_str.
        /// @param  val   The value to append.
        /// @return Reference to this builder.
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

		/// @brief Copies accumulated content into a pre-allocated TUSEDSTRING of the same length.
		/// @param str Pre-allocated output string whose length must equal length().
		/// @return True on success.
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

		/// @brief Copies accumulated content into a TUSEDSTRING, optionally trimming whitespace.
		/// @param str    Destination string.
		/// @param trim   When true, the result is trimmed before assignment.
		/// @return True on success.
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

		/// @brief Returns the accumulated content as a new td::String.
		/// @return New string containing all accumulated characters.
		inline td::String toString() const
		{
			td::String strRet;
			getString(strRet);
			return strRet;
		}

		/// @brief Writes all accumulated character data to the given output stream or file object.
		/// @tparam TOS  Stream/file type with a write() method.
		/// @param  f    The output target.
		/// @return Reference to f.
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

	typedef StringBuilder<td::String, 1024> StringBuilderSmall; ///< StringBuilder with 1 KB chunks.
	typedef StringBuilder<td::String, 4*1024> StringBuilderNormal; ///< StringBuilder with 4 KB chunks.
	typedef StringBuilder<td::String, 64*1024> StringBuilderLarge; ///< StringBuilder with 64 KB chunks.
}
