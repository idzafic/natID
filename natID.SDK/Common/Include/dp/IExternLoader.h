// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IExternLoader.h
    @brief Interface for populating a data set from an external data source. */
#pragma once

namespace dp
{
	class IDataSet;

	/// @brief Abstract interface for loading data from an external source into an IDataSet.
	class IExternLoader
	{
	public:
		//should return true if rowa <= rowb
		/// @brief Populates the given data set with data from the external source.
		/// @param pRS Pointer to the target data set to be populated.
		/// @return True if the data set was successfully populated.
		virtual bool populateDP(dp::IDataSet* pRS) const = 0;
	};
}
