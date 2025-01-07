// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/EnumSerializer.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <typeinfo>

namespace mu
{

	class EnumSerializerManager
	{
		struct tEn
		{
			EnumSerializer* enumSerializer;		
	#ifdef _DEBUG
			td::String rawName;
	#endif
		};
		cnt::SafeFullVector<struct tEn> enumerators;

	public:
		EnumSerializerManager(int nEntries)
			: enumerators(nEntries)
		{
		}

		template <typename TENUM>
		void setEnum(int pos, EnumSerializer* enumSer)
		{
			enumerators[pos].enumSerializer = enumSer;
	#ifdef _DEBUG
			enumerators[pos].rawName = typeid(TENUM).raw_name();
	#endif
		}

		template <typename TENUM>
		EnumSerializer* getEnum(int pos)
		{
#ifdef _DEBUG
			assert (enumerators[pos].rawName == td::String(typeid(TENUM).raw_name()));
#endif
			return enumerators[pos].enumSerializer;	
		}
	};

};