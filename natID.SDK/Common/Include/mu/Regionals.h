// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Regionals.h
    @brief Locale-aware formatter for dates, times, numbers, and floating-point values. */
#pragma once
#include <mu/muLib.h>
#include <sstream>
#include <string>
#include <cnt/PushBackVector.h>
#include <cmath>
#include <cstdlib>
#include <td/DateTime.h>
#include <td/Decimal.h>
#include <td/PascalString.h>
#include <mu/UTFConverter.h>
#include <typeinfo>
#include <cstdio>
#include <cnt/Array.h>
#include <mu/Hash.h>
#include <math/math.h>
#include <td/Conv.h>
#include <td/Color.h>
#include <mu/mu.h>

namespace td
{
class Variant;
}

namespace mu
{
	class Regionals;

/// @brief Formats a td::Variant value into the Regionals format buffer.
/// @param pReg       Pointer to the Regionals instance that owns the format buffer.
/// @param val        Variant value to format.
/// @param nDec       Number of decimal places for floating-point values.
/// @param showThSep  If true, thousand separators are inserted in numeric output.
/// @param fmt        Floating-point format style (Decimal or Scientific).
/// @param trailingTxt Optional text appended after the formatted value.
/// @return Number of characters written, or a negative value on error.
MAINUTILS_API int formatVariant(Regionals* pReg, const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr);

	const char _h = 'h'; ///< Format character representing a 12-hour clock hour.
	const char _H = 'H'; ///< Format character representing a 24-hour clock hour.
	const char _s = 's'; ///< Format character representing seconds (lowercase).
	const char _S = 'S'; ///< Format character representing seconds (uppercase alias).

	const char _t = 't'; ///< Format character representing AM/PM designator.
	const char _u = 'u'; ///< Format character representing milliseconds.

	const char _d = 'd'; ///< Format character representing day (lowercase).
	const char _D = 'D'; ///< Format character representing day (uppercase alias).

	const char _m = 'm'; ///< Format character representing month or minute (lowercase).
	const char _M = 'M'; ///< Format character representing month or minute (uppercase alias).

	const char _y = 'y'; ///< Format character representing year (lowercase).
	const char _Y = 'Y'; ///< Format character representing year (uppercase alias).
	const char _quot = '\''; ///< Quote character used to delimit literal text in format strings.

	const int BUFF_OFFSET = 32; ///< Byte offset in formatBuffer reserved for scratch work during formatting.

	/// @brief Provides locale-aware formatting of dates, times, integers, floats, and decimals.
	class Regionals
	{
	protected:
		/// @brief Identifies the semantic role of a date part within a format pattern.
		typedef enum {Year= 0, YearShort = 1, Month = 2, MonthName = 3, MonthShortName = 4, Day = 5, DayName = 6, DayShortName = 7, DateSeparator = 8} tDatePart;
		/// @brief Identifies the semantic role of a time part within a format pattern.
		typedef enum {Hour12= 0, Hour24 = 1, Minute = 2, Second = 3, Milisecond = 4, AMPM = 5, TimeSeparator = 6} tTimePart;

	protected:
        td::PascalString<1>    locName;          ///< Name of the locale (e.g. "en-US").
		int _maxLen;                             ///< Maximum usable length of formatBuffer.
		td::UINT4 _locID;                        ///< Hash-based numeric identifier for the locale name.
        float maxDblFormat;                      ///< Upper threshold above which doubles are shown as stars.
        float minDblFormat;                      ///< Lower threshold below which doubles are treated as zero.
        float maxFltFormat;                      ///< Upper threshold above which floats are shown as stars.
        float minFltFormat;                      ///< Lower threshold below which floats are treated as zero.
        int _fmtBuffLen;                         ///< Effective length of the format buffer excluding the offset.
        char decPoint;                           ///< Decimal separator character (e.g. '.' or ',').
        char thousandSeparator;                  ///< Thousand-grouping separator character (e.g. ',' or '.').
        char dateInputSeparator;                 ///< Separator character used in input date strings.
        char dateInputFormat[3];                 ///< Three-character code describing the input date component order (e.g. "dmy").

		/// @brief Descriptor for one part of a date format pattern.
		typedef struct _tDateFmt
		{
			tDatePart part;                ///< Which date component this entry represents.
			td::PascalString<0> format;    ///< Printf-style format string used to render this part.
		} tDatePartFmt;

		/// @brief Descriptor for one part of a time format pattern.
		typedef struct _tTimeFmt
		{
			tTimePart part;                ///< Which time component this entry represents.
			td::PascalString<0> format;    ///< Printf-style format string used to render this part.
		} tTimePartFmt;


	public:
		char* formatBuffer; ///< Heap-allocated output buffer used by all format methods.
	protected:
		td::PascalString<1>	bc; ///< String displayed after a date to indicate Before Christ (e.g. "BC").
		td::PascalString<1>	am; ///< String used for the AM period designator.
		td::PascalString<1>	pm; ///< String used for the PM period designator.

		cnt::PushBackVector<tDatePartFmt>	dateFormatWinLong;       ///< Parsed Windows long date format parts.
		cnt::PushBackVector<tDatePartFmt>	dateFormatWinShort;      ///< Parsed Windows short date format parts.
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortY4;    ///< Own short date format parts with 4-digit year.
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortY2;    ///< Own short date format parts with 2-digit year.
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortDM;    ///< Own short date format parts showing day and month only.

		cnt::PushBackVector<tTimePartFmt>	timeFormatWinLong;           ///< Parsed Windows long time format parts.
		cnt::PushBackVector<tTimePartFmt>	timeFormatWinShort;          ///< Parsed Windows short time format parts.
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMMSSu;    ///< Own time format parts: H:MM:SS.uuu.
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMMSS;     ///< Own time format parts: H:MM:SS.
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMM;       ///< Own time format parts: H:MM.

		cnt::Array< td::PascalString<1>, 12>	monthNames;       ///< Full month names indexed 0–11.
		cnt::Array< td::PascalString<1>, 12>	monthShortNames;  ///< Abbreviated month names indexed 0–11.

		cnt::Array< td::PascalString<1>, 7>		dayNames;         ///< Full weekday names indexed 0–6.
		cnt::Array< td::PascalString<1>, 7>		dayShortNames;    ///< Abbreviated weekday names indexed 0–6.
	public:

	/// @brief Constructs a Regionals instance with the C locale and a custom format buffer size.
	/// @param buffLen Total size in bytes of the internal format buffer; must be greater than 60.
	Regionals(int buffLen)
		: locName("C")
        , _maxLen(buffLen - BUFF_OFFSET-1)
		, decPoint('.')
		, thousandSeparator(',')
		, maxDblFormat(1e+15f)
		, minDblFormat(1e-15f)
		, maxFltFormat(1e+7f)
		, minFltFormat(1e-7f)
	    ,_fmtBuffLen(buffLen - BUFF_OFFSET-1)
	    , formatBuffer(nullptr)
        , dateInputSeparator('/')
	{
        dateInputFormat[0] = 'y';
        dateInputFormat[1] = 'm';
        dateInputFormat[2] = 'd';
		formatBuffer = new char[buffLen];
		bc = "BC";
		am = "AM";
		pm = "PM";
		assert (buffLen > 60);
		dateFormatWinLong.reserve(16);
		dateFormatWinShort.reserve(16);
		dateFormatOwnShortY4.reserve(10);
		dateFormatOwnShortY2.reserve(10);
		dateFormatOwnShortDM.reserve(8);

		timeFormatWinLong.reserve(16);
		timeFormatWinShort.reserve(16);
		timeFormatOwnShortHMMSSu.reserve(10);
		timeFormatOwnShortHMMSS.reserve(8);
		timeFormatOwnShortHMM.reserve(8);
	}

	/// @brief Returns the decimal separator character for this locale.
	/// @return The decimal point character.
	char getDecPoint() const
	{
		return decPoint;
	}

	/// @brief Returns the thousand separator character for this locale.
	/// @return The thousand separator character.
	char getThousandSeparator() const
	{
		return thousandSeparator;
	}

	/// @brief Converts a locale-formatted numeric string to a double, replacing the locale decimal point and stripping thousand separators.
	/// @param pVal Null-terminated string containing the locale-formatted number, or nullptr.
	/// @return Parsed double value, or 0 on failure.
	double toDouble(const char* pVal)
	{
		if (!pVal)
			return 0;

		int i = 0;
		int iUsedCh = 0;
		char ch = pVal[0];
		while (ch != 0)
		{
			if (ch == decPoint)
			{
				formatBuffer[iUsedCh++] = '.';
			}
			else if (ch != thousandSeparator)
			{
				formatBuffer[iUsedCh++] = ch;
			}
			ch = pVal[++i];

			if (iUsedCh >= 32)
				return 0;
		}
		formatBuffer[iUsedCh] = 0;
        return mu::toDouble(formatBuffer);
	}

	/// @brief Updates all locale format tables from a locale information object.
	/// @tparam TLI Locale info type exposing getAM(), getPM(), getBC(), getDecimalPoint(),
	///             getThousandSeparator(), date/time format strings, and month/day name accessors.
	/// @param li  Locale info object providing the new settings.
	template <class TLI>
	void update(const TLI& li)
	{
		dateFormatWinLong.clean();
		dateFormatWinShort.clean();
		dateFormatOwnShortY4.clean();
		dateFormatOwnShortY2.clean();
		dateFormatOwnShortDM.clean();

		timeFormatWinLong.clean();
		timeFormatWinShort.clean();
		timeFormatOwnShortHMMSSu.clean();
		timeFormatOwnShortHMMSS.clean();
		timeFormatOwnShortHMM.clean();

		am = li.getAM();
		pm = li.getPM();
		bc = li.getBC();

		update(li.getLocaleName(), li.getDecimalPoint(), li.getThousandSeparator(), li.getDateShortFmt(), li.getDateLongFmt(), li.getDateSep(), li.getTimeShortFmt(), li.getTimeLongFmt(), li.getTimeSep());
		for (int i = 0; i < 12; ++i)
		{
			monthNames[i] = li.getLongMonthName(i);
			monthShortNames[i] = li.getShortMonthName(i);
		}

		for (int i = 0; i < 7; ++i)
		{
			dayNames[i] = li.getLongDayName(i);
			dayShortNames[i] = li.getShortDayName(i);
		}
	}

	/// @brief Updates locale settings from individual parameters and rebuilds all format tables.
	/// @param nameOfLocale     Null-terminated locale name string.
	/// @param dec              Decimal separator character.
	/// @param thousand         Thousand separator character.
	/// @param pDateShortFormat Short date format pattern string, or nullptr to skip.
	/// @param pDateLongFormat  Long date format pattern string, or nullptr to skip.
	/// @param dateSeparator    Date component separator string used in own date formats.
	/// @param pTimeShortFormat Short time format pattern string, or nullptr to skip.
	/// @param pTimeLongFormat  Long time format pattern string, or nullptr to skip.
	/// @param timeSeparator    Time component separator string used in own time formats.
	/// @param dateInSeparator  Character separating components in user-input date strings.
	/// @param inputDateFormat  Three-character string specifying input date order (e.g. "dmy").
	void update(const char* nameOfLocale, char dec, char thousand, const char* pDateShortFormat = 0, const char* pDateLongFormat = 0, const char* dateSeparator = ".", const char* pTimeShortFormat = 0, const char* pTimeLongFormat = 0, const char* timeSeparator = ":", char dateInSeparator = '.', const char* inputDateFormat = "dmy")
	{
        dateInputSeparator = dateInSeparator;
        dateInputFormat[0] = inputDateFormat[0];
        dateInputFormat[1] = inputDateFormat[1];
        dateInputFormat[2] = inputDateFormat[2];

		locName = nameOfLocale;
		_locID = mu::calcHashNo(nameOfLocale);
		decPoint = dec;
		thousandSeparator = thousand;

		if (pDateShortFormat)
			prepareDateFormat(td::Date::Format::WinLong, pDateLongFormat);

		if (pDateLongFormat)
			prepareDateFormat(td::Date::Format::WinShort, pDateShortFormat);

		if (pDateShortFormat)
		{
			prepareOwnDateFormats(td::Date::Format::ShortY2, dateSeparator);
			prepareOwnDateFormats(td::Date::Format::ShortY4, dateSeparator);
			prepareOwnDateFormats(td::Date::Format::ShortDM, dateSeparator);
		}

		if (pTimeShortFormat)
			prepareTimeFormat(td::Time::Format::WinLong, pTimeLongFormat);

		if (pTimeLongFormat)
			prepareTimeFormat(td::Time::Format::WinShort, pTimeShortFormat);

		if (pTimeShortFormat)
		{
			prepareOwnTimeFormats(td::Time::Format::ShortHMM, timeSeparator);
			prepareOwnTimeFormats(td::Time::Format::ShortHMMSS, timeSeparator);
			prepareOwnTimeFormats(td::Time::Format::ShortHMMSSu, timeSeparator);
		}
	}

	/// @brief Returns the name of the current locale.
	/// @return Null-terminated locale name string.
	const char* getLocName() const
	{
		return locName.c_str();
	}

	/// @brief Constructs a fully initialised Regionals instance for the specified locale.
	/// @param nameOfLocale     Null-terminated locale name string.
	/// @param buffLen          Total size in bytes of the internal format buffer; must be greater than 60.
	/// @param dec              Decimal separator character.
	/// @param thousand         Thousand separator character.
	/// @param pDateShortFormat Short date format pattern string, or nullptr to skip.
	/// @param pDateLongFormat  Long date format pattern string, or nullptr to skip.
	/// @param dateSeparator    Date component separator string used in own date formats.
	/// @param pTimeShortFormat Short time format pattern string, or nullptr to skip.
	/// @param pTimeLongFormat  Long time format pattern string, or nullptr to skip.
	/// @param timeSeparator    Time component separator string used in own time formats.
	/// @param dateInSeparator  Character separating components in user-input date strings.
	/// @param inputDateFormat  Three-character string specifying input date order (e.g. "dmy").
	Regionals(const char* nameOfLocale, int buffLen, char dec, char thousand, const char* pDateShortFormat = 0, const char* pDateLongFormat = 0, const char* dateSeparator = ".", const char* pTimeShortFormat = 0, const char* pTimeLongFormat = 0, const char* timeSeparator = ":", char dateInSeparator = '.', const char* inputDateFormat = "dmy")
		: _maxLen(buffLen - BUFF_OFFSET-1)
			, locName(nameOfLocale)
			, decPoint(dec)
			, thousandSeparator(thousand)
			, maxDblFormat(1e+15f)
			, minDblFormat(1e-15f)
			, maxFltFormat(1e+7f)
			, minFltFormat(1e-7f)
	        , _fmtBuffLen(buffLen - BUFF_OFFSET-1)
			, formatBuffer(new char[buffLen])
            , dateInputSeparator(dateInSeparator)
		{
            dateInputFormat[0] = inputDateFormat[0];
            dateInputFormat[1] = inputDateFormat[1];
            dateInputFormat[2] = inputDateFormat[2];

			assert (buffLen > 60);

			_locID = mu::calcHashNo(nameOfLocale);

			bc = "BC";
			am = "AM";
			pm = "PM";

			dateFormatWinLong.reserve(16);
			dateFormatWinShort.reserve(16);
			dateFormatOwnShortY4.reserve(10);
			dateFormatOwnShortY2.reserve(10);
			dateFormatOwnShortDM.reserve(8);

			timeFormatWinLong.reserve(16);
			timeFormatWinShort.reserve(16);
			timeFormatOwnShortHMMSSu.reserve(10);
			timeFormatOwnShortHMMSS.reserve(8);
			timeFormatOwnShortHMM.reserve(8);

			if (pDateLongFormat)
				prepareDateFormat(td::Date::Format::WinLong, pDateLongFormat);

			if (pDateShortFormat)
				prepareDateFormat(td::Date::Format::WinShort, pDateShortFormat);

			if (pDateShortFormat)
			{
				prepareOwnDateFormats(td::Date::Format::ShortY2, dateSeparator);
				prepareOwnDateFormats(td::Date::Format::ShortY4, dateSeparator);
				prepareOwnDateFormats(td::Date::Format::ShortDM, dateSeparator);
			}

			if (pTimeShortFormat)
				prepareTimeFormat(td::Time::Format::WinShort, pTimeShortFormat);

			if (pTimeLongFormat)
				prepareTimeFormat(td::Time::Format::WinLong, pTimeLongFormat);


			if (pTimeShortFormat)
			{
				prepareOwnTimeFormats(td::Time::Format::ShortHMM, timeSeparator);
				prepareOwnTimeFormats(td::Time::Format::ShortHMMSS, timeSeparator);
				prepareOwnTimeFormats(td::Time::Format::ShortHMMSSu, timeSeparator);
			}
		}

		/// @brief Destructor; releases the format buffer.
		~Regionals()
		{
			delete [] formatBuffer;
		}

		/// @brief Returns the numeric identifier of the current locale.
		/// @return 32-bit hash-based locale ID.
		inline td::UINT4 getID() const
		{
			return _locID;
		}

	protected:

		/// @brief Builds the own time format part list for the given format type using the parsed Windows short format.
		/// @param formatType  Target own time format (ShortHMM, ShortHMMSS, or ShortHMMSSu).
		/// @param separator   Time component separator string.
		/// @param msSeparator Separator string placed before the milliseconds component (defaults to ".").
		void prepareOwnTimeFormats(td::Time::Format formatType, const char* separator, const char* msSeparator = ".")
		{
			int nMaxParts = 2;
			cnt::PushBackVector<tTimePartFmt>* pList = 0;

			unsigned int requiredFlag = 0;

			switch(formatType)
			{
			case td::Time::Format::ShortHMM:
				pList = &timeFormatOwnShortHMM;
				requiredFlag = 2 + 1;
				break;
			case td::Time::Format::ShortHMMSS:
				pList = &timeFormatOwnShortHMMSS;
				nMaxParts = 3;	//na kraju ne dolazi separator
				requiredFlag = 4 + 2 + 1;
				break;
			case td::Time::Format::ShortHMMSSu:
				nMaxParts = 3;	//tacka ce biti dodana kasnije
				pList = &timeFormatOwnShortHMMSSu;
				requiredFlag = 8 + 4 + 2 + 1;
				break;
			default:
				assert(0);
			}

			int iPos = 0;
			unsigned int flag = 0;
			cnt::PushBackVector<tTimePartFmt>::const_iterator it (timeFormatWinShort.begin());
			cnt::PushBackVector<tTimePartFmt>::const_iterator itEnd(timeFormatWinShort.end());
			while (it != itEnd)
			{
				switch(it->part)
				{
				case Hour12:
				case Hour24:
					{
						if (!(requiredFlag & 1))
							break;

						if (flag & 1)
							break;

						flag |= 1;
						++iPos;
						tTimePartFmt fmt;
						fmt.part = Hour24;
						fmt.format = it->format;
						pList->push_back(fmt);

						if (iPos < nMaxParts)
						{
							fmt.format = separator;
							fmt.part = TimeSeparator;
							pList->push_back(fmt);
						}
					}
					break;
				case Minute:
					{
						if (!(requiredFlag & 2))
							break;

						if (flag & 2)
							break;

						flag |= 2;
						++iPos;
						tTimePartFmt fmt;
						fmt.part = Minute;
						fmt.format = "%02d";
						pList->push_back(fmt);
						if (iPos < nMaxParts)
						{
							fmt.format = separator;
							fmt.part = TimeSeparator;
							pList->push_back(fmt);
						}
					}
					break;
				case Second:
					{
						if (!(requiredFlag & 4))
							break;

						if (flag & 4)
							break;

						flag |= 4;
						++iPos;
						tTimePartFmt fmt;
						fmt.part = Second;
						fmt.format = "%02d";
						pList->push_back(fmt);
						if (iPos < nMaxParts)
						{
							fmt.format = msSeparator;
							fmt.part = TimeSeparator;
							pList->push_back(fmt);
						}
					}
					break;
				default:
					break;
				}
				++it;
			}

			if (requiredFlag & 8)
			{
				tTimePartFmt fmt;
				fmt.format = msSeparator;
				fmt.part = TimeSeparator;
				pList->push_back(fmt);

				fmt.part = Milisecond;
				fmt.format = "%03d";
				pList->push_back(fmt);
			}
		}

		/// @brief Builds the own date format part list for the given format type using the parsed Windows short format.
		/// @param formatType Target own date format (ShortY4, ShortY2, or ShortDM).
		/// @param separator  Date component separator string.
		void prepareOwnDateFormats(td::Date::Format formatType, const char* separator)
		{
			bool yearShort = false;
			bool ignoreYear = false;
			int nMaxParts = 3;
			cnt::PushBackVector<tDatePartFmt>* pList = 0;
			switch(formatType)
			{
			case td::Date::Format::ShortY4:
				pList = &dateFormatOwnShortY4;
				break;
			case td::Date::Format::ShortY2:
				pList = &dateFormatOwnShortY2;
				yearShort = true;
				break;
			case td::Date::Format::ShortDM:
				ignoreYear = true;
				nMaxParts = 2;
				pList = &dateFormatOwnShortDM;
				break;
			default:
				assert(0);
			}

			int iPos = 0;
			unsigned int flag = 0;
			cnt::PushBackVector<tDatePartFmt>::const_iterator it (dateFormatWinShort.begin());
			cnt::PushBackVector<tDatePartFmt>::const_iterator itEnd (dateFormatWinShort.end());
			while (it != itEnd)
			{
				switch(it->part)
				{
				case Year:
				case YearShort:
					{
						if (ignoreYear)
							break;

						if (flag & 1)
							break;
						flag |= 1;
						++iPos;
						tDatePartFmt fmt;
						if (yearShort)
						{
							fmt.format = "%02d";
							fmt.part = YearShort;
						}
						else
						{
							fmt.format = "%d";
							fmt.part = Year;
						}
						pList->push_back(fmt);

						if (iPos < nMaxParts)
						{
							fmt.format = separator;
							fmt.part = DateSeparator;
							pList->push_back(fmt);
						}
					}
					break;
				case Month:
				case MonthName:
				case MonthShortName:
					{
						if (flag & 2)
							break;
						flag |= 2;
						++iPos;
						tDatePartFmt fmt;

						if (it->part == Month)
							fmt.format = it->format;
						else
							fmt.format = "%02d";

						fmt.part = Month;
						pList->push_back(fmt);

						if (iPos < nMaxParts)
						{
							fmt.format = separator;
							fmt.part = DateSeparator;
							pList->push_back(fmt);
						}
					}
				case Day:
				//case DayName:
				//case DayShortName:
					{
						if (flag & 4)
							break;
						flag |= 4;

						++iPos;
						tDatePartFmt fmt;

						//fmt.format = L"%02d";
						fmt.format = it->format;
						fmt.part = Day;
						pList->push_back(fmt);

						if (iPos < nMaxParts)
						{
							fmt.format = separator;
							fmt.part = DateSeparator;
							pList->push_back(fmt);
						}
					}
				default:
					break;
				}
				++it;
			}
		}

		/// @brief Appends a single parsed time format entry to the appropriate format list.
		/// @param formatType Target format list (WinLong or WinShort).
		/// @param ss         String stream accumulating literal separator text.
		/// @param lastChar   Format character identifying the time component type.
		/// @param nEntries   Number of consecutive occurrences of lastChar seen.
		void processTimeFormatEntries(td::Time::Format formatType, std::ostringstream& ss, char lastChar, int nEntries)
		{
			if (lastChar == 0)
				return;

			tTimePartFmt fmt;

			if ( lastChar == _h )
			{
				if (nEntries == 1)
				{
					fmt.part = Hour12;
					fmt.format = "%d";
				}
				else //if (nEntries == 2)
				{
					fmt.part = Hour12;
					fmt.format = "%02d";
				}
			}
			else if ( lastChar == _H )
			{
				if (nEntries == 1)
				{
					fmt.part = Hour24;
					fmt.format = "%d";
				}
				else //if (nEntries == 2)
				{
					fmt.part = Hour24;
					fmt.format = "%02d";
				}
			}
			else if (lastChar == _m)
			{
				if (nEntries == 1)
				{
					fmt.part = Minute;
					fmt.format = "%d";
				}
				else //if (nEntries == 2)
				{
					fmt.part = Minute;
					fmt.format = "%02d";
				}
			}
			else if (lastChar == _s)
			{
				if (nEntries == 1)
				{
					fmt.part = Second;
					fmt.format = "%d";
				}
				else //if (nEntries == 2)
				{
					fmt.part = Second;
					fmt.format = "%02d";
				}
			}
			else if (lastChar == _u)
			{
				if (nEntries == 1)
				{
					fmt.part = Milisecond;
					fmt.format = "%d";
				}
				else //if (nEntries == 2)
				{
					fmt.part = Milisecond;
					fmt.format = "%03d";
				}
			}
			else if (lastChar == _t)
			{
				fmt.part = AMPM;
				fmt.format = STRFMT;
			}
			else
			{
				ss << std::ends;
				fmt.part = TimeSeparator;
				fmt.format = ss.str().c_str();
				ss.str("");
			}

			if (formatType == td::Time::Format::WinLong)
			{
				timeFormatWinLong.push_back(fmt);
			}
			else if (formatType == td::Time::Format::WinShort)
			{
				timeFormatWinShort.push_back(fmt);
			}
		}

		/// @brief Appends a single parsed date format entry to the appropriate format list.
		/// @param formatType Target format list (WinLong or WinShort).
		/// @param ss         String stream accumulating literal separator text.
		/// @param lastChar   Format character identifying the date component type.
		/// @param nEntries   Number of consecutive occurrences of lastChar seen.
		void processDateFormatEntries(td::Date::Format formatType, std::ostringstream& ss, char lastChar, int nEntries)
		{
			if (lastChar == 0)
				return;

			tDatePartFmt fmt;

			if ( lastChar == _d ) // || (lastChar == _m) || (lastChar == _y))
			{
				if (nEntries == 1)
				{
					fmt.part = Day;
					fmt.format = "%d";
				}
				else if (nEntries == 2)
				{
					fmt.part = Day;
					fmt.format = "%02d";
				}
				else if (nEntries == 3)
				{
					fmt.part = DayShortName;
					fmt.format = STRFMT;
				}
				else
				{
					fmt.part = DayName;
					fmt.format = STRFMT;
				}
			}
			else if (lastChar == _m)
			{
				if (nEntries == 1)
				{
					fmt.part = Month;
					fmt.format = "%d";
				}
				else if (nEntries == 2)
				{
					fmt.part = Month;
					fmt.format = "%02d";
				}
				else if (nEntries == 3)
				{
					fmt.part = MonthShortName;
					fmt.format = STRFMT;
				}
				else
				{
					fmt.part = MonthName;
					fmt.format = STRFMT;
				}
			}
			else if (lastChar == _y)
			{
				if (nEntries == 2)
				{
					fmt.part = YearShort;
					fmt.format = "%02d";
				}
				else
				{
					fmt.part = Year;
					fmt.format = "%d";
				}
			}
			else if (lastChar != 0)
			{
				ss << std::ends;
				fmt.part = DateSeparator;
				fmt.format = ss.str().c_str();
				ss.str("");
			}

			if (formatType == td::Date::Format::WinLong)
			{
				dateFormatWinLong.push_back(fmt);
			}
			else if (formatType == td::Date::Format::WinShort)
			{
				dateFormatWinShort.push_back(fmt);
			}
		}

		/// @brief Parses a platform time format string and populates the corresponding format part list.
		/// @param formatType Target format list (WinLong or WinShort).
		/// @param format     Null-terminated time format pattern to parse.
		void prepareTimeFormat(td::Time::Format formatType, const char* format)
		{
			assert(format);

			if (format == 0)
				return;

			const char* pCh = format;

			std::ostringstream ss;

			char lastChar = 0;
			int nEntries = 0;

			bool hourDetected = false;

			bool hour12FormatDetected = false;

			while (*pCh != 0)
			{
				switch(*pCh)
				{
				case _h:
					{
						if (lastChar != _h)
						{
							if (hourDetected)
							{
								processTimeFormatEntries(formatType, ss, lastChar, nEntries);
								lastChar = 0;
								//wrong time format
								assert(0);
								break;
							}

							hour12FormatDetected = true;

							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//hour pattern detected
							lastChar = _h;
							nEntries = 1;
						}
						else
						{
							hourDetected = true;
							++nEntries;
						}
					}
					break;
				case _H:
					{
						if (lastChar != _H)
						{
							if (hourDetected)
							{
								processTimeFormatEntries(formatType, ss, lastChar, nEntries);
								lastChar = 0;
								//wrong time format
								assert(0);
								break;
							}

							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//hour pattern detected
							lastChar = _H;
							nEntries = 1;
						}
						else
						{
							hourDetected = true;
							++nEntries;
						}
					}
					break;
				case _m:
				case _M:
					{
						if (lastChar != _m)
						{
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//minute pattern detected
							lastChar = _m;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _s:
				case _S:
					{
						if (lastChar != _s)
						{
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//second pattern detected
							lastChar = _s;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _u:
					{
						if (lastChar != _u)
						{
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//milisecond pattern detected
							lastChar = _u;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _t:
					{
						if (!hour12FormatDetected)
						{
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							lastChar = 0;
							break;
						}

						if (lastChar != _t)
						{
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);
							//AMPM pattern detected
							lastChar = _t;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				default:
					{
						//process separators
						if ( (lastChar != _h) || (lastChar != _H) || (lastChar != _m) || (lastChar != _s) || (lastChar != _u) || (lastChar != _t) )
							processTimeFormatEntries(formatType, ss, lastChar, nEntries);

						lastChar = (char)*pCh;
						ss << lastChar;
					}
				}
				++pCh;
			}

			processTimeFormatEntries(formatType, ss, lastChar, nEntries);
		}

		/// @brief Parses a platform date format string and populates the corresponding format part list.
		/// @param formatType Target format list (WinLong or WinShort).
		/// @param format     Null-terminated date format pattern to parse.
		void prepareDateFormat(td::Date::Format formatType, const char* format)
		{
			assert(format);

			if (format == 0)
				return;

			const char* pCh = format;

			std::ostringstream ss;

			char lastChar = 0;
			int nEntries = 0;


			//bool inQuote = false;

			while (*pCh != 0)
			{
				switch(*pCh)
				{
				case _d:
				case _D:
					{
						if (lastChar != _d)
						{
							processDateFormatEntries(formatType, ss, lastChar, nEntries);
							//day pattern detected
							lastChar = _d;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _m:
				case _M:
					{
						if (lastChar != _m)
						{
							processDateFormatEntries(formatType, ss, lastChar, nEntries);
							//month pattern detected
							lastChar = _m;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _y:
				case _Y:
					{
						if (lastChar != _y)
						{
							processDateFormatEntries(formatType, ss, lastChar, nEntries);
							//year pattern detected
							lastChar = _y;
							nEntries = 1;
						}
						else
						{
							++nEntries;
						}
					}
					break;
				case _quot:
				{
					processDateFormatEntries(formatType, ss, lastChar, nEntries);
					++pCh;
					while (*pCh != 0 && *pCh != '\'')
					{
						ss << *pCh;
						++pCh;
					}

					lastChar = _quot;
				}
				break;

				default:
					{
						//process separators
						if ( (lastChar != _d) || (lastChar != _m) || (lastChar != _y) )
							processDateFormatEntries(formatType, ss, lastChar, nEntries);

						lastChar = (char)*pCh;
						ss << lastChar;
					}
				}
				if (*pCh != 0)
					++pCh;
			}

			processDateFormatEntries(formatType, ss, lastChar, nEntries);
		}

		/// @brief Replaces the decimal point character in pSource with the locale decimal point, then copies to pDest.
		/// @param pSource      Source buffer containing the formatted number with a standard decimal point.
		/// @param pDest        Destination buffer receiving the locale-formatted string.
		/// @param nLen         Length of the string in pSource.
		/// @param nDecPosition Number of digits after the decimal point; 0 means no replacement.
		inline void replaceDecPoint(char* pSource, char* pDest, int nLen, int nDecPosition)
		{
			if (nDecPosition > 0)
			{
				int nDec = nLen - nDecPosition - 1;
				pSource[nDec] = decPoint;
			}

			STRCPY(pDest, nLen, pSource);
		}

		//inline int showStars(int nDec)
		//{
		//	if (nDec > 0)
		//	{
		//		int nLen = nDec + 2;
		//		reserve(nDec + 2);
		//		for (int i=0; i<nLen; i++)
		//		{
		//			if (i == 1)
		//				strData[i] = decPoint;
		//			else
		//				strData[i] = L'*';
		//		}
		//		strData[nLen] = 0;
		//		return nLen;
		//	}
		//	else
		//	{
		//		reserve(1);
		//		strData[0] = L'*';
		//		strData[1] = 0;
		//		return 1;
		//	}
		//}

		/// @brief Allocates or reallocates a char buffer pointed to by *pDest with nLen+1 bytes.
		/// @param pDest Pointer to the char pointer to reallocate.
		/// @param nLen  Number of characters the new buffer must hold (excluding the null terminator).
		void reserve(char** pDest, int nLen)
		{
			if (*pDest)
			{
				delete [] *pDest;
			}
			*pDest = new char[nLen + 1];
		}

		/// @brief Writes a star-pattern string into pBuff representing an overflow indicator.
		/// @param pBuff Output buffer receiving the star pattern.
		/// @param nDec  Number of decimal places; determines the total pattern length.
		/// @return Number of characters written.
		inline int showStars(char* pBuff, int nDec)
		{
			if (nDec > 0)
			{
				int nLen = nDec + 2;
				//reserve(nDec + 2);
				for (int i=0; i<nLen; i++)
				{
					if (i == 1)
						pBuff[i] = '.';
					else
						pBuff[i] = '*';
				}
				pBuff[nLen] = 0;
				return nLen;
			}
			else
			{
				pBuff[0] = '*';
				pBuff[1] = 0;
				return 1;
			}
		}

		/// @brief Formats a floating-point value into pBuff using printf semantics, applying range limits.
		/// @tparam T      Float or double.
		/// @param pBuff   Output character buffer.
		/// @param val     Value to format.
		/// @param nDec    Number of decimal places (negative triggers scientific notation for small values).
		/// @param BUFFLEN Size of pBuff in bytes.
		/// @param fmt     Floating-point display format (Decimal or Scientific).
		/// @param trailingTxt Optional text to append after the number.
		/// @return Number of characters written, or a negative value on error.
		template <typename T>
        inline int convertFloat(char* pBuff, T val, int nDec, int BUFFLEN, td::FormatFloat fmt= td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
            if (val != 0)
            {
                if (nDec < 0)
                {
                    T eps = std::pow(T(10), nDec);
                    nDec = -nDec;
                    if (math::abs(val) < eps)
                    {
                        nDec = nDec / 2;
                        if (nDec == 0)
                            nDec = 1;
                        if (trailingTxt)
                            return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*e%s", nDec, val, trailingTxt);
                        else
                            return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*e", nDec, val);
                    }
                }
            }
            if (nDec < 0)
                nDec = -nDec;

			float minNumber = minFltFormat;
			float maxNumber = maxFltFormat;

			if (typeid(T) == typeid(double))
			{
				minNumber = minDblFormat;
				maxNumber = maxDblFormat;
			}

			T absVal = math::abs(val);

			if ( maxNumber < absVal)
			{
				if (BUFFLEN >= nDec + 2)
					return showStars(pBuff, nDec);
				else
					return -1;
			}

			if (absVal < minNumber)
			{
				val = 0;
			}

            if (fmt == td::FormatFloat::Decimal)
            {
                if (trailingTxt)
                {
                    if (nDec < 0)
                    {
                        return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%f%s", val, trailingTxt);
                    }

                    return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*f%s", nDec, val, trailingTxt);
                }

                if (nDec < 0)
                {
                    return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%f", val);
                }

                return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*f", nDec, val);
            }

            if (trailingTxt)
            {
                return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*e%s", nDec, val, trailingTxt);

//                if (nDec < 0)
//                {
//                    return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%g%s", val, trailingTxt);
//                }
//
//                return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*g%s", nDec, val, trailingTxt);
            }

//            if (nDec < 0)
//            {
//                return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%g", val);
//            }

            return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*e", nDec, val);
//            return SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, "%.*g", nDec, val);
		}

		/// @brief Inserts thousand separators into pIn and writes the result to pOut.
		/// @param pIn      Source buffer containing a formatted integer or decimal string.
		/// @param pOut     Destination buffer receiving the string with separators.
		/// @param nTh      Number of thousand-separator groups to insert.
		/// @param nToSkip  Number of leading characters to copy before inserting the first separator.
		/// @param negative If true, the value is negative and the leading minus sign is handled accordingly.
		inline void replaceThSeps(char* pIn, char* pOut, int nTh, int nToSkip, bool negative)
		{
			int i = 0;
			while (i < nToSkip)
			{
				*pOut= *pIn;
				++pOut;
				++pIn;
				++i;
			}

			if (negative)
			{
				if (nToSkip > 1)
				{
					*pOut++ = thousandSeparator;
					nTh--;
				}
			}
			else if ( nToSkip > 0)
			{
				*pOut++ = thousandSeparator;
				nTh--;
			}

			i = 0;
			while (nTh > 0)
			{
				if (i == 3)
				{
					*pOut++ = thousandSeparator;
					nTh--;
					i = 0;
				}
				else
				{
					*pOut = *pIn;
					++pOut;
					++pIn;
					++i;
				}
			}
			STRCPY(pOut, _fmtBuffLen, pIn);
		}

		/// @brief Formats a floating-point value into pDest, applying the locale decimal point and optional thousand separators.
		/// @tparam T              Float or double.
		/// @param pDest           Destination buffer for the final formatted string.
		/// @param pBuff           Scratch buffer of at least BUFFLEN bytes used for intermediate formatting.
		/// @param val             Value to format.
		/// @param nDec            Number of decimal places.
		/// @param BUFFLEN         Size of pBuff in bytes.
		/// @param replaceThousand If true, thousand separators are inserted.
		/// @param fmt             Floating-point display format (Decimal or Scientific).
		/// @param trailingTxt     Optional text to append after the number.
		/// @return Number of characters written to pDest, or a negative value on error.
		template <typename T>
        inline int formatFloat(char* pDest, char* pBuff, T val, int nDec, int BUFFLEN, bool replaceThousand, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
			assert(BUFFLEN >= nDec + 1);

			if (BUFFLEN < nDec + 1)
				return -1;

			int nLen = convertFloat(pBuff, val, nDec, BUFFLEN, fmt, trailingTxt);
			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

            pBuff[nLen] = 0;
            if (nDec < 0)
                nDec = -nDec;

            int nDecPos = nLen - nDec - 1;
            if (fmt == td::FormatFloat::Scientific)
            {
                if (nLen <= 1)
                    return nLen;
                nDecPos = 1;
            }

            if (trailingTxt)
            {
                int lenTrail = (int) strlen(trailingTxt);
                nDecPos -= lenTrail;
                if (nDec > 0 && nDecPos >= 0)
                    pBuff[nDecPos] = decPoint;
            }
            else
            {
                if (nDec > 0 && nDecPos >= 0)
                    pBuff[nDecPos] = decPoint;
            }


			if (replaceThousand && (fmt == td::FormatFloat::Decimal))
			{
				int nCio = nDecPos;

				int nToSkip = 0;
				bool negative = false;
				if (val < 0)
				{
					negative = true;
					nCio--;
					nToSkip = 1;
				}

				if (nCio > 3)
				{
					nToSkip += nCio % 3;
					int nTh = (nCio - 1) / 3;
					nLen += nTh;
					replaceThSeps(pBuff, pDest, nTh, nToSkip, negative);
				}
				else
				{
					STRCPY(pDest, BUFFLEN, pBuff);
				}
			}
			else
			{
				STRCPY(pDest, BUFFLEN, pBuff);
			}

			return nLen;
		}

		/// @brief Formats an integer value into pDest, optionally inserting thousand separators.
		/// @tparam T              Integer type.
		/// @param pDest           Destination buffer for the final formatted string.
		/// @param pBuff           Scratch buffer of at least BUFFLEN bytes.
		/// @param val             Integer value to format.
		/// @param pWFormat        Printf-style format string for the value (e.g. "%d").
		/// @param BUFFLEN         Size of pBuff in bytes.
		/// @param replaceThousand If true, thousand separators are inserted.
		/// @param trailingTxt     Optional text to append after the number.
		/// @return Number of characters written to pDest, or a negative value on error.
		template <typename T>
		inline int formatInt(char* pDest, char* pBuff, T val, const char* pWFormat, int BUFFLEN, bool replaceThousand, const char* trailingTxt = nullptr)
		{
            int nLen = 0;
            if (trailingTxt)
            {
                if (!replaceThousand)
                {
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val, trailingTxt);
                }

                if (val >= 0 && val < 1000)
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val, trailingTxt);
                else if (val < 0 && val > -1000)
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val, trailingTxt);

                nLen = SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, pWFormat, val, trailingTxt);
            }
            else
            {
                if (!replaceThousand)
                {
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val);
                }

                if (val >= 0 && val < 1000)
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val);
                else if (val < 0 && val > -1000)
                    return SNPRINTF(pDest, BUFFLEN - 1, _TRUNCATE, pWFormat, val);

                nLen = SNPRINTF(pBuff, BUFFLEN - 1, _TRUNCATE, pWFormat, val);
            }


			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			int nCio = nLen;

			int nToSkip = 0;
			bool negative = false;
			if (val < 0)
			{
				negative = true;
				nCio--;
				nToSkip = 1;
			}

			if (nCio > 3)
			{
				nToSkip += nCio % 3;
				int nTh = (nCio - 1) / 3;
				nLen += nTh;
				replaceThSeps(pBuff, pDest, nTh, nToSkip, negative);
			}
			else
			{
				STRCPY(pDest, BUFFLEN, pBuff);
			}
			return nLen;
		}

		/// @brief Formats a td::Time value into pBuff according to the specified format.
		/// @param tf       Time format to apply.
		/// @param t        Time value to format.
		/// @param pBuff    Output character buffer.
		/// @param nBufLen  Size of pBuff in bytes.
		/// @return Number of characters written, or a negative value on error.
		inline int format(td::Time::Format tf, const td::Time& t, char* pBuff, int nBufLen)
		{
			int toRet = 0;
			//int nBufLen = _maxLen;
			//char* pBuff = formatBuffer;
			cnt::PushBackVector<tTimePartFmt>::const_iterator it;
			cnt::PushBackVector<tTimePartFmt>::const_iterator itEnd;
			switch (tf)
			{
				case td::Time::Format::ShortHMM:
					it = timeFormatOwnShortHMM.begin();
					itEnd = timeFormatOwnShortHMM.end();
					break;
				case td::Time::Format::ShortHMMSS:
					it = timeFormatOwnShortHMMSS.begin();
					itEnd = timeFormatOwnShortHMMSS.end();
					break;
				case td::Time::Format::ShortHMMSSu:
					it = timeFormatOwnShortHMMSSu.begin();
					itEnd = timeFormatOwnShortHMMSSu.end();
					break;
				case td::Time::Format::WinShort:
					it = timeFormatWinShort.begin();
					itEnd = timeFormatWinShort.end();
					break;
				case td::Time::Format::WinLong:
					it = timeFormatWinLong.begin();
					itEnd = timeFormatWinLong.end();
					break;
				default:
					assert(false);
					it = timeFormatOwnShortHMM.begin();
					itEnd = timeFormatOwnShortHMM.end();
			}

			bool bPM = false;
			while (it != itEnd)
			{
				int nPart1 = 0;
				switch( it->part)
				{
				case mu::Regionals::Hour24:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), t.getHour());
					break;
				case mu::Regionals::Hour12:
					{
						int hHour12 = t.getHour();
						if (hHour12 >= 12)
						{
							bPM = true;
							hHour12 -= 12;
						}
						nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), hHour12);
					}
					break;
				case mu::Regionals::Minute:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), t.getMinute());
					break;
				case mu::Regionals::Second:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), t.getSecond());
					break;
				case mu::Regionals::Milisecond:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), t.getMilisecond());
					break;
				case mu::Regionals::AMPM:
					{
						if (bPM)
						{
							nPart1 = pm.length();
							if (nPart1 < nBufLen)
							{
								STRCPY(pBuff, nBufLen, pm.c_str());
							}
							else
								nPart1 = -1;
						}
						else
						{
							nPart1 = am.length();
							if (nPart1 < nBufLen)
							{
								STRCPY(pBuff, nBufLen, am.c_str());
							}
							else
								nPart1 = -1;
						}
					}
					break;
				case mu::Regionals::TimeSeparator:
					{
						nPart1 = it->format.length();
						if (nPart1 < nBufLen)
						{
							STRCPY(pBuff, nBufLen, it->format.c_str());
						}
						else
							nPart1 = -1;
					}
					break;
				}
				if (nPart1 > 0)
				{
					toRet += nPart1;
					pBuff += nPart1;
					nBufLen -= nPart1;
				}
				else
					return -1;
				++it;
			}

			return toRet;
		}

	public:

		/// @brief Sets the Before Christ designator string.
		/// @param strBC Null-terminated string to use (e.g. "BC").
		void setBCString(const char* strBC)
		{
			bc = strBC;
		}

		/// @brief Returns the maximum usable length of the format buffer.
		/// @return Maximum number of characters that can be written to formatBuffer.
		int getBuffLen() const
		{
			return _maxLen;
		}

		/// @brief Sets all 12 full month name strings.
		/// @param names Array of 12 PascalString<1> month names indexed January=0 through December=11.
		void setMonthNames (const td::PascalString<1> names[12])
		{
			for (int i=0; i<12; i++)
			{
				monthNames[i] = names[i];
			}
		}

		/// @brief Sets all 7 full day name strings.
		/// @param names Array of 7 PascalString<1> day names indexed Sunday=0 through Saturday=6.
		void setDayNames (const td::PascalString<1> names[7])
		{
			for (int i=0; i<7; i++)
			{
				dayNames[i] = names[i];
			}
		}

		/// @brief Sets the full name of a single month.
		/// @param i    Zero-based month index (0=January, 11=December).
		/// @param name Null-terminated month name string.
		void setMonthName(int i, const char* name)
		{
			assert( (i >= 0) && (i < 12) );
			monthNames[i] = name;
		}

		/// @brief Sets the abbreviated name of a single month.
		/// @param i         Zero-based month index (0=January, 11=December).
		/// @param shortName Null-terminated abbreviated month name string.
		void setMonthShortName(int i, const char* shortName)
		{
			assert( (i >= 0) && (i < 12) );
			monthShortNames[i] = shortName;
		}

		/// @brief Sets the full name of a single weekday.
		/// @param i    Zero-based day index (0=Sunday, 6=Saturday).
		/// @param name Null-terminated day name string.
		void setDayName(int i, const char* name)
		{
			assert( (i >= 0) && (i < 7) );
			dayNames[i] = name;
		}

		/// @brief Sets the abbreviated name of a single weekday.
		/// @param i         Zero-based day index (0=Sunday, 6=Saturday).
		/// @param shortName Null-terminated abbreviated day name string.
		void setDayShortName(int i, const char* shortName)
		{
			assert( (i >= 0) && (i < 7) );
			dayShortNames[i] = shortName;
		}

		/// @brief Formats a floating-point value into the internal format buffer.
		/// @tparam T             Float or double.
		/// @param val            Value to format.
		/// @param nDec           Number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param fmt            Floating-point display format.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
        int formatFloat(T val, int nDec, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
			return formatFloat(formatBuffer, formatBuffer + BUFF_OFFSET, val, nDec, _maxLen - BUFF_OFFSET, setThSeparator, fmt, trailingTxt);
		}

		/// @brief Formats a label followed by a floating-point value into the internal format buffer.
		/// @tparam T             Float or double.
		/// @param label          Null-terminated prefix label copied before the number.
		/// @param labelLen       Length of label in characters.
		/// @param val            Value to format.
		/// @param nDec           Number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param fmt            Floating-point display format.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Total number of characters written including the label.
		template <typename T>
		int formatFloat(const char* label, int labelLen, T val, int nDec, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
			STRCPY(formatBuffer, _fmtBuffLen, label);
			return formatFloat(formatBuffer + labelLen, formatBuffer + BUFF_OFFSET + labelLen, val, nDec, _maxLen - BUFF_OFFSET - labelLen, setThSeparator, fmt, trailingTxt) + labelLen;
		}

		/// @brief Formats a floating-point value followed by a label into the internal format buffer.
		/// @tparam T             Float or double.
		/// @param val            Value to format.
		/// @param nDec           Number of decimal places.
		/// @param postLabel      Null-terminated suffix label appended after the number.
		/// @param postLabelLen   Length of postLabel in characters.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param fmt            Floating-point display format.
		/// @param trailingTxt    Optional text to append after the number but before postLabel.
		/// @return Total number of characters written including the suffix label.
		template <typename T>
		int formatFloat(T val, int nDec, const char* postLabel, int postLabelLen, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
            int nLen = formatFloat(formatBuffer, formatBuffer + BUFF_OFFSET, val, nDec, _maxLen - BUFF_OFFSET, setThSeparator, fmt, trailingTxt);
			STRCPY(formatBuffer + nLen, _fmtBuffLen - nLen, postLabel);
			return nLen + postLabelLen;
		}

		/// @brief Formats an integer value into the internal format buffer using a custom printf format.
		/// @tparam T             Integer type.
		/// @param val            Value to format.
		/// @param pFormat        Printf-style format string (e.g. "%d").
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
		int formatInt(T val, const char* pFormat, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, pFormat, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
		}

		/// @brief Formats a signed integer value into the internal format buffer using %d.
		/// @tparam T             Signed integer type.
		/// @param val            Value to format.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
		int formatInt(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%d%s", _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%d", _maxLen - BUFF_OFFSET, setThSeparator);
		}

		/// @brief Formats an unsigned integer value into the internal format buffer using %u.
		/// @tparam T             Unsigned integer type.
		/// @param val            Value to format.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
		int formatUInt(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%u%s", _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%u", _maxLen - BUFF_OFFSET, setThSeparator);
		}

		/// @brief Formats a signed 64-bit integer value into the internal format buffer.
		/// @tparam T             64-bit signed integer type.
		/// @param val            Value to format.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
		int formatLINT(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LINT8, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LINT8, _maxLen - BUFF_OFFSET, setThSeparator);
		}

		/// @brief Formats an unsigned 64-bit integer value into the internal format buffer.
		/// @tparam T             64-bit unsigned integer type.
		/// @param val            Value to format.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @param trailingTxt    Optional text to append after the number.
		/// @return Number of characters written.
		template <typename T>
		int formatLUINT(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LUINT8, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LUINT8, _maxLen - BUFF_OFFSET, setThSeparator);
		}

		/// @brief Builds a separator-delimited string of five formatted floating-point values in the internal buffer.
		/// @tparam TFloat     Float or double.
		/// @tparam SEPARATOR  Single character placed between consecutive values.
		/// @param val1..val5  Values to format.
		/// @param nDec1..nDec5 Number of decimal places for each value.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written, or a negative value on error.
		template <typename TFloat, char SEPARATOR>
		inline int buildFloat5(TFloat val1, int nDec1, TFloat val2, int nDec2, TFloat val3, int nDec3, TFloat val4, int nDec4, TFloat val5, int nDec5, bool setThSeparator)
		{
			char* pDest = formatBuffer;
			char* pTmp = formatBuffer + BUFF_OFFSET;

			int nTotalLen = 0;
			//1
			int nLen = formatFloat(pDest, pTmp, val1, nDec1, _maxLen - BUFF_OFFSET, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;
			assert(nTotalLen + BUFF_OFFSET < _maxLen);

			//2
			nLen = formatFloat(pDest, pTmp, val2, nDec2, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;
			assert(nTotalLen + BUFF_OFFSET < _maxLen);

			//3
			nLen = formatFloat(pDest, pTmp, val3, nDec3, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;
			assert(nTotalLen + BUFF_OFFSET < _maxLen);

			//4
			nLen = formatFloat(pDest, pTmp, val4, nDec4, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;
			assert(nTotalLen + BUFF_OFFSET < _maxLen);

			//5
			nLen = formatFloat(pDest, pTmp, val5, nDec5, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			//pDest[nLen++]= SEPARATOR;
			nTotalLen += nLen;
			return nTotalLen;
		}

		/// @brief Builds a separator-delimited string of four formatted floating-point values in the internal buffer.
		/// @tparam TFloat     Float or double.
		/// @tparam SEPARATOR  Single character placed between consecutive values.
		/// @param val1..val4  Values to format.
		/// @param nDec1..nDec4 Number of decimal places for each value.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written, or a negative value on error.
		template <typename TFloat, char SEPARATOR>
		inline int buildFloat4(TFloat val1, int nDec1, TFloat val2, int nDec2, TFloat val3, int nDec3, TFloat val4, int nDec4, bool setThSeparator)
		{
			char* pDest = formatBuffer;
			char* pTmp = formatBuffer + BUFF_OFFSET;

			int nTotalLen = 0;
			//1
			int nLen = formatFloat(pDest, pTmp, val1, nDec1, _maxLen - BUFF_OFFSET, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//2
			nLen = formatFloat(pDest, pTmp, val2, nDec2, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//3
			nLen = formatFloat(pDest, pTmp, val3, nDec3, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//4
			nLen = formatFloat(pDest, pTmp, val4, nDec4, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			//pDest[nLen++]= SEPARATOR;
			nTotalLen += nLen;
			return nTotalLen;
		}

		/// @brief Builds a separator-delimited string of three formatted floating-point values in the internal buffer.
		/// @tparam TFloat     Float or double.
		/// @tparam SEPARATOR  Single character placed between consecutive values.
		/// @param val1..val3  Values to format.
		/// @param nDec1..nDec3 Number of decimal places for each value.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written, or a negative value on error.
		template <typename TFloat, char SEPARATOR>
		inline int buildFloat3(TFloat val1, int nDec1, TFloat val2, int nDec2, TFloat val3, int nDec3, bool setThSeparator)
		{
			char* pDest = formatBuffer;
			char* pTmp = formatBuffer + BUFF_OFFSET;

			int nTotalLen = 0;
			//1
			int nLen = formatFloat(pDest, pTmp, val1, nDec1, _maxLen - BUFF_OFFSET, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//2
			nLen = formatFloat(pDest, pTmp, val2, nDec2, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//3
			nLen = formatFloat(pDest, pTmp, val3, nDec3, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			//pDest[nLen++]= SEPARATOR;
			nTotalLen += nLen;
			return nTotalLen;
		}

		/// @brief Builds a separator-delimited string of two formatted floating-point values in the internal buffer.
		/// @tparam TFloat     Float or double.
		/// @tparam SEPARATOR  Single character placed between consecutive values.
		/// @param val1        First value to format.
		/// @param nDec1       Number of decimal places for val1.
		/// @param val2        Second value to format.
		/// @param nDec2       Number of decimal places for val2.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written, or a negative value on error.
		template <typename TFloat, char SEPARATOR>
		inline int buildFloat2(TFloat val1, int nDec1, TFloat val2, int nDec2, bool setThSeparator)
		{
			char* pDest = formatBuffer;
			char* pTmp = formatBuffer + BUFF_OFFSET;

			int nTotalLen = 0;
			//1
			int nLen = formatFloat(pDest, pTmp, val1, nDec1, _maxLen - BUFF_OFFSET, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			pDest[nLen++]= SEPARATOR;
			pDest += nLen;
			nTotalLen += nLen;
			pTmp = pDest + BUFF_OFFSET;

			//2
			nLen = formatFloat(pDest, pTmp, val2, nDec2, _maxLen - BUFF_OFFSET - nTotalLen, setThSeparator);

			if (nLen < 0)
			{
				assert(0);
				return nLen;
			}

			//pDest[nLen++]= SEPARATOR;
			nTotalLen += nLen;
			return nTotalLen;
		}

		/// @brief Formats five floating-point values with the same decimal precision, separated by newlines.
		/// @tparam TFloat Float or double.
		/// @param val1..val5     Values to format.
		/// @param nDec           Common number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written.
		template <typename TFloat>
		inline int uniformFltBuild5(TFloat val1, TFloat val2, TFloat val3, TFloat val4, TFloat val5, int nDec, bool setThSeparator = true)
		{
			return buildFloat5<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, val5, nDec, setThSeparator);
		}

		/// @brief Formats four floating-point values with the same decimal precision, separated by newlines.
		/// @tparam TFloat Float or double.
		/// @param val1..val4     Values to format.
		/// @param nDec           Common number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written.
		template <typename TFloat>
		inline int uniformFltBuild4(TFloat val1, TFloat val2, TFloat val3, TFloat val4, int nDec, bool setThSeparator= true)
		{
			return buildFloat3<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, setThSeparator);
		}

		/// @brief Formats three floating-point values with the same decimal precision, separated by newlines.
		/// @tparam TFloat Float or double.
		/// @param val1..val3     Values to format.
		/// @param nDec           Common number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written.
		template <typename TFloat>
		inline int uniformFltBuild3(TFloat val1, TFloat val2, TFloat val3, int nDec, bool setThSeparator= true)
		{
			return buildFloat3<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, setThSeparator);
		}

		/// @brief Formats two floating-point values with the same decimal precision, separated by a newline.
		/// @tparam TFloat Float or double.
		/// @param val1           First value to format.
		/// @param val2           Second value to format.
		/// @param nDec           Common number of decimal places.
		/// @param setThSeparator If true, thousand separators are inserted.
		/// @return Total number of characters written.
		template <typename TFloat>
		inline int uniformFltBuild2(TFloat val1, TFloat val2, int nDec, bool setThSeparator= true)
		{
			return buildFloat2<TFloat, '\n'>(val1, nDec, val2, nDec, setThSeparator);
		}


		/// @brief Formats a td::Date value into the internal format buffer according to the specified date format.
		/// @param df Date format to apply.
		/// @param d  Date value to format.
		/// @return Number of characters written, or -1 on error.
		int format(td::Date::Format df, const td::Date& d)
		{
			char* pBuff = formatBuffer;
			if (d.isNull())
			{
				STRCPY(pBuff, _fmtBuffLen, "null");
				return 4;
			}

			int toRet = 0;
			int nBufLen = _maxLen;

			cnt::PushBackVector<tDatePartFmt>::const_iterator it = dateFormatOwnShortY4.begin();
			cnt::PushBackVector<tDatePartFmt>::const_iterator itEnd = dateFormatOwnShortY4.end();
			switch (df)
			{
				case td::Date::Format::ShortY4:
					it = dateFormatOwnShortY4.begin();
					itEnd = dateFormatOwnShortY4.end();
					break;
				case td::Date::Format::ShortY2:
					it = dateFormatOwnShortY2.begin();
					itEnd = dateFormatOwnShortY2.end();
					break;
				case td::Date::Format::WinLong:
					it = dateFormatWinLong.begin();
					itEnd = dateFormatWinLong.end();
					break;
				case td::Date::Format::WinShort:
					it = dateFormatWinShort.begin();
					itEnd = dateFormatWinShort.end();
					break;
				case td::Date::Format::ShortDM:
					it = dateFormatOwnShortDM.begin();
					itEnd = dateFormatOwnShortDM.end();
					break;
				default:
					assert(false);
			}

			while (it != itEnd)
			{
				int nPart1 = 0;
				switch( it->part)
				{
				case mu::Regionals::Day:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), d.getDay());
					break;
				case mu::Regionals::Month:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), d.getMonth());
					break;
				case mu::Regionals::Year:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), d.getYear());
					break;
				case mu::Regionals::YearShort:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), d.getYear() % 100);
					break;
				case mu::Regionals::DayName:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), dayNames[d.getWeekDay()].c_str());
					break;
				case mu::Regionals::DayShortName:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), dayShortNames[d.getWeekDay()].c_str());
					break;
				case mu::Regionals::MonthName:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), monthNames[d.getMonth() - 1].c_str());
					break;
				case mu::Regionals::MonthShortName:
					nPart1 = SNPRINTF(pBuff, nBufLen, _TRUNCATE, it->format.c_str(), monthShortNames[d.getMonth() - 1].c_str());
					break;
				case mu::Regionals::DateSeparator:
					{
						nPart1 = it->format.length();
						if (nPart1 < nBufLen)
						{
							STRCPY(pBuff, nBufLen, it->format.c_str());
						}
						else
							nPart1 = -1;
					}
					break;
				}
				if (nPart1 > 0)
				{
					toRet += nPart1;
					pBuff += nPart1;
					nBufLen -= nPart1;
				}
				else
					return -1;
				++it;
			}

			if (d.BC())
			{
				toRet += bc.length()+1;
				*pBuff = ' ';
				STRCPY(pBuff+1, toRet, bc.c_str());
			}

			return toRet;
		}

		/// @brief Formats a td::Date into a user-input oriented string stored in buff.
		/// @param buff        Output character buffer.
		/// @param buffLen     Size of buff in bytes.
		/// @param date        Date value to format.
		/// @param putZeros    If true, each component is zero-padded to a fixed width.
		/// @return Number of characters written.
        inline int formatInputDate(char* buff, int buffLen, const td::Date& date, bool putZeros)
        {
            int nLen = 0;
            if (dateInputFormat[0] == 'd')
            {
                if (dateInputFormat[1] == 'm')
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getDay(), dateInputSeparator, date.getMonth(), dateInputSeparator, date.getYear());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getDay(), dateInputSeparator, date.getMonth(), dateInputSeparator, date.getYear());
                }
                else
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getDay(), dateInputSeparator, date.getYear(), dateInputSeparator, date.getMonth());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getDay(), dateInputSeparator, date.getYear(), dateInputSeparator, date.getMonth());
                }
            }
            else if (dateInputFormat[0] == 'm')
            {
                if (dateInputFormat[1] == 'y')
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getMonth(), dateInputSeparator, date.getYear(), dateInputSeparator, date.getDay());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getMonth(), dateInputSeparator, date.getYear(), dateInputSeparator, date.getDay());
                }
                else
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getMonth(), dateInputSeparator, date.getDay(), dateInputSeparator, date.getYear());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getMonth(), dateInputSeparator, date.getDay(), dateInputSeparator, date.getYear());
                }
            }
            else //if (dateInputFormat[0] == 'y')
            {
                if (dateInputFormat[1] == 'm')
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getYear(), dateInputSeparator, date.getMonth(), dateInputSeparator, date.getDay());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getYear(), dateInputSeparator, date.getMonth(), dateInputSeparator, date.getDay());
                }
                else
                {
                    if (putZeros)
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d%c%02d%c%04d", date.getYear(), dateInputSeparator, date.getDay(), dateInputSeparator, date.getMonth());
                    else
                        nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d%c%d%c%d", date.getYear(), dateInputSeparator, date.getDay(), dateInputSeparator, date.getMonth());
                }
            }
            return nLen;
        }

		/// @brief Formats a td::Date into the internal format buffer using the input date order.
		/// @param date     Date value to format.
		/// @param putZeros If true, each component is zero-padded to a fixed width.
		/// @return Number of characters written.
        inline int formatInputDate(const td::Date& date, bool putZeros)
        {
            return formatInputDate(formatBuffer, _maxLen, date, putZeros);
        }

		/// @brief Parses a date string buff according to the configured input date order.
		/// @param buff Null-terminated input date string.
		/// @return Parsed td::Date value; falls back to the current date on empty or invalid input.
        inline td::Date getInputDate(const char* buff)
        {
            int nLen = (int) strlen(buff);
            if (nLen < 1)
            {
                td::Date date;
                date.now();
                return date;
            }

            int a=1,b=1,c=1;
            char ch1, ch2;
#ifdef MU_WINDOWS
            sscanf_s(buff, "%d%c%d%c%d", &a, &ch1, (unsigned)sizeof(char), &b, &ch2, (unsigned)sizeof(char), &c);
#else
            sscanf(buff, "%d%c%d%c%d", &a, &ch1, &b, &ch2, &c);
#endif
            td::Date date;
            date.now();
            if (dateInputFormat[0] == 'd')
            {
                if (dateInputFormat[1] == 'm')
                {
                    date.setDate(c, b, a);
                }
                else
                {
                    date.setDate(c, a, b);
                }
            }
            else if (dateInputFormat[0] == 'm')
            {
                if (dateInputFormat[1] == 'y')
                {
                    date.setDate(b, a, c);
                }
                else
                {
                    date.setDate(c, a, b);
                }
            }
            else //(dateInputFormat[0] == 'd')
            {
                if (dateInputFormat[1] == 'y')
                {
                    date.setDate(b, c, a);
                }
                else
                {
                    date.setDate(c, b, a);
                }
            }
            return date;
        }

		/// @brief Formats a td::Time into the provided buffer using a fixed HH:MM:SS.ffff layout.
		/// @param buff            Output character buffer.
		/// @param buffLen         Size of buff in bytes.
		/// @param time            Time value to format.
		/// @param putLeadingZeros If true, hour, minute and second are zero-padded.
		/// @return Number of characters written.
        int formatInputTime(char* buff, int buffLen, const td::Time& time, bool putLeadingZeros)
        {
            int nLen = 0;
            if (putLeadingZeros)
                nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d:%02d:%02d.%04d", time.getHour(), time.getMinute(), time.getSecond(), time.getFraction());
            else
                nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d:%d:%d.%04d", time.getHour(), time.getMinute(), time.getSecond(), time.getFraction());

            return nLen;
        }

		/// @brief Formats a td::Time into the internal format buffer using a fixed HH:MM:SS.ffff layout.
		/// @param time            Time value to format.
		/// @param putLeadingZeros If true, hour, minute and second are zero-padded.
		/// @return Number of characters written.
        int formatInputTime(const td::Time& time, bool putLeadingZeros)
        {
            return formatInputTime(formatBuffer, _maxLen, time, putLeadingZeros);
        }

		/// @brief Reads a date from the internal format buffer using the configured input date order.
		/// @return Parsed td::Date value.
        inline td::Date getInputDate()
        {
            return getInputDate(formatBuffer);
        }

		/// @brief Formats a td::Color value into the internal format buffer.
		/// @param color Color value to format.
		/// @return Number of characters written.
        inline int format(const td::Color& color)
        {
            return color.formc_str(formatBuffer, _maxLen);
        }

		/// @brief Formats a td::Time value into the internal format buffer using the specified time format.
		/// @param tf Time format to apply.
		/// @param t  Time value to format.
		/// @return Number of characters written, or -1 on error.
		inline int format(td::Time::Format tf, const td::Time& t)
		{
			return format(tf, t, formatBuffer, _maxLen);
		}

		/// @brief Formats a td::DateTime value into the internal format buffer using separate date and time formats.
		/// @param df Date format to apply to the date component.
		/// @param tf Time format to apply to the time component.
		/// @param dt DateTime value to format.
		/// @return Total number of characters written (date + space + time), or -1 on error.
		int format(td::Date::Format df, td::Time::Format tf, const td::DateTime& dt)
		{
			td::Date d(dt.getDate());
			int nLen1 = 0;
			if ( (d.getMonth() == 0) || (d.getDay() == 0) )
			{
				formatBuffer[nLen1++] = '0';
				formatBuffer[nLen1++] = '.';
				formatBuffer[nLen1++] = '0';
				formatBuffer[nLen1++] = '.';
				formatBuffer[nLen1++] = '0';
			}
			else
				nLen1 = format(df, d);

			if (nLen1 < 0)
				return nLen1;
			int nBuff = _maxLen - nLen1 - 1;
			assert(nBuff > 5);
			if (nBuff > 5)
			{
				formatBuffer[nLen1++] = ' ';
			}
			td::Time t(dt.getTime());
			int nLen2 = format(tf, t, formatBuffer + nLen1, nBuff);
			if (nLen2 < 0)
				return nLen2;

			return nLen1 + nLen2;
		}

	protected:
		/// @brief Formats a Decimal value with an INT4 integer part into buff.
		/// @param buff        Output buffer.
		/// @param intPart     Integer component of the decimal value.
		/// @param decPart     Fractional component of the decimal value.
		/// @param bNegative   If true, a leading minus sign is prepended.
		/// @param NDEC        Number of decimal places.
		/// @param trailingTxt Optional text appended after the formatted value.
		/// @return Number of characters written.
		inline int prepareDecimal(char* buff, td::INT4 intPart, td::INT4 decPart, bool bNegative, int NDEC, const char* trailingTxt) const
		{
			int nLen = 0;

			if (bNegative)
			{
				if (NDEC > 0)
				{
                    if (trailingTxt)
                    {
                        const char* negFmt = "-%d%c%0*d%s";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart, decPoint, NDEC, decPart, trailingTxt);
                    }
                    else
                    {
                        const char* negFmt = "-%d%c%0*d";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart, decPoint, NDEC, decPart);
                    }

				}

			}
			else
			{
                if (trailingTxt)
                {
                    const char* posFmt = "%d%c%0*d%s";
                    nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart, decPoint, NDEC, decPart, trailingTxt);
                }
                else
                {
                    const char* posFmt = "%d%c%0*d";
                    nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart, decPoint, NDEC, decPart);
                }

			}

			return nLen;
		}

		/// @brief Formats a Decimal value with a LINT8 integer part into buff.
		/// @param buff        Output buffer.
		/// @param intPart     Integer component of the decimal value (64-bit).
		/// @param decPart     Fractional component of the decimal value (64-bit).
		/// @param bNegative   If true, a leading minus sign is prepended.
		/// @param NDEC        Number of decimal places.
		/// @param trailingTxt Optional text appended after the formatted value.
		/// @return Number of characters written.
		inline int prepareDecimal(char* buff, td::LINT8 intPart, td::LINT8 decPart, bool bNegative, int NDEC, const char* trailingTxt) const
		{
			int nLen = 0;

			if (bNegative)
			{
				if (NDEC > 0)
				{
                    if (trailingTxt)
                    {
                        const char* negFmt = "-" FMT_LINT8 "%c%0*" FMT_LINT8_EXT "%s";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart, decPoint, NDEC, decPart, trailingTxt);
                    }
                    else
                    {
                        const char* negFmt = "-" FMT_LINT8 "%c%0*" FMT_LINT8_EXT;
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart, decPoint, NDEC, decPart);
                    }

				}
				else
				{
                    if (trailingTxt)
                    {
                        const char* negFmt = "-" FMT_LINT8 "%s";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart, trailingTxt);
                    }
					else
                    {
                        const char* negFmt = "-" FMT_LINT8;
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, negFmt, intPart);
                    }
				}
			}
			else
			{
				if (NDEC > 0)
				{
                    if (trailingTxt)
                    {
                        const char* posFmt = FMT_LINT8 "%c%0*" FMT_LINT8_EXT "%s";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart, decPoint, NDEC, decPart, trailingTxt);
                    }
                    else
                    {
                        const char* posFmt = FMT_LINT8 "%c%0*" FMT_LINT8_EXT;
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart, decPoint, NDEC, decPart);
                    }

				}
				else
				{
                    if (trailingTxt)
                    {
                        const char* posFmt = FMT_LINT8 "%s";
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart, trailingTxt);
                    }
                    else
                    {
                        const char* posFmt = FMT_LINT8;
                        nLen = SNPRINTF(buff, _maxLen, _TRUNCATE, posFmt, intPart);
                    }

				}
			}

			return nLen;
		}

	public:
		/// @brief Formats a td::Decimal value into the internal format buffer, optionally with thousand separators.
		/// @tparam TDEC           Underlying integer type of the Decimal.
		/// @tparam NLEN           Number of decimal places.
		/// @param dec             Decimal value to format.
		/// @param replaceThousand If true, thousand separators are inserted into the integer part.
		/// @param trailingTxt     Optional text appended after the formatted value.
		/// @return Number of characters written, or a negative value on error.
		template<typename TDEC, int NLEN>
		int format(const td::Decimal<TDEC, NLEN>& dec, bool replaceThousand, const char* trailingTxt = nullptr)
		{
			//char* pDest = formatBuffer;
			char* pTmp = formatBuffer + BUFF_OFFSET;

			bool bNegative = false;
			if (dec.getValue() < 0)
			{
				bNegative = true;
			}

			if (dec.getIntPart() < 1000)
				replaceThousand = false;

			if (replaceThousand)
			{
				int nLen = prepareDecimal(pTmp, dec.getIntPart(), dec.getDecPart(), bNegative, NLEN, trailingTxt);

				if (nLen < 0)
					return nLen;

				int nCio = nLen;

				if (NLEN > 0)
					nCio -= (NLEN + 1);

				int nToSkip = 0;

				if (bNegative)
				{
					nCio--;
					nToSkip = 1;
				}

				nToSkip += nCio % 3;
				int nTh = (nCio - 1) / 3;
				nLen += nTh;
				replaceThSeps(pTmp, formatBuffer, nTh, nToSkip, bNegative);
				return nLen;
			}
			return prepareDecimal(formatBuffer, dec.getIntPart(), dec.getDecPart(), bNegative, NLEN, trailingTxt);
		}


		/// @brief Formats a td::Variant value into the internal format buffer.
		/// @param val       Variant value to format.
		/// @param nDec      Number of decimal places for floating-point variants.
		/// @param showThSep If true, thousand separators are inserted.
		/// @param fmt       Floating-point display format.
		/// @return Number of characters written.
        int format(const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal)
		{
			return mu::formatVariant(this, val, nDec, showThSep, fmt);
		}

		/// @brief Formats a td::Variant value with an optional trailing text into the internal format buffer.
		/// @param val         Variant value to format.
		/// @param nDec        Number of decimal places for floating-point variants.
		/// @param showThSep   If true, thousand separators are inserted.
		/// @param fmt         Floating-point display format.
		/// @param trailingTxt Optional text appended after the formatted value.
		/// @return Number of characters written.
        int formatWithTrailing(const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
        {
            return mu::formatVariant(this, val, nDec, showThSep, fmt, trailingTxt);
        }

		/// @brief Formats an RGBA color value as a '#' prefixed 8-digit hexadecimal string.
		/// @param val 32-bit RGBA color value to format.
		/// @return Pointer to the internal format buffer containing the formatted color string.
		const char* formatColor(td::UINT4 val)
		{
			formatBuffer[0]='#';

			int pos = 1;
			//for each 4 bits
			while (pos <= 8)
			{
				td::UINT4 rest = val & 0xF0000000;
				rest >>= 28;
				if ( rest <= 9 )
				{
					formatBuffer[pos] = '0' + (char) rest;
				}
				else
				{
					formatBuffer[pos] = 'a' + (char) (rest - 10);
				}
				val <<= 4;
				++pos;
			}
			formatBuffer[pos] = '\0';
			return formatBuffer;
		}

		/// @brief Returns a const pointer to the current contents of the internal format buffer.
		/// @return Const pointer to the null-terminated format buffer.
		inline const char* getBuffer() const
		{
			return formatBuffer;
		}

		/// @brief Returns a mutable pointer to the internal format buffer.
		/// @return Pointer to the format buffer.
		inline char* getBuffer()
		{
			return formatBuffer;
		}

		/// @brief Returns a const pointer to the current contents of the internal format buffer.
		/// @return Const pointer to the null-terminated format buffer.
		inline const char* c_str() const
		{
			return formatBuffer;
		}

		/// @brief Converts the UTF-8 content currently in the format buffer to UTF-16 and appends it after the UTF-8 data.
		/// @param inputLen Number of UTF-8 bytes in the format buffer to convert.
		/// @return Pointer to the start of the newly written UTF-16 data within the format buffer, or nullptr on overflow.
		const td::UTF16* toUTF16(int inputLen)
		{
			const mu::UTF8* sourceStart = (const mu::UTF8*)(formatBuffer);
			const mu::UTF8* sourceEnd = sourceStart + inputLen;

			int bytesNeeded = mu::UTFConverter::measureUTF8toUTF16(sourceStart, sourceEnd);

			++inputLen;
			bytesNeeded += inputLen;
			if (bytesNeeded + 2 < _maxLen)
			{

				mu::UTF16* pBeg = (mu::UTF16*) (formatBuffer + inputLen);
				mu::UTF16* pEnd = (mu::UTF16*) (formatBuffer + bytesNeeded);
				mu::UTFConverter::convertUTF8toUTF16(sourceStart, sourceEnd, pBeg, pEnd);
				*pEnd = 0;
				return (td::UTF16*) pBeg;
			}
			return 0;
		}

		/// @brief Extracts only the numeric characters from a data buffer, normalising the decimal and thousand separators.
		/// @tparam NCHBUFF  Compile-time size of buff.
		/// @param pData     Input character data.
		/// @param nDataLen  Number of bytes in pData.
		/// @param dt        Expected data type of the numeric value.
		/// @param buff      Output buffer receiving the extracted number string.
		/// @param sci       If true, scientific notation is accepted.
		/// @return Tuple of (number of characters extracted, whether the operation succeeded).
        template <size_t NCHBUFF>
        std::tuple<size_t, bool> extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char (&buff)[NCHBUFF], bool sci= false)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::extractNumbers(pData, nDataLen, dt, buff, chDecPoint, thSep, sci);
        }

		/// @brief Extracts only the numeric characters from a data buffer into an explicitly sized output buffer.
		/// @param pData    Input character data.
		/// @param nDataLen Number of bytes in pData.
		/// @param dt       Expected data type of the numeric value.
		/// @param buff     Output buffer receiving the extracted number string.
		/// @param buffLen  Size of buff in bytes.
		/// @param sci      If true, scientific notation is accepted.
		/// @return true if the extraction succeeded, false otherwise.
        inline bool extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char* buff, size_t buffLen, bool sci= false)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::extractNumbers(pData, nDataLen, dt, buff, buffLen, chDecPoint, thSep, sci);
        }

		/// @brief Converts a locale-formatted number string to a standard C-locale number string using a fixed-size buffer.
		/// @tparam NBUFLEN Compile-time size of buff.
		/// @param pStrIn  Input locale-formatted number string.
		/// @param nLenIn  Number of characters in pStrIn.
		/// @param buff    Output buffer receiving the converted string.
		/// @return Number of characters written to buff.
        template <size_t NBUFLEN>
        inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char (&buff) [NBUFLEN])
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::toAnsiCNumber(pStrIn, nLenIn, buff, chDecPoint, thSep);
        }

		/// @brief Converts a locale-formatted number string to a standard C-locale number string.
		/// @param pStrIn  Input locale-formatted number string.
		/// @param nLenIn  Number of characters in pStrIn.
		/// @param buff    Output buffer receiving the converted string.
		/// @param buffLen Size of buff in bytes.
		/// @return Number of characters written to buff.
        inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char* buff, size_t buffLen)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::toAnsiCNumber(pStrIn, nLenIn, buff, buffLen, chDecPoint, thSep);
        }
	};
};


