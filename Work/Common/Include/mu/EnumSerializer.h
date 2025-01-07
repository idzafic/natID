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
#include <map>
#include <sstream>

namespace mu
{
	class EnumSerializer
	{
		std::map<td::String, td::UINT4> mapNameToEnum;
		std::map<td::UINT4, const td::String*> mapEnumToName;
		
		void formatErrorName(const td::String& strName, td::String& strErr) const
		{
			std::ostringstream ss;
			ss << "Wrong enum name: \'" << strName.c_str() << "\'. Expected one of: [";
			std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
			bool first = true;
			while (it != mapEnumToName.end())
			{
				if (first)
					ss << it->second->c_str();
				else
					ss << ',' << it->second->c_str();
				++it;
				first = false;
			}
			ss << ']';
			ss << std::ends;

			strErr = ss.str().c_str();
		}

		void formatErrorValue(td::UINT4 val, td::String& strErr) const
		{
			std::ostringstream ss;
			ss << "Wrong enum value: \'" << val << "\'. Expected one of: [";
			std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();
			bool first = true;
			while (it != mapEnumToName.end())
			{
				if (first)
					ss << it->first;
				else
					ss << ',' << it->first;
				first = false;
				++it;
			}
			ss << ']';
			ss << std::ends;
			strErr = ss.str().c_str();
		}	

	public:
		EnumSerializer()
		{
		}

		void add(const char* name, td::UINT4 val)
		{
			//typedef std::pair <td::String, td::UINT4> tEnumPair;
			std::pair <td::String, td::UINT4> toInsert(name, val);
			std::pair< std::map <td::String, td::UINT4>::iterator, bool> valRet = mapNameToEnum.insert(toInsert);
			mapEnumToName[val] = &(valRet.first->first);
			//mapNameToEnum[name] = val;
			//mapEnumToName[val] = name;

		}
		
		td::UINT4 getIntValue(const td::String& strEnumName) const
		{
			std::map<td::String, td::UINT4>::const_iterator it = mapNameToEnum.find(strEnumName);
			if (it != mapNameToEnum.end())
			{
				return it->second;
			}

			td::String strErr;
			formatErrorName(strEnumName, strErr);
			throw strErr;
			return mapEnumToName.begin()->first;
		}

		template <typename T>
		T getValue(const td::String& strEnumName) const
		{
			std::map<td::String, td::UINT4>::const_iterator it = mapNameToEnum.find(strEnumName);
			if (it != mapNameToEnum.end())
			{
				return (T) it->second;
			}

			td::String strErr;
			formatErrorName(strEnumName, strErr);
			throw strErr;
			return (T) mapEnumToName.begin()->first;
		}
		
		const td::String& getName(td::UINT4 val) const
		{
			std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.find(val);
			if (it != mapEnumToName.end())
			{
				return *(it->second);
			}

			td::String strErr;
			formatErrorValue(val, strErr);
			throw strErr;
		}

		template<typename T>
		void show(std::ostream& ss) const
		{
			bool first = true;
			std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();		
			while (it != mapEnumToName.end())
			{
				if (!first)
					ss << ", ";
				else 
					first = false;

				ss << it->second->c_str();		
				++it;
			}		
			ss << std::endl;
		}

		template<typename T>
		void showDetailed(std::ostream& ss) const
		{
			ss << "Enum: " << typeid(T).name() << std::endl;

			std::map<td::UINT4, const td::String*>::const_iterator it = mapEnumToName.begin();		
			while (it != mapEnumToName.end())
			{
				ss << it->second->c_str() << "=" << it->first << ", ";		
				++it;
			}		
			ss << std::endl;
		}
	};
};