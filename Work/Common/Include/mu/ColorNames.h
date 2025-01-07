// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <mu/Utils.h>
#include <mu/mu.h>
#include <mu/Regionals.h>
#include <map>

namespace mu
{
	class ColorNames
	{		
		std::map<td::String, td::UINT4> mapNameToColor;
		std::map<td::UINT4, const td::String*> mapColorToName;

	public:
		typedef std::map<td::String, td::UINT4>::const_iterator const_iterator;
		//typedef std::map<td::UINT4, const td::String*>::const_iterator iteratorColorToName;
		ColorNames()
			: mapNameToColor()
			, mapColorToName()
		{
		}

		const_iterator begin() const
		{
			return mapNameToColor.begin();
		}

		const_iterator end() const
		{
			return mapNameToColor.end();
		}

		void add(const char* name, td::UINT4 val)
		{
			//typedef std::pair <td::String, td::UINT4> tEnumPair;
			std::pair <td::String, td::UINT4> toInsert(name, val);
			std::pair< std::map <td::String, td::UINT4>::iterator, bool> valRet = mapNameToColor.insert(toInsert);
			mapColorToName[val] = &(valRet.first->first);
		}

		td::UINT4 getValue(const td::String& strColorName) const
		{
			if (strColorName.length() == 0)
				return 0xff000000;

			if (strColorName.getAt(0) == '#')
			{
				return mu::Utils::toColor(strColorName.c_str());
			}
			else
			{
				std::map<td::String, td::UINT4>::const_iterator it(mapNameToColor.find(strColorName));
				if (it != mapNameToColor.end())
				{
					return it->second;
				}
			}		

			return 0xff000000;
		}		

		const char* getName(td::UINT4 val)
		{			
			std::map<td::UINT4, const td::String*>::const_iterator it = mapColorToName.find(val);
			if (it == mapColorToName.end())
			{				
				return mu::getMainThreadRegionals()->formatColor(val);
			}
			else
			{
				return it->second->c_str();
			}
		}
	};
};
