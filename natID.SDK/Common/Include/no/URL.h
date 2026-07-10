// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file URL.h
 * @brief URL encoding utility for percent-encoding arbitrary strings.
 */
//
//  URL.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <cnt/StringBuilder.h>

namespace no
{
	//charToHex
    /// @brief Percent-encodes a string so it is safe for use in a URL.
    ///
    /// Unreserved characters (alphanumerics and <tt>-_.!~*'()</tt>) are copied
    /// as-is.  Spaces are encoded as '+' when @p plusForSpace is true, or as
    /// @c %20 otherwise.  All other bytes are encoded as <tt>%XX</tt>.
    ///
    /// @tparam TBUILDER Type of the string builder (must support @c reset(),
    ///         @c <<, and @c getString()).
    /// @param s Input string to encode.
    /// @param build String builder used as an intermediate work buffer.
    /// @param plusForSpace If true, spaces are encoded as '+'; otherwise as @c %20.
    /// @param resetCnt If true, the builder is reset before encoding begins.
    /// @return The percent-encoded string.
	template <class TBUILDER>
	inline td::String encodeURL(const td::String& s, TBUILDER& build, bool plusForSpace = true, bool resetCnt = true)
	{
		if (resetCnt)
			build.reset();

		const char *pStr = s.c_str();
		size_t nLen = s.length();

		for (size_t i = 0; i<nLen; ++i)
		{
			char c = pStr[i];
			if ((c >= '0' && c <= '9') ||
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
				c == '*' || c == '\'' || c == '(' || c == ')')
			{
				build << c;
			}
			else if ( (c == ' ') && plusForSpace)
			{
				//v.push_back('+');
				build << '+';
			}
			else
			{
				//v.push_back('%');
				build << '%';
				unsigned char hex[2];
				td::charToHex(c, hex);
				build << (char)hex[0];
				build << (char)hex[1];
			}
		}

		td::String toRet;
		build.getString(toRet);
		return toRet;
	}
}
