// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BaseDateTime.h
    @brief Defines the BaseDate and BaseTime structures used as low-level date and time representations. */
#pragma once
#include <td/Types.h>

namespace td
{
	/// @brief Plain-old-data structure representing a calendar date.
	typedef struct _BaseDate
	{
		td::UINT2 year;  ///< Calendar year (e.g., 2024).
		td::BYTE month;  ///< Month of the year, in the range 1-12.
		td::BYTE day;    ///< Day of the month, in the range 1-31.
		td::BYTE BC;     ///< Flag indicating a Before Common Era (BC) year; non-zero means BC.
		td::BYTE utc;    ///< UTC offset or flag indicating whether the date is expressed in UTC.
	} BaseDate;

	/// @brief Plain-old-data structure representing a time of day with microsecond precision.
	typedef struct _BaseTime
	{
		td::BYTE hour;    ///< Hour component of the time, in the range 0-23.
		td::BYTE min;     ///< Minute component of the time, in the range 0-59.
		td::BYTE sec;     ///< Second component of the time, in the range 0-59.
		td::UINT4 usec;   ///< Microsecond component of the time, in the range 0-999999.
	} BaseTime;
}


