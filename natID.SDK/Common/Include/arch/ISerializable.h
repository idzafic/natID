// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISerializable.h
    @brief Abstract interface for objects that can be serialized to and from binary archives. */
#pragma once
#include "td/Types.h"

namespace arch
{

	class ArchiveOut;
	class ArchiveIn;

	/// @brief Abstract interface that must be implemented by any class that supports binary archive serialization.
	class ISerializable
	{
	public:
		/// @brief Serializes this object to a binary output archive.
		/// @param ar The output archive to write to.
		virtual void serialize(ArchiveOut& ar) const = 0;

		/// @brief Deserializes this object from a binary input archive.
		/// @param ar The input archive to read from.
		virtual void serialize(ArchiveIn& ar) = 0;

		/// @brief Returns the guard byte used to validate type identity during guarded deserialization.
		/// @return A byte identifying this serializable type.
		virtual td::BYTE getGuardedByte() const = 0;
	};
};
