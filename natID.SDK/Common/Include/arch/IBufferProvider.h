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
#include <cnt/List.h>

namespace arch
{
	class IBufferProvider
	{
	public:				
		virtual char* get(td::UINT4& buffSize) = 0;	
		virtual void completed(const char* buff, td::UINT4 headerSize, td::UINT4 payloadSize, bool bError) = 0;
		virtual void reset() = 0;		
		virtual void setReadMode(bool bRead = true) = 0;
		virtual cnt::ListSL< char* >& getBuffers() = 0;
	};
};
