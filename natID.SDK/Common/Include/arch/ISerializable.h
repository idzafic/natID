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

	class ArchiveOut;
	class ArchiveIn;

	class ISerializable
	{
	public:
		virtual void serialize(ArchiveOut& ar) const = 0;	
		virtual void serialize(ArchiveIn& ar) = 0;
		virtual td::BYTE getGuardedByte() const = 0;
	};
};
