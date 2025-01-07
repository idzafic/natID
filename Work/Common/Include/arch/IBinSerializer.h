// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "td/Types.h"

namespace arch
{
	class IBinSerializer
	{
	public:
		virtual bool open(const char* serializerName) = 0;
		virtual void write(const char* bytes, td::UINT4 nBytes) = 0;
		virtual void read(char* bytes, td::UINT4 nBytes) = 0;
		virtual void close() = 0;
		virtual void release() = 0;
		virtual bool goTo(td::LUINT8 pos) = 0;
		//virtual void setCompression(char compressionLevel) = 0;
		//virtual td::UINT4 getHeaderSize() const = 0;
		//virtual void setPayloadSize(td::UINT4 payloadSize) = 0;
		//virtual void markAsLastPayload() = 0;
		//virtual void setMask(td::UINT4 mask) = 0;
		virtual ~IBinSerializer(){};		
	};
};
