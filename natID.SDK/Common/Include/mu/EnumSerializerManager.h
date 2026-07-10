// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file EnumSerializerManager.h
 *  @brief Manages a fixed-size collection of EnumSerializer instances indexed by position.
 */
#pragma once
#include <mu/EnumSerializer.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <typeinfo>

namespace mu
{

	/// @brief Manages a registry of EnumSerializer pointers, one per registered enum type.
	class EnumSerializerManager
	{
		/// @brief Internal entry holding a pointer to an EnumSerializer and optional debug type name.
		struct tEn
		{
			EnumSerializer* enumSerializer;		///< Pointer to the enum serializer instance.
	#ifdef _DEBUG
			td::String rawName;                 ///< Raw type name used for debug validation.
	#endif
		};
		cnt::SafeFullVector<struct tEn> enumerators; ///< Fixed-size vector of serializer entries.

	public:
		/// @brief Constructs the manager with a pre-allocated number of enum entries.
		/// @param nEntries Number of enum serializer slots to allocate.
		EnumSerializerManager(int nEntries)
			: enumerators(nEntries)
		{
		}

		/// @brief Registers an EnumSerializer at the given position for enum type TENUM.
		/// @tparam TENUM The enum type associated with the serializer.
		/// @param pos Zero-based index into the internal registry.
		/// @param enumSer Pointer to the EnumSerializer to store.
		template <typename TENUM>
		void setEnum(int pos, EnumSerializer* enumSer)
		{
			enumerators[pos].enumSerializer = enumSer;
	#ifdef _DEBUG
			enumerators[pos].rawName = typeid(TENUM).raw_name();
	#endif
		}

		/// @brief Retrieves the EnumSerializer registered at the given position for enum type TENUM.
		/// @tparam TENUM The enum type expected at this position (validated in debug builds).
		/// @param pos Zero-based index into the internal registry.
		/// @return Pointer to the registered EnumSerializer.
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
