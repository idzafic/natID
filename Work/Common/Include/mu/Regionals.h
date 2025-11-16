// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

MAINUTILS_API int formatVariant(Regionals* pReg, const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr);

	const char _h = 'h';
	const char _H = 'H';
	const char _s = 's';
	const char _S = 'S';

	const char _t = 't';
	const char _u = 'u';

	const char _d = 'd';
	const char _D = 'D';

	const char _m = 'm';
	const char _M = 'M';

	const char _y = 'y';
	const char _Y = 'Y';
	const char _quot = '\'';

	const int BUFF_OFFSET = 32;

	class Regionals
	{
	protected:
		typedef enum {Year= 0, YearShort = 1, Month = 2, MonthName = 3, MonthShortName = 4, Day = 5, DayName = 6, DayShortName = 7, DateSeparator = 8} tDatePart;
		typedef enum {Hour12= 0, Hour24 = 1, Minute = 2, Second = 3, Milisecond = 4, AMPM = 5, TimeSeparator = 6} tTimePart;
	
	protected:
        td::PascalString<1>    locName;
		int _maxLen;
		td::UINT4 _locID;
        float maxDblFormat;
        float minDblFormat;
        float maxFltFormat;
        float minFltFormat;
        int _fmtBuffLen;
        char decPoint;
        char thousandSeparator;
        char dateInputSeparator;
        char dateInputFormat[3];

		typedef struct _tDateFmt
		{
			tDatePart part;
			td::PascalString<0> format;
		} tDatePartFmt;

		typedef struct _tTimeFmt
		{
			tTimePart part;
			td::PascalString<0> format;
		} tTimePartFmt;
		
		
	public:
		char* formatBuffer;
	protected:		
		td::PascalString<1>	bc;
		td::PascalString<1>	am;
		td::PascalString<1>	pm;

		cnt::PushBackVector<tDatePartFmt>	dateFormatWinLong;
		cnt::PushBackVector<tDatePartFmt>	dateFormatWinShort;
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortY4;
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortY2;
		cnt::PushBackVector<tDatePartFmt>	dateFormatOwnShortDM;

		cnt::PushBackVector<tTimePartFmt>	timeFormatWinLong;
		cnt::PushBackVector<tTimePartFmt>	timeFormatWinShort;
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMMSSu;
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMMSS;
		cnt::PushBackVector<tTimePartFmt>	timeFormatOwnShortHMM;

		cnt::Array< td::PascalString<1>, 12>	monthNames;
		cnt::Array< td::PascalString<1>, 12>	monthShortNames;

		cnt::Array< td::PascalString<1>, 7>		dayNames;
		cnt::Array< td::PascalString<1>, 7>		dayShortNames;
public:

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

	char getDecPoint() const
	{
		return decPoint;
	}

	char getThousandSeparator() const
	{
		return thousandSeparator;
	}

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
        mu::toDouble(formatBuffer);
	}

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

	const char* getLocName() const
	{
		return locName.c_str();
	}
	
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

		~Regionals()
		{
			delete [] formatBuffer;
		}

		inline td::UINT4 getID() const
		{
			return _locID;
		}

	protected:		

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

		void reserve(char** pDest, int nLen)
		{
			if (*pDest)
			{
				delete [] *pDest;
			}
			*pDest = new char[nLen + 1];
		}

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

		template <typename T>
        inline int convertFloat(char* pBuff, T val, int nDec, int BUFFLEN, td::FormatFloat fmt= td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
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

		void setBCString(const char* strBC)
		{
			bc = strBC;
		}

		int getBuffLen() const
		{
			return _maxLen;
		}

		void setMonthNames (const td::PascalString<1> names[12])
		{
			for (int i=0; i<12; i++)
			{
				monthNames[i] = names[i];
			}
		}

		void setDayNames (const td::PascalString<1> names[7])
		{
			for (int i=0; i<7; i++)
			{
				dayNames[i] = names[i];
			}
		}

		void setMonthName(int i, const char* name)
		{
			assert( (i >= 0) && (i < 12) );
			monthNames[i] = name;
		}

		void setMonthShortName(int i, const char* shortName)
		{
			assert( (i >= 0) && (i < 12) );
			monthShortNames[i] = shortName;
		}

		void setDayName(int i, const char* name)
		{
			assert( (i >= 0) && (i < 7) );
			dayNames[i] = name;
		}

		void setDayShortName(int i, const char* shortName)
		{
			assert( (i >= 0) && (i < 7) );
			dayShortNames[i] = shortName;
		}

		template <typename T>
        int formatFloat(T val, int nDec, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
			return formatFloat(formatBuffer, formatBuffer + BUFF_OFFSET, val, nDec, _maxLen - BUFF_OFFSET, setThSeparator, fmt, trailingTxt);
		}

		template <typename T>
		int formatFloat(const char* label, int labelLen, T val, int nDec, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
			STRCPY(formatBuffer, _fmtBuffLen, label);
			return formatFloat(formatBuffer + labelLen, formatBuffer + BUFF_OFFSET + labelLen, val, nDec, _maxLen - BUFF_OFFSET - labelLen, setThSeparator, fmt, trailingTxt) + labelLen;
		}

		template <typename T>
		int formatFloat(T val, int nDec, const char* postLabel, int postLabelLen, bool setThSeparator = true, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
		{
            int nLen = formatFloat(formatBuffer, formatBuffer + BUFF_OFFSET, val, nDec, _maxLen - BUFF_OFFSET, setThSeparator, fmt, trailingTxt);
			STRCPY(formatBuffer + nLen, _fmtBuffLen - nLen, postLabel);
			return nLen + postLabelLen;
		}

		template <typename T>
		int formatInt(T val, const char* pFormat, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, pFormat, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
		}

		template <typename T>
		int formatInt(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%d%s", _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%d", _maxLen - BUFF_OFFSET, setThSeparator);
		}

		template <typename T>
		int formatUInt(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%u%s", _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, "%u", _maxLen - BUFF_OFFSET, setThSeparator);
		}

		template <typename T>
		int formatLINT(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LINT8, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LINT8, _maxLen - BUFF_OFFSET, setThSeparator);
		}

		template <typename T>
		int formatLUINT(T val, bool setThSeparator = true, const char* trailingTxt = nullptr)
		{
            if (trailingTxt != nullptr)
                return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LUINT8, _maxLen - BUFF_OFFSET, setThSeparator, trailingTxt);
			return formatInt(formatBuffer, formatBuffer + BUFF_OFFSET, val, FMT_LUINT8, _maxLen - BUFF_OFFSET, setThSeparator);
		}

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

		template <typename TFloat>
		inline int uniformFltBuild5(TFloat val1, TFloat val2, TFloat val3, TFloat val4, TFloat val5, int nDec, bool setThSeparator = true)
		{
			return buildFloat5<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, val5, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline int uniformFltBuild4(TFloat val1, TFloat val2, TFloat val3, TFloat val4, int nDec, bool setThSeparator= true)
		{
			return buildFloat3<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, val4, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline int uniformFltBuild3(TFloat val1, TFloat val2, TFloat val3, int nDec, bool setThSeparator= true)
		{
			return buildFloat3<TFloat, '\n'>(val1, nDec, val2, nDec, val3, nDec, setThSeparator);
		}

		template <typename TFloat>
		inline int uniformFltBuild2(TFloat val1, TFloat val2, int nDec, bool setThSeparator= true)
		{
			return buildFloat2<TFloat, '\n'>(val1, nDec, val2, nDec, setThSeparator);
		}

    
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
        
        inline int formatInputDate(const td::Date& date, bool putZeros)
        {
            return formatInputDate(formatBuffer, _maxLen, date, putZeros);
        }
        
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
        
        int formatInputTime(char* buff, int buffLen, const td::Time& time, bool putLeadingZeros)
        {
            int nLen = 0;
            if (putLeadingZeros)
                nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%02d:%02d:%02d.%04d", time.getHour(), time.getMinute(), time.getSecond(), time.getFraction());
            else
                nLen = SNPRINTF(buff, buffLen, _TRUNCATE, "%d:%d:%d.%04d", time.getHour(), time.getMinute(), time.getSecond(), time.getFraction());
                
            return nLen;
        }
        
        int formatInputTime(const td::Time& time, bool putLeadingZeros)
        {
            return formatInputTime(formatBuffer, _maxLen, time, putLeadingZeros);
        }
        
        inline td::Date getInputDate()
        {
            return getInputDate(formatBuffer);
        }
        
        inline int format(const td::Color& color)
        {
            return color.formc_str(formatBuffer, _maxLen);
        }
        
		inline int format(td::Time::Format tf, const td::Time& t)
		{
			return format(tf, t, formatBuffer, _maxLen);
		}

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
        
 
        int format(const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal)
		{
			return mu::formatVariant(this, val, nDec, showThSep, fmt);
		}
        
        int formatWithTrailing(const td::Variant& val, int nDec, bool showThSep, td::FormatFloat fmt = td::FormatFloat::Decimal, const char* trailingTxt = nullptr)
        {
            return mu::formatVariant(this, val, nDec, showThSep, fmt, trailingTxt);
        }

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

		inline const char* getBuffer() const
		{
			return formatBuffer;
		}

		inline char* getBuffer()
		{
			return formatBuffer;
		}

		inline const char* c_str() const
		{
			return formatBuffer;
		}

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
        
        template <size_t NCHBUFF>
        std::tuple<size_t, bool> extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char (&buff)[NCHBUFF], bool sci= false)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::extractNumbers(pData, nDataLen, dt, buff, chDecPoint, thSep, sci);
        }
        
        inline bool extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char* buff, size_t buffLen, bool sci= false)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::extractNumbers(pData, nDataLen, dt, buff, buffLen, chDecPoint, thSep, sci);
        }
        
         template <size_t NBUFLEN>
        inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char (&buff) [NBUFLEN])
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::toAnsiCNumber(pStrIn, nLenIn, buff, chDecPoint, thSep);
        }
        
        inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char* buff, size_t buffLen)
        {
            char chDecPoint = getDecPoint();
            char thSep = getThousandSeparator();
            return td::toAnsiCNumber(pStrIn, nLenIn, buff, buffLen, chDecPoint, thSep);
        }
	};	
};


