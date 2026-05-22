// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "MemorySerializer.h"
#include "ArchiveIn.h"

namespace arch
{
	class MemoryIn : public ArchiveIn
	{
	protected:
		MemorySerializer<MemoryIn, false> _fIn;

	public:
		
		MemoryIn(IBufferProvider* pBufferProvier) 
			: ArchiveIn(_fIn) 			
		{ 		
			_fIn.setBufferProvider(pBufferProvier, this);
			if (pBufferProvier)
				pBufferProvier->setReadMode(true);
			_fIn.open(nullptr);
		}

		Header* encodeHeader2() const
		{
			assert(false);
			return nullptr;
		}

		Header* decodeHeader2(char* pHeader)
		{
			if (!decodeHeader(pHeader))
				return nullptr;
			return &_header;
		}

		Header& getHeader()
		{
			return _header;
		}
		void setPayloadLen(td::UINT4 payloadLen)
		{
			_header.payloadSize = payloadLen;
		}

		bool open(const char*)
		{
			_fIn.open(nullptr);
			return true;
		}

		template <class T>
		inline size_t readFromFile(T& f, size_t nBytes = 0) //0 means whole content from the current location
		{
			return _fIn.readFromFile(f, nBytes);
		}

		//template <class T>
		//inline size_t writeToFile(T& f, bool withHeaders = false)
		//{
		//	return _fOut.writeToFile(f, withHeaders);
		//}

	};
}
