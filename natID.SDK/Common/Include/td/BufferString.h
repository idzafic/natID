// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BufferString.h
    @brief Defines the BufferString template class for use as a resizable string buffer, fixed binary file buffer, or lightweight string wrapper. */
#pragma once
#include <td/Types.h>
#include <istream>
#include <mem/IBufferReader.h>

namespace td
{
	//class BufferString
	//Use Cases:
	//a: BS_INITIAL_SIZE > 0 && BS_INCREMENT > 0 used as input buffer for getLine
	//b: BS_INITIAL_SIZE > 0 && BS_INCREMENT == 0 used as input buffer for binary file
	//c: BS_INITIAL_SIZE == 0 && BS_INCREMENT == 0 used as wrapper for const T*

	/// @brief A template buffer class that can serve as a dynamically resizable output string buffer,
	///        a fixed-size binary file read buffer, or a lightweight wrapper around an existing C-string.
	/// @tparam T The character type of the buffer (e.g., td::UTF8, td::UTF16, td::UTF32).
	/// @tparam BS_INITIAL_SIZE The initial allocation size in elements; 0 means the buffer acts as a wrapper.
	/// @tparam BS_INCREMENT The growth increment in elements; 0 means the buffer is non-growable.
	template<typename T, int BS_INITIAL_SIZE, int BS_INCREMENT>
	class BufferString
	{
		T* _buffer = nullptr;               ///< Pointer to the underlying character buffer.
		std::istream* _pFile = nullptr;     ///< Pointer to the file stream used as input for binary loading.
        mem::IBufferReader* _pBufferReader = nullptr; ///< Pointer to a memory buffer reader interface used as an alternative to file input.
        int _size = 0;            //a: current buffer size, b: bytes read, c: string len
        int _currPos = 0;        //a: current end of string, b: c: current read position,
		bool _whiteSpaceOnly;               ///< Indicates whether all characters appended so far are whitespace.
	public:
		typedef T* iterator;             ///< Mutable iterator type for traversing the buffer.
		typedef const T* const_iterator; ///< Immutable iterator type for traversing the buffer.

		/// @brief Default constructor that allocates the buffer for use cases (a) and (b), or leaves it null for case (c).
		BufferString()
			: _buffer(nullptr)
            , _pFile(nullptr)
            , _size(0)
			, _currPos(0)
			, _whiteSpaceOnly(true)
		{
			if (BS_INITIAL_SIZE > 0)
			{
				//initialized for case a i b
				_size = BS_INITIAL_SIZE;
				_buffer = new T[BS_INITIAL_SIZE + 1];
			}
			//_buffer[0] = 0;
		}

		/// @brief Destructor that frees the allocated buffer for use cases (a) and (b).
		~BufferString()
		{
			//clear for case a i b
			if (BS_INITIAL_SIZE > 0)
				delete [] _buffer;
		}

		/// @brief Returns an iterator to the first element of the buffer.
		/// @return A mutable pointer to the beginning of the buffer.
		iterator begin()
		{
			return _buffer;
		}

		/// @brief Returns a const iterator to the first element of the buffer.
		/// @return An immutable pointer to the beginning of the buffer.
		const_iterator begin() const
		{
			return _buffer;
		}

		/// @brief Returns an iterator to the current write/read position within the buffer.
		/// @return A mutable pointer to the current position in the buffer.
		iterator currentPosition()
		{
			return _buffer + _currPos;
		}

		/// @brief Returns a const iterator to the current write/read position within the buffer.
		/// @return An immutable pointer to the current position in the buffer.
		const_iterator currentPosition() const
		{
			return _buffer + _currPos;
		}

		/// @brief Returns a const iterator one past the last allocated element of the buffer.
		/// @return An immutable pointer past the end of the allocated buffer.
		const_iterator end() const
		{
			return _buffer + _size;
		}

		/// @brief Returns the last character currently stored in the buffer.
		/// @return The last character, or zero if the buffer is empty.
		T getLastChar() const
		{
			if (_currPos > 0)
			{
				return _buffer[_currPos - 1];
			}
			return (T)0;
		}

		/// @brief Returns the first character currently stored in the buffer.
		/// @return The first character, or zero if the buffer is empty.
		T getFirstChar() const
		{
			if (_currPos > 0)
			{
				return _buffer[0];
			}
			return (T)0;
		}

		/// @brief Removes the last character from the buffer by decrementing the position and placing a null terminator.
		void removeLastChar()
		{
			if (_currPos > 0)
			{
				--_currPos;
				_buffer[_currPos] = 0;
			}
		}

		/// @brief Associates a file stream as the data input source for binary file reading (use case b).
		/// @param pInputFile Pointer to an open input stream to read data from.
		void setFileAsInput(std::istream* pInputFile)
		{
			//case b
			assert (BS_INITIAL_SIZE > 0);
			assert (BS_INCREMENT == 0);
			_pFile = pInputFile;
		}

		/// @brief Associates an existing C-string as the data source for use as a string wrapper (use case c).
		/// @param pInputBuffer Pointer to the null-terminated string to wrap.
		void setBufferAsInput(const T* pInputBuffer)
		{
			//case c
			assert (BS_INITIAL_SIZE == 0);
			assert (BS_INCREMENT == 0);
			_buffer = const_cast<T*>(pInputBuffer);
			_size = (int) strlen(pInputBuffer);
			//_pFile = pInputFile;
		}

		/// @brief Associates an in-memory buffer reader as the data source for use as a string wrapper (use case c).
		/// @param pIMemBuffer Pointer to the IBufferReader interface providing data chunks.
        void setMemBufferAsInput(mem::IBufferReader* pIMemBuffer)
        {
            //case c
            assert (BS_INITIAL_SIZE == 0);
            assert (BS_INCREMENT == 0);
            _pBufferReader = pIMemBuffer;
            auto [pChunk, size] = _pBufferReader->getDataChunk();
            _buffer = pChunk;
            _size = (int) size;
            //_pFile = pInputFile;
        }

		/// @brief Loads the next chunk of data from the associated file or memory buffer reader into the internal buffer.
		/// @return True if data was successfully loaded, false if no more data is available or no source is set.
		bool load()
		{
			//through can be return only in case b
			if(!_pFile && !_pBufferReader)
				return false;

            if (_pBufferReader)
            {
                auto [pChunk, size] = _pBufferReader->getDataChunk();
                if (pChunk && size > 0)
                {
                    _buffer = pChunk;
                    _size = (int) size;
                    return true;
                }
                else
                {
                    _currPos = 0;
                    _size = 0;
                    return false;
                }
            }
            else if (_pFile->good() && !_pFile->eof())
			{
				std::streamoff pos1 = _pFile->tellg();
				if (pos1 >= 0)
				{
					_pFile->read((char*) _buffer, BS_INITIAL_SIZE);
                    _size = (int) _pFile->gcount();
                    if (_size > 0)
                        return true;
                    else
                    {
                        _currPos = 0;
                        _size = 0;
                        return false;
                    }
				}
				_currPos = 0;
				_size = 0;
				return false;
			}
			return false;
		}

		/// @brief Resets the current position to the beginning and clears the first element, marking the buffer as empty.
		void erase()
		{
			assert(_size > 0);
			_currPos = 0;
			_buffer[0] = 0;
			_whiteSpaceOnly = true;
		}

		/// @brief Resets the current position to the beginning and clears the first element, marking the buffer as empty.
		void clean()
		{
			assert(_size > 0);
			_currPos = 0;
			_buffer[0] = 0;
			_whiteSpaceOnly = true;
		}

		/// @brief Returns the maximum number of characters the buffer can theoretically hold.
		/// @return A large constant representing the effective maximum capacity.
		int max_size() const
		{
			assert(BS_INCREMENT > 0);
			return 10000000;
		}

		/// @brief Returns the number of valid elements currently stored in the buffer.
		/// @return The current position for growable buffers (case a), or the loaded size for fixed buffers (case b/c).
		int size() const
		{
			if ((BS_INCREMENT > 0))
				return _currPos;
			else
				return _size;
		}

		/// @brief Returns the current string length (number of characters before the current position).
		/// @return The current write position, which equals the number of stored characters.
		int length() const
		{
			return _currPos;
		}

		/// @brief Converts all lowercase ASCII letters in the buffer to uppercase in place.
		void toUpper()
		{
			for (int i = 0; i<_currPos; ++i)
			{
				if ( (_buffer[i] >='a') && (_buffer[i] <='z'))
					_buffer[i] += 'A' - 'a';
			}
		}

		/// @brief Checks whether all characters in the buffer are ASCII decimal digits.
		/// @return True if the buffer is non-empty and every character is a digit '0'-'9', false otherwise.
		bool isNumber()
		{
			if (_currPos == 0)
				return false;

			for (int i = 0; i<_currPos; ++i)
			{
				if ((_buffer[i] >= '0') && (_buffer[i] <= '9'))
					continue;
				else
					return false;
			}
			return true;
		}

		/// @brief Returns a null-terminated C-string representation of the current buffer contents.
		/// @return Pointer to the internal buffer with a null terminator placed at the current position.
		const T* c_str() const
		{
			assert(BS_INITIAL_SIZE > 0);

			_buffer[_currPos] = 0;

			return _buffer;
		}

		/// @brief Appends a sequence of characters to the buffer, growing it if needed.
		/// @param nLen Number of characters to append.
		/// @param str Pointer to the character array to append.
		void append(int nLen, const T* str)
		{
			assert(BS_INCREMENT > 0);

			if (_currPos + nLen >= _size)
			{
				assert(BS_INCREMENT > 0);
				_size += BS_INCREMENT + nLen;
				T* pTmp = new T[_size + 1];
				memcpy(pTmp, _buffer, _currPos * sizeof(T));
				delete [] _buffer;
				_buffer = pTmp;
			}
			//_buffer[_currPos++] = val;
			memcpy(_buffer + _currPos, str, nLen);
			_currPos += nLen;
		}

		/// @brief Copies the buffer content into a target string object using fromKnownString.
		/// @tparam TSTR The target string type that provides a fromKnownString method.
		/// @param strOut The string object to populate with the buffer content.
		template <class TSTR>
		void str(TSTR& strOut) const
		{
			strOut.fromKnownString(c_str(), length());
		}

		/// @brief Copies the buffer content into a target string, optionally trimming whitespace.
		/// @tparam TSTR The target string type that provides fromKnownString and trim methods.
		/// @param strOut The string object to populate with the buffer content.
		/// @param trim If true, the resulting string is trimmed of leading and trailing whitespace.
		template <class TSTR>
		void getString(TSTR& strOut, bool trim = false) const
		{
			if (!trim)
				strOut.fromKnownString(c_str(), length());
			else
			{
				TSTR strIn;
				strIn.fromKnownString(c_str(), length());
				strOut = strIn.trim();
			}
		}

		/// @brief Checks whether all characters appended to the buffer so far were whitespace.
		/// @return True if only whitespace characters (space, tab, newline, carriage return) have been appended.
		bool isWhiteSpaceOnly() const
		{
			return _whiteSpaceOnly;
		}

		/// @brief Appends a single character to the buffer, growing the buffer if needed.
		/// @param val The character to append.
		void operator += (const T val)
		{
			assert(BS_INCREMENT > 0);

			if (_whiteSpaceOnly)
			{
				if (val != '\n' && val != ' ' && val != '\t' && val != '\r')
					_whiteSpaceOnly = false;
			}
			if (_currPos == _size)
			{
				_size += BS_INCREMENT;
				T* pTmp = new T[_size + 1];
				memcpy(pTmp, _buffer, _currPos * sizeof(T));
				delete [] _buffer;
				_buffer = pTmp;
			}
			_buffer[_currPos++] = val;
			//_buffer[_currLen] = 0;
		}
	};
}

//SAX parser output buffer
typedef td::BufferString<td::UTF8, 512,    256>  FileString8;  ///< SAX parser output buffer for UTF-8 with 512-byte initial size and 256-byte growth increment.
typedef td::BufferString<td::UTF16, 512*2, 512>  FileString16; ///< SAX parser output buffer for UTF-16 with 1024-byte initial size and 512-byte growth increment.
typedef td::BufferString<td::UTF32, 512*4, 1024> FileString32; ///< SAX parser output buffer for UTF-32 with 2048-byte initial size and 1024-byte growth increment.

//SAX parser input bufferi (file)
typedef td::BufferString<td::UTF8, 1024,      0>   FileBuffer1k;  ///< SAX parser file input buffer with 1 KB capacity.
typedef td::BufferString<td::UTF8, 1024 * 4,  0>   FileBuffer4k;  ///< SAX parser file input buffer with 4 KB capacity.
typedef td::BufferString<td::UTF8, 1024 * 16, 0>   FileBuffer16k; ///< SAX parser file input buffer with 16 KB capacity.
typedef td::BufferString<td::UTF8, 1024 * 64, 0>   FileBuffer64k; ///< SAX parser file input buffer with 64 KB capacity.

//SAX parser input bufferi (memory)
typedef td::BufferString<td::UTF8, 0, 0>      StringWrapper8;  ///< Lightweight UTF-8 string wrapper with no internal allocation.
typedef td::BufferString<td::UTF16, 0, 0>     StringWrapper16; ///< Lightweight UTF-16 string wrapper with no internal allocation.
typedef td::BufferString<td::UTF32, 0, 0>     StringWrapper32; ///< Lightweight UTF-32 string wrapper with no internal allocation.
