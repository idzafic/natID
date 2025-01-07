// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "mu/EnumSerializer.h"
#include <typeinfo>
#include "td/Types.h"
#include "mu/EnumMacro.h"

namespace td
{
	template <typename T>
	inline void buildEnumSerializer(mu::EnumSerializer& ser)
	{
		if (typeid(T) == typeid(LineStyle))
		{			
			ENUMMACRO(ser, SOLID);
			ENUMMACRO(ser, DASH);
			ENUMMACRO(ser, DOTDASH);
			ENUMMACRO(ser, DOT);
			return;
		}
		
		if (typeid(T) == typeid(DataType))
		{
			ENUMMACRO(ser, boolean);
			ENUMMACRO(ser, byte);
			ENUMMACRO(ser, word);
			ENUMMACRO(ser, int2);
			ENUMMACRO(ser, uint4);
			ENUMMACRO(ser, int4);
			ENUMMACRO(ser, lint8);
			ENUMMACRO(ser, luint8);
			ENUMMACRO(ser, real4);
			ENUMMACRO(ser, real8);
			ENUMMACRO(ser, string8);
			ENUMMACRO(ser, string16);
			ENUMMACRO(ser, string32);
			ENUMMACRO(ser, enumerator);
			ENUMMACRO(ser, date);
			ENUMMACRO(ser, time);
			ENUMMACRO(ser, dateTime);
			ENUMMACRO(ser, decimal0);
			ENUMMACRO(ser, decimal1);
			ENUMMACRO(ser, decimal2);
			ENUMMACRO(ser, decimal3);
			ENUMMACRO(ser, decimal4);
			ENUMMACRO(ser, ch);
			ENUMMACRO(ser, nch);
			ENUMMACRO(ser, chFix);
			ENUMMACRO(ser, nchFix);
			ENUMMACRO(ser, dbDate);
			ENUMMACRO(ser, dbTime);
			ENUMMACRO(ser, dbDateTime);
			ENUMMACRO(ser, dbDecimal);
			ENUMMACRO(ser, binary);
			ENUMMACRO(ser, guid);
			ENUMMACRO(ser, color);
			ENUMMACRO(ser, smallDecimal0);
			ENUMMACRO(ser, smallDecimal1);
			ENUMMACRO(ser, smallDecimal2);
			ENUMMACRO(ser, smallDecimal3);
			ENUMMACRO(ser, smallDecimal4);		
			return;
		}

		throw td::String("Unknown enum type");
	}
};