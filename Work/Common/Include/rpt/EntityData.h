// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <rpt/AutoElement.h>
#include <td/String.h>
#include <td/Variant.h>

namespace rpt
{
	//indexFrom and indexTo are negative values
	template <typename T, int indexFrom, int indexTo>
	class EntityData
	{
	protected:
		T _data[indexFrom - indexTo];
		T _empty;
		T _assignEmpty;
	public:
		EntityData(){}
		EntityData(const EntityData& d)
		{
			operator = (d);
		}
		void operator =(const EntityData& d)
		{
			int nElems = indexFrom - indexTo;
			for (int i = 0; i < nElems; ++i)
				_data[i] = d._data[i];
		}

		const T& operator [] (int i) const
		{
			//assert( (i <= indexFrom) && (i > indexTo) );
			//int iFrom = indexFrom;
			//int iTo = indexTo;
			if (i > indexFrom || i <= indexTo)
				return _empty;
			return _data[indexFrom - i];
		}

		T& operator [] (int i)
		{
			//assert(i <= indexFrom && i > indexTo);
			if (i > indexFrom || i <= indexTo)
				return _assignEmpty;
			return _data[indexFrom - i];
		}
	};

	typedef rpt::EntityData<td::Variant, RPT_TXT_DOCUMENT_TYPE, RPT_DOCUMENT_LAST> DocumentData;
	typedef rpt::EntityData<td::String, RPT_TXT_COMPANY1, RPT_COMPANY_LAST> CompanyData;
	typedef rpt::EntityData<td::String, RPT_TXT_OBJ_TYPE, RPT_OBJ_LAST> ObjectData;
    typedef rpt::EntityData<td::String, RPT_TXT_SINATURE1, RPT_SIGNATURE_LAST> SignatureData;
}
