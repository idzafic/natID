// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "Archive.h"
#include "IBufferProvider.h"

namespace arch
{
	template <class TARCHIVE, bool bWrite>
	class MemorySerializer : public IBinSerializer
	{
	protected:
		IBufferProvider* _bufferProvider;
		char* _currBuffer;
		td::UINT4 _bufferSize;
		td::UINT4 _remainingSize;
		char* _curPos;  //pointer pointing to current position in _currBuffer;
		TARCHIVE* _arch;
		Header* _header;
		//bool _showHeaderOnEveryPage = true;
		bool _bError;		
	protected:		
				
		void getNextBuffer()
		{
			//bool firstBuffer = false;
			if (_currBuffer)
			{
				if (bWrite)
				{
					writePayloadSize();
				}

				_bufferProvider->completed(_currBuffer, _header->size(), _header->payloadSize, _bError);
			}
			//else
			//{
			//	firstBuffer = true;
			//}

			if (_bufferProvider)
			{
				_currBuffer = _bufferProvider->get(_bufferSize);
				_remainingSize = _bufferSize;				

				//int headerSize = _header->size();

				//assert(_bufferSize >= (td::UINT4) _header->size());
				_curPos = _currBuffer;
			}

			if (_currBuffer == nullptr)
			{
				if (bWrite)
				{
					close();
					arch::Exception e;
					e.str = "Unexpected end! MemorySerializer::getNextBuffer!";
					throw e;
				}		
				else
				{
					//in case when stream is open but buffer empty 
					return;
				}
			}		
			

			//bool encHeader = _showHeaderOnEveryPage || firstBuffer;
			//if (!encHeader)
			//	return;
			
			if (bWrite)
			{	
				_header = _arch->encodeHeader2();
			}
			else
			{
				_header = _arch->decodeHeader2(_currBuffer);
				if (!_header)
				{
					close();
					return;
				}
				int nBytes = _header->size();
				_remainingSize -= nBytes;
				_curPos += nBytes;

				if (_header->lastPayload && (_remainingSize == 0))
					close();
			}
		}

		void writePayloadSize()
		{
			if (bWrite)
			{
				if (_currBuffer)
				{
					if (_curPos != _currBuffer)
					{						
						td::UINT4 size = _bufferSize - _remainingSize - _header->size();
						_header->writePayloadSize(_currBuffer, size);					
					}					
				}
			}
		}

		void finish()
		{			
			if (_currBuffer)
			{
				if (_curPos != _currBuffer)
				{
					if (bWrite)
					{
						writePayloadSize();
						markAsLastPayload();
					}

					_bufferProvider->completed(_currBuffer, _header->size(), _header->payloadSize, _bError);
				}
			}				
							
			_currBuffer = nullptr;
			_curPos = nullptr;
			_remainingSize = 0;
			_bufferSize = 0;
		}
	public:
		MemorySerializer()
			: _bufferProvider(nullptr)			
			, _currBuffer(nullptr)
			, _bufferSize(0)
			, _remainingSize(0)		
			, _curPos(nullptr)
			, _arch(nullptr)
			, _header(nullptr)			
			, _bError(false)			
		{
		}


		//void showHeaderOnEveryPage(bool showOnEveryPage)
		//{
		//	_showHeaderOnEveryPage = showOnEveryPage;
		//}
		
		td::UINT4 getHeaderSize() const
		{
			if (!_header)
				return 0;

			return _header->size();
		}

		td::UINT4 getBufferSize() const
		{
			return _bufferSize;
		}

		const char* getCurrentBuffer() const
		{
			//getBuffers
			return _currBuffer;
		}		

		void markAsLastPayload()
		{
			if (bWrite)
			{
				if (_currBuffer)
				{					
					_header->markAsLastPayload(_currBuffer);
				}				
			}
			else
			{
				_header->lastPayload = true;
			}
			
		}

		void setBufferProvider(IBufferProvider* pBufferProvider, TARCHIVE* pArch)
		{
			_bufferProvider = pBufferProvider;
			_arch = pArch;
		}
        
        IBufferProvider* getBufferProvider()
        {
            return _bufferProvider;
        }

		~MemorySerializer()
		{			
			close();
		}

		virtual bool open(const char*)
		{			
			getNextBuffer();
			return true;
		}

		virtual void write(const char* bytes, td::UINT4 nBytes)
		{
			if (_currBuffer == nullptr)
			{
				arch::Exception e;
				e.str = "Writing over EOF! (Archive is closed)!";
				throw e;
				return;
			}
			assert(bWrite);
			if (_remainingSize >= nBytes)
			{				
				memcpy(_curPos, bytes, nBytes);
				_remainingSize -= nBytes;
				_curPos += nBytes;
			}
			else if (_remainingSize > 0)
			{
				memcpy(_curPos, bytes, _remainingSize);
				const char* toWrite = bytes + _remainingSize;
				_curPos += _remainingSize;
				nBytes -= _remainingSize;
				_remainingSize = 0;
				getNextBuffer();
				//recursive call: write remaining bytes
				write(toWrite, nBytes);
			}
			else
			{
				//last buffer was empty
				getNextBuffer();
				//recursive call
				write(bytes, nBytes);
			}			
		}

		virtual void read(char* bytes, td::UINT4 nBytes)
		{			
			assert(!bWrite);

			if (_currBuffer == nullptr)
			{
				//assert(false);
				arch::Exception e;
				e.str = "Reading over EOF! (Archive is closed)!";
				throw e;
				return;
			}

			if (_remainingSize >= nBytes)
			{				
				memcpy(bytes, _curPos, nBytes);
				_remainingSize -= nBytes;
				_curPos += nBytes;
			}
			else if (_remainingSize > 0)
			{
				memcpy(bytes, _curPos, _remainingSize);
				char* toRead = bytes + _remainingSize;
				_curPos += _remainingSize;
				nBytes -= _remainingSize;
				_remainingSize = 0;
				getNextBuffer();
				//recursive call: write remaining bytes
				read(toRead, nBytes);
			}
			else
			{
				//last buffer was empty
				getNextBuffer();
				//recursive call
				read(bytes, nBytes);
			}	
			
			if (_header->lastPayload && (_remainingSize == 0))
				close();
		}

		virtual bool goTo(td::LUINT8 pos)
		{
			if (bWrite)
				return false;
			//TODO: dodati odlazak na poziciju
			td::UINT4 buffSize = _bufferSize;
			td::UINT4 nToSkip = (td::UINT4) (pos / buffSize);
			td::UINT4 newPos = pos % buffSize;
			if (newPos == 0)
				newPos += 32;

			_bufferProvider->reset();

			for (td::UINT4 i = 0; i <= nToSkip; ++i)
			{
				_currBuffer = _bufferProvider->get(_bufferSize);				
			}

			_header = _arch->decodeHeader2(_currBuffer);

			if (!_header)
			{
				close();
				return false;
			}

			_remainingSize = _bufferSize - newPos;
			assert(_bufferSize >= (td::UINT4) _header->size());
			_curPos = _currBuffer + newPos;
			
			return true;
		}

		virtual void close()
		{
			finish();
		}

		//virtual void setCompression(char compressionLevel)
		//{			
		//}

		template <class T>
		inline size_t readFromFile(T& f, size_t nBytes = 0)
		{	
			if (nBytes == 0)
			{
				std::streampos pos1 = f.tellg();
				f.seekg(0, f.end);
				std::streampos pos2 = f.tellg();
				f.seekg(pos1, f.beg);
				nBytes = pos2 - pos1;
			}
			
			if (nBytes == 0)
				return 0;

			if (_currBuffer == nullptr)
			{
				assert(false);
				arch::Exception e;
				e.str = "Writing over EOF! (Archive is closed)!";
				throw e;
				return 0;
			}			
			size_t toRet = 0;
			while (nBytes > 0)
			{
				//size_t headerSize = _header->size();
				size_t buffPayloadSize = _remainingSize; //bufferSize - headerSize;

				if (nBytes > buffPayloadSize)
				{
					std::streampos pos1 = f.tellg();
					f.read(_curPos, buffPayloadSize);
					std::streampos pos2 = f.tellg();
					if (pos2 - pos1 != buffPayloadSize)
					{
						toRet += (size_t) (pos2 - pos1);
						return toRet;
					}

					nBytes -= buffPayloadSize;
					toRet += buffPayloadSize;
					_remainingSize = 0;
					if (nBytes > 0)
						getNextBuffer();
				}
				else
				{
					std::streampos pos1 = f.tellg();
					f.read(_curPos, nBytes);
					std::streampos pos2 = f.tellg();		
					if (pos2 - pos1 != nBytes)
					{
						toRet += (size_t) (pos2 - pos1);
						return toRet;
					}
					toRet += nBytes;
					_remainingSize -= (td::UINT4) nBytes;
					writePayloadSize();
					nBytes = 0;
				}				
			}
			return toRet;
		}

//		template <class T>
//		inline size_t writeToFileOrig(T& f, bool withHeaders = false)
//		{
//			if (!_currBuffer)
//				return 0;
//			size_t nToRet = 0;
//			cnt::ListSL<char*>& lst = _bufferProvider->getBuffers();
//			cnt::ListSL<char*>::const_iterator it(lst.begin());
//			const char* pBuff = *it;
//			td::UINT4 headerSize = _header->size();
//			while (pBuff != _currBuffer)
//			{
//				if (withHeaders)
//				{
//					f.write(pBuff, _bufferSize);
//					nToRet += _bufferSize;
//				}					
//				else
//				{
//					td::UINT4 d = _bufferSize - headerSize;
//					f.write(pBuff + headerSize, d);
//					nToRet += d;
//				}
//					
//				++it;
//				pBuff = *it;
//			}
//			if (_header->payloadSize > 0)
//			{
//				if (withHeaders)
//				{
//					f.write(pBuff, headerSize + _header->payloadSize);
//					nToRet += headerSize + _header->payloadSize;
//				}					
//				else
//				{
//					f.write(pBuff + headerSize, _header->payloadSize);
//					nToRet += _header->payloadSize;
//				}					
//			}	
//			return nToRet;
//		}
        
        template <class T>
        inline size_t writeToFile(T& f, bool withHeaders = false)
        {
            if (!_currBuffer)
                return 0;
            size_t nToRet = 0;
            cnt::ListSL<char*>& lst = _bufferProvider->getBuffers();
            cnt::ListSL<char*>::const_iterator it(lst.begin());
            const char* pBuff = *it;
            td::UINT4 headerSize = _header->size();
            while (pBuff != _currBuffer)
            {
                if (withHeaders)
                {
                    f.write(pBuff, _bufferSize);
                    nToRet += _bufferSize;
                }
                else
                {
                    td::UINT4 d = _bufferSize - headerSize;
                    f.write(pBuff + headerSize, d);
                    nToRet += d;
                }
                    
                ++it;
                pBuff = *it;
            }
            writePayloadSize();
            if (_header->payloadSize > 0)
            {
                if (withHeaders)
                {
                    f.write(pBuff, headerSize + _header->payloadSize);
                    nToRet += headerSize + _header->payloadSize;
                }
                else
                {
                    f.write(pBuff + headerSize, _header->payloadSize);
                    nToRet += _header->payloadSize;
                }
            }
            return nToRet;
        }

		virtual void release()
		{
		}

	};	
}
