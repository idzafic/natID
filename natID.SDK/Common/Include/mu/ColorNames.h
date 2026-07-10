// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ColorNames.h
    @brief Provides a bidirectional mapping between color names (strings) and their packed ARGB integer values. */
#pragma once
#include <td/String.h>
#include <mu/Utils.h>
#include <mu/mu.h>
#include <mu/Regionals.h>
#include <map>

namespace mu
{
	/// @brief Manages a bidirectional registry that maps color names to ARGB values and vice versa.
	class ColorNames
	{
		std::map<td::String, td::UINT4> mapNameToColor; ///< Maps a color name string to its packed ARGB value.
		std::map<td::UINT4, const td::String*> mapColorToName; ///< Maps a packed ARGB value back to its canonical name string.

	public:
		typedef std::map<td::String, td::UINT4>::const_iterator const_iterator; ///< Iterator type for name-to-color traversal.
		//typedef std::map<td::UINT4, const td::String*>::const_iterator iteratorColorToName;

		/// @brief Default constructor; initializes empty name-to-color and color-to-name maps.
		ColorNames()
			: mapNameToColor()
			, mapColorToName()
		{
		}

		/// @brief Returns a const iterator to the first element in the name-to-color map.
		/// @return Const iterator pointing to the beginning of the map.
		const_iterator begin() const
		{
			return mapNameToColor.begin();
		}

		/// @brief Returns a const iterator past the last element in the name-to-color map.
		/// @return Const iterator pointing past the end of the map.
		const_iterator end() const
		{
			return mapNameToColor.end();
		}

		/// @brief Registers a color name and its corresponding ARGB value in both maps.
		/// @param name The color name string (e.g., "red").
		/// @param val The packed ARGB color value.
		void add(const char* name, td::UINT4 val)
		{
			//typedef std::pair <td::String, td::UINT4> tEnumPair;
			std::pair <td::String, td::UINT4> toInsert(name, val);
			std::pair< std::map <td::String, td::UINT4>::iterator, bool> valRet = mapNameToColor.insert(toInsert);
			mapColorToName[val] = &(valRet.first->first);
		}

		/// @brief Looks up the ARGB value for a color name or hex string.
		/// @param strColorName The color name or a '#'-prefixed hex color string.
		/// @return The packed ARGB value, or 0xff000000 (opaque black) if not found.
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

		/// @brief Returns the registered name for a given ARGB color value.
		/// @param val The packed ARGB color value to look up.
		/// @return The color name string, or a formatted hex representation if not registered.
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
