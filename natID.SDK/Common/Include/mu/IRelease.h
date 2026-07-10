// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IRelease.h
 *  @brief Interface for objects that support explicit resource release via a release() method.
 */
#pragma once

namespace mu
{
	/// @brief Abstract interface for objects that require explicit release of their resources.
	class IRelease
	{
	public:
		/// @brief Releases resources held by this object; the object must not be used after calling this.
		virtual void release() = 0;
	};
}
