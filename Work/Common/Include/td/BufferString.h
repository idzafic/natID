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
#include <istream>
#include <mem/IBufferReader.h>

namespace td
{
	//class BufferString
	//Use Cases:
	//a: BS_INITIAL_SIZE > 0 && BS_INCREMENT > 0 used as input buffer for getLine
	//b: BS_INITIAL_SIZE > 0 && BS_INCREMENT == 0 used as input buffer for binary file
	//c: BS_INITIAL_SIZE == 0 && BS_INCREMENT == 0 used as wrapper for const T*

	template<typename T, int BS_INITIAL_SIZE, int BS_INCREMENT>
	class BufferString
	{
		T* _buffer = nullptr;
		std::istream* _pFile = nullptr;
        mem::IBufferReader* _pBufferReader = nullptr;
        int _size = 0;            //a: current buffer size, b: bytes read, c: string len
        int _currPos = 0;        //a: current end of string, b: c: current read position,
		bool _whiteSpaceOnly;
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

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

		~BufferString()			
		{
			//clear for case a i b
			if (BS_INITIAL_SIZE > 0)
				delete [] _buffer;
		}

		iterator begin()
		{
			return _buffer;
		}

		const_iterator begin() const
		{
			return _buffer; 
		}

		iterator currentPosition()
		{
			return _buffer + _currPos;
		}

		const_iterator currentPosition() const
		{
			return _buffer + _currPos;
		}

		const_iterator end() const
		{
			return _buffer + _size;
		}

		T getLastChar() const
		{
			if (_currPos > 0)
			{
				return _buffer[_currPos - 1];
			}
			return (T)0;
		}

		T getFirstChar() const
		{
			if (_currPos > 0)
			{
				return _buffer[0];
			}
			return (T)0;
		}

		void removeLastChar()
		{
			if (_currPos > 0)
			{
				--_currPos;
				_buffer[_currPos] = 0;
			}
		}

		void setFileAsInput(std::istream* pInputFile)
		{
			//case b
			assert (BS_INITIAL_SIZE > 0);
			assert (BS_INCREMENT == 0);			
			_pFile = pInputFile;
		}

		void setBufferAsInput(const T* pInputBuffer)
		{
			//case c
			assert (BS_INITIAL_SIZE == 0);
			assert (BS_INCREMENT == 0);			
			_buffer = const_cast<T*>(pInputBuffer);
			_size = (int) strlen(pInputBuffer);
			//_pFile = pInputFile;
		}
        
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

		void erase()
		{
			assert(_size > 0);
			_currPos = 0;
			_buffer[0] = 0;
			_whiteSpaceOnly = true;
		}

		void clean()
		{
			assert(_size > 0);
			_currPos = 0;
			_buffer[0] = 0;
			_whiteSpaceOnly = true;
		}

		int max_size() const
		{
			assert(BS_INCREMENT > 0);
			return 10000000;
		}

		int size() const
		{
			if ((BS_INCREMENT > 0))
				return _currPos;
			else
				return _size;
		}

		int length() const
		{
			return _currPos;
		}

		void toUpper() 
		{
			for (int i = 0; i<_currPos; ++i)
			{
				if ( (_buffer[i] >='a') && (_buffer[i] <='z'))
					_buffer[i] += 'A' - 'a';
			}
			
		}

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

		const T* c_str() const
		{
			assert(BS_INITIAL_SIZE > 0);

			_buffer[_currPos] = 0;

			return _buffer;
		}

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

		template <class TSTR>
		void str(TSTR& strOut) const
		{
			strOut.fromKnownString(c_str(), length());
		}

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

		bool isWhiteSpaceOnly() const
		{
			return _whiteSpaceOnly;
		}

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
typedef td::BufferString<td::UTF8, 512,    256>  FileString8;
typedef td::BufferString<td::UTF16, 512*2, 512>  FileString16;
typedef td::BufferString<td::UTF32, 512*4, 1024> FileString32;

//SAX parser input bufferi (file)
typedef td::BufferString<td::UTF8, 1024,      0>   FileBuffer1k;
typedef td::BufferString<td::UTF8, 1024 * 4,  0>   FileBuffer4k;
typedef td::BufferString<td::UTF8, 1024 * 16, 0>   FileBuffer16k;
typedef td::BufferString<td::UTF8, 1024 * 64, 0>   FileBuffer64k;

//SAX parser input bufferi (memory)
typedef td::BufferString<td::UTF8, 0, 0>      StringWrapper8;
typedef td::BufferString<td::UTF16, 0, 0>     StringWrapper16;
typedef td::BufferString<td::UTF32, 0, 0>     StringWrapper32;
