// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file EntityData.h
    @brief Template class for indexed entity data storage with negative index ranges. */
#pragma once
#include <rpt/AutoElement.h>
#include <td/String.h>
#include <td/Variant.h>

namespace rpt
{
	//indexFrom and indexTo are negative values
	/// @brief Template container that stores data elements accessible via a negative integer index range.
	/// @tparam T Element type stored in the container.
	/// @tparam indexFrom Starting (higher) index boundary (negative value).
	/// @tparam indexTo Ending (lower) index boundary (negative value).
	template <typename T, int indexFrom, int indexTo>
	class EntityData
	{
	protected:
		T _data[indexFrom - indexTo]; ///< Internal array holding the data elements.
		T _empty; ///< Default empty value returned for out-of-range const access.
		T _assignEmpty; ///< Default empty value returned for out-of-range non-const access.
	public:
		/// @brief Default constructor.
		EntityData(){}
		/// @brief Copy constructor.
		/// @param d Source EntityData instance to copy from.
		EntityData(const EntityData& d)
		{
			operator = (d);
		}
		/// @brief Copy assignment operator.
		/// @param d Source EntityData instance to copy from.
		void operator =(const EntityData& d)
		{
			int nElems = indexFrom - indexTo;
			for (int i = 0; i < nElems; ++i)
				_data[i] = d._data[i];
		}

		/// @brief Const element access by index.
		/// @param i Index of the element (must be in range (indexTo, indexFrom]).
		/// @return Const reference to the element, or _empty if index is out of range.
		const T& operator [] (int i) const
		{
			//assert( (i <= indexFrom) && (i > indexTo) );
			//int iFrom = indexFrom;
			//int iTo = indexTo;
			if (i > indexFrom || i <= indexTo)
				return _empty;
			return _data[indexFrom - i];
		}

		/// @brief Non-const element access by index.
		/// @param i Index of the element (must be in range (indexTo, indexFrom]).
		/// @return Reference to the element, or _assignEmpty if index is out of range.
		T& operator [] (int i)
		{
			//assert(i <= indexFrom && i > indexTo);
			if (i > indexFrom || i <= indexTo)
				return _assignEmpty;
			return _data[indexFrom - i];
		}
	};

	typedef rpt::EntityData<td::Variant, RPT_TXT_DOCUMENT_TYPE, RPT_DOCUMENT_LAST> DocumentData; ///< Variant-typed entity data for document-level fields.
	typedef rpt::EntityData<td::String, RPT_TXT_COMPANY1, RPT_COMPANY_LAST> CompanyData; ///< String-typed entity data for company-level fields.
	typedef rpt::EntityData<td::String, RPT_TXT_OBJ_TYPE, RPT_OBJ_LAST> ObjectData; ///< String-typed entity data for object-level fields.
    typedef rpt::EntityData<td::String, RPT_TXT_SINATURE1, RPT_SIGNATURE_LAST> SignatureData; ///< String-typed entity data for signature fields.
}
