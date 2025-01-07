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

namespace td
{
	typedef struct _BaseDate
	{
		td::UINT2 year;
		td::BYTE month;
		td::BYTE day;
		td::BYTE BC;
		td::BYTE utc;
	} BaseDate;

	typedef struct _BaseTime
	{
		td::BYTE hour;
		td::BYTE min;
		td::BYTE sec;
		td::UINT4 usec;
	} BaseTime;
}


