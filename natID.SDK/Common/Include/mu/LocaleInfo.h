// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#ifdef USE_BOOST_LOCALES
#include <boost/locale.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#endif
#include <stdexcept>  // for runtime_error
#include <mu/Utils.h>
#include <cnt/Array.h>
#include <td/String.h>
#include <gui/adv/Types.h>
//#include "db/ISQlDataProvider.h"

//class LocaleInfo reads locale information and provide localized names for month, date, thousand separator, decimal point and currency symbol.

namespace mu
{
	class LocaleInfo
	{

	protected:
		//time_t _Now;		
		td::UINT4 _lcid;
		cnt::Array< td::String, 7> _vecShortDayNames;
		cnt::Array< td::String, 7> _vecLongDayNames;
		cnt::Array< td::String, 12> _vecShortMonthNames;
		cnt::Array <td::String, 12> _vecLongMonthNames;
		td::String _localeString;
		td::String _localeRealName;
		td::String _thousandSeparator;
		td::String _decimalPoint;
		td::String _localMonetarySymbol;
		td::String _internationalMonetarySymbol;

		td::String _trueName;
		td::String _falseName;
		
		td::String _timeShortFmt;
		td::String _timeLongFmt;
		td::String _am;
		td::String _pm;
		td::String _bc;
		td::String _timeSep;

		td::String _dateShortFmt;
		td::String _dateLongFmt;
		td::String _dateSep;
		qtu::HAlignment _stringDefHAlign;
	public:
		LocaleInfo()	
			: _lcid(0)
			, _stringDefHAlign(qtu::Left)
			, _bc("BC")
		{
			_vecShortDayNames.clean();
			_vecLongDayNames.clean();
			_vecShortMonthNames.clean();
			_vecLongMonthNames.clean();

		}

		//: _Now(0 + 24*3600*3), // first day is Sunday 
		LocaleInfo(const char* localeString, const char* realName = "")			
			:_lcid(0)
			,_localeString(localeString ? localeString : "")
			, _localeRealName(realName)
			, _stringDefHAlign(qtu::Left)
			, _bc("BC")
		{			
			_vecShortDayNames.clean();
			_vecLongDayNames.clean();
			_vecShortMonthNames.clean();
			_vecLongMonthNames.clean();
		}

		bool readLocaleInfo()
		{
			// TODO use return values.
			createCalenderNames();
			readCurrencySymbol();
			readThousandSeparator();
			readDecimalPoint();
			readBooleanValueNames();
			return true;
		}

		const char* getLongDayName(size_t idx) const
		{
			if (idx < 7)
			{
				return _vecLongDayNames[idx].c_str();
			}
			return "";
		}

		const char* getShortDayName(size_t idx) const
		{
			if (idx < 7)
			{
				return _vecShortDayNames[idx].c_str();
			}
			return "";
		}

		const char* getLongMonthName(size_t idx) const
		{
			if (idx < 12)
			{
				return _vecLongMonthNames[idx].c_str();
			}
			return "";
		}

		const char* getShortMonthName(size_t idx) const
		{
			if (idx < 12)
			{
				return _vecShortMonthNames[idx].c_str();
			}
			return "";
		}

		const char getThousandSeparator() const
		{
			if (_thousandSeparator.length() > 0)
				return _thousandSeparator.getAt(0);
			return ' ';
		}

		const char getDecimalPoint() const
		{
			if (_decimalPoint.length() > 0)
				return _decimalPoint.getAt(0);
			return ' ';
		}

		const char* getLocalMonetarySymbol() const
		{
			return _localMonetarySymbol.c_str();
		}

		const char* getInternationalMonetarySymbol() const
		{
			return _internationalMonetarySymbol.c_str();
		}


		const char* getTrueName() const
		{
			return _trueName.c_str();
		}

		const char* getFalseName() const
		{
			return _falseName.c_str();
		}

		const char* getAM() const
		{
			return _am.c_str();
		}

		const char* getPM() const
		{
			return _pm.c_str();
		}

		const char* getBC() const
		{
			return _bc.c_str();
		}

		const char* getDateShortFmt() const
		{
			return _dateShortFmt.c_str();
		}

		const char* getDateLongFmt() const
		{
			return _dateLongFmt.c_str();
		}

		const char* getTimeShortFmt() const
		{
			return _timeShortFmt.c_str();
		}

		const char* getTimeLongFmt() const
		{
			return _timeLongFmt.c_str();
		}

		const char* getDateSep() const
		{
			return _dateSep.c_str();
		}

		const char* getTimeSep() const
		{
			return _timeSep.c_str();
		}

		const char* getLocaleName() const
		{
			return _localeString.c_str();
		}
		

		bool operator == (const LocaleInfo& li)
		{
			if (_lcid != li._lcid)
				return false;
			return true;
		}

		//OLD IMPLEMENENTATION USING C++ std::locale
		template <class TDB>
		bool storeToDB(TDB* pDB, const char* timeSep, const char* dateSep, const char* timeShortFmt, const char* timeLongFmt, const char* dateShortFmt, const char* dateLongFmt)
		{			

			td::UINT4 langID = mu::Utils::calcHashNo(_localeString.c_str());

			//INSERT Lang entry
			{

				td::String strTimeSep(timeSep);
				td::String strDateSep(dateSep);
				td::String strTimeShormFmt(timeShortFmt);
				td::String strTimeLongFmt(timeLongFmt);
				td::String strDateShormFmt(dateShortFmt);
				td::String strDateLongFmt(dateLongFmt);

				db::IStatementPtr pStLang(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO Lang (ID, Name, RealName, DecPoint, ThSep, DateSep, TimeSep, DateShortFmt, DateLongFmt, TimeShortFmt, TimeLongFmt, TrueDesc, FalseDesc, StringDefHAlign) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)"));
				td::WORD defAlign = (td::WORD) qtu::Left;
				db::Ref<td::String> rLS(16, td::ch);
				db::Ref<td::String> rLRN(128);
				db::Ref<td::String> rDC(1, td::chFix);
				db::Ref<td::String> rthSep(1, td::chFix);
				db::Ref<td::String> rTN(16);
				db::Ref<td::String> rFN(16);

				db::Ref<td::String> rDateSep(1, td::chFix);
				db::Ref<td::String> rTimeSep(1, td::chFix);				
				db::Ref<td::String> rDateSF(16, td::ch);
				db::Ref<td::String> rDateLF(32, td::ch);
				db::Ref<td::String> rTimeSF(16, td::ch);
				db::Ref<td::String> rTimeLF(32, td::ch);
				

				db::Params parLang(pStLang->allocParams());

				parLang << langID
					<< rLS
					<< rLRN
					<< rDC
					<< rthSep
					<< rDateSep
					<< rTimeSep
					<< rDateSF
					<< rDateLF
					<< rTimeSF
					<< rTimeLF
					<< rTN
					<< rFN
					<< defAlign;

				rLS = _localeString;
				rLRN = _localeRealName;
				rDC = _decimalPoint;
				rthSep = _thousandSeparator;
				rTN = _trueName;
				rFN = _falseName;
				rTimeSep = strTimeSep;
				rDateSep = strDateSep;
				rTimeSF = strTimeShormFmt;
				rTimeLF = strTimeLongFmt;
				rDateSF = strDateShormFmt;
				rDateLF = strDateLongFmt;

				if (!pStLang->execute())
				{
					td::String strErr;
					pStLang->getErrorStr(strErr);
					return false;
				}
			}


			//Insert Lang Days entries

			{
				db::Ref<td::String> refSN(12);
				db::Ref<td::String> refLN(64);
				td::WORD dayID;
				db::IStatementPtr pStLangDays(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO LangDays (LangID, DayID, ShortName, LongName) VALUES (?,?,?,?)"));
				db::Params parLangDays(pStLangDays->allocParams());
				parLangDays << langID
					<< dayID
					<< refSN
					<< refLN;
				for (dayID = 0; dayID < 7; ++dayID)
				{
					refSN = _vecShortDayNames[dayID];
					refLN = _vecLongDayNames[dayID];
					if (!pStLangDays->execute())
						return false;
				}
			}
			

			//Insert Lang months entries
			{
				db::Ref<td::String> refSN(12);
				db::Ref<td::String> refLN(64);
				td::WORD monthID;
				db::IStatementPtr pStLangMonths(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO LangMonths (LangID, MonthID, ShortName, LongName) VALUES (?,?,?,?)"));
				db::Params parLangMonths(pStLangMonths->allocParams());
				parLangMonths << langID
					<< monthID
					<< refSN
					<< refLN;
				for (int iMonth = 0; iMonth < 12; ++iMonth)
				{					
					refSN = _vecShortMonthNames[iMonth];
					refLN = _vecLongMonthNames[iMonth];
					monthID = iMonth + 1;
					if (!pStLangMonths->execute())
						return false;
				}
			}
			return true;
		}

		template <class TDB>
		bool loadFromDB(TDB* pDB, td::UINT4 langID)
		{		
			//load lang data		
			{
				db::IStatementPtr pStLang(pDB->createStatement(db::IStatement::DBS_SELECT, "SELECT * FROM Lang Where ID=?"));
				db::Params parLang(pStLang->allocParams());
				parLang << langID;
				td::WORD stringDefHAlign;
				db::Columns cols(pStLang->allocBindColumns(19));
				cols << "Name" << _localeString
					<< "RealName" << _localeRealName
					<< "DecPoint" << _decimalPoint
					<< "ThSep" << _thousandSeparator
					<< "DateSep" << _dateSep
					<< "TimeSep" << _timeSep
					<< "DateShortFmt" << _dateShortFmt
					<< "DateLongFmt" << _dateLongFmt
					<< "TimeShortFmt" << _timeShortFmt
					<< "TimeLongFmt" << _timeLongFmt
					<< "AM" << _am
					<< "PM" << _pm
					<< "BC" << _bc
					<< "TrueDesc" << _trueName
					<< "FalseDesc" << _falseName
					<< "StringDefHAlign" << stringDefHAlign
					<< "LCID" << _lcid
					<< "LocalMonetSymb" << _localMonetarySymbol
					<< "IntMonetSymb" << _internationalMonetarySymbol;

				if (!pStLang->execute())
				{
					return false;
				}

				if (!pStLang->moveNext())
					return false;

				_stringDefHAlign = (qtu::HAlignment) stringDefHAlign;
			}
			
			//load day data
			{
				db::IStatementPtr pStLangDays(pDB->createStatement(db::IStatement::DBS_SELECT, "SELECT * FROM LangDays WHERE LangID=? ORDER BY DayID"));
				db::Params parLangDays(pStLangDays->allocParams());
				parLangDays << langID;
				db::Columns cols(pStLangDays->allocBindColumns(3));
				td::WORD dayID;
				db::Ref<td::String> refSN(_vecShortDayNames[0]);
				db::Ref<td::String> refLN(_vecLongDayNames[0]);
				//td::String shortName;
				//td::String longName;

				cols << "DayID" << dayID << "ShortName" << refSN << "LongName" << refLN;

				if (!pStLangDays->execute())
				{
					return false;
				}
				int iDay = 0;
				while (pStLangDays->moveNext())
				{
					if (iDay != dayID)
						return false;

					if (++iDay < 7)
					{
						refSN = _vecShortDayNames[iDay];
						refLN = _vecLongDayNames[iDay];
					}					
				}				

				if (iDay != 7)
					return false;
			}

			//load month data
			{
				db::IStatementPtr pStLangMonths(pDB->createStatement(db::IStatement::DBS_SELECT, "SELECT * FROM LangMonths WHERE LangID=? ORDER BY MonthID"));
				db::Params parLangDays(pStLangMonths->allocParams());
				parLangDays << langID;
				db::Columns cols(pStLangMonths->allocBindColumns(3));
				td::WORD monthID;
				db::Ref<td::String> refSN(_vecShortMonthNames[0]);
				db::Ref<td::String> refLN(_vecLongMonthNames[0]);
				//td::String shortName;
				//td::String longName;

				cols << "MonthID" << monthID << "ShortName" << refSN << "LongName" << refLN;
				
				if (!pStLangMonths->execute())
				{
					return false;
				}
				int iMonth = 0;
				while (pStLangMonths->moveNext())
				{					
					if (iMonth != --monthID)
						return false;

					if (++iMonth < 12)
					{
						refSN = _vecShortMonthNames[iMonth];
						refLN = _vecLongMonthNames[iMonth];
					}
				}

				if (iMonth != 12)
					return false;
			}

			return true;
		}

		template <class TDB>
		bool loadFromDB(TDB* pDB)
		{
			td::UINT4 langID = mu::Utils::calcHashNo(_localeString.c_str());

			return loadFromDB(pDB, langID);
		}
		
		//new implementation using Windows Locale
		template <class TDB>
		bool storeToDB(TDB* pDB)
		{

			td::UINT4 langID = mu::Utils::calcHashNo(_localeString.c_str());

			//INSERT Lang entry
			{
				db::IStatementPtr pStLang(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO Lang (ID, Name, RealName, DecPoint, ThSep, DateSep, TimeSep, DateShortFmt, DateLongFmt, "
										"TimeShortFmt, TimeLongFmt, AM, PM, BC, TrueDesc, FalseDesc, StringDefHAlign, LCID, LocalMonetSymb, IntMonetSymb) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"));
				td::WORD defAlign = (td::WORD) (_stringDefHAlign);
				db::Ref<td::String> rLS(16, td::ch);
				db::Ref<td::String> rLRN(128);
				db::Ref<td::String> rDC(1, td::chFix);
				db::Ref<td::String> rthSep(1, td::chFix);
				db::Ref<td::String> rTN(16);
				db::Ref<td::String> rFN(16);

				db::Ref<td::String> rDateSep(1, td::chFix);
				db::Ref<td::String> rTimeSep(1, td::chFix);
				db::Ref<td::String> rDateSF(16, td::ch);
				db::Ref<td::String> rDateLF(32, td::ch);
				db::Ref<td::String> rTimeSF(16, td::ch);
				db::Ref<td::String> rTimeLF(32, td::ch);
				db::Ref<td::String> rAM(32);
				db::Ref<td::String> rPM(32);
				db::Ref<td::String> rBC(16);
				db::Ref<td::String> rLocMoney(8);
				db::Ref<td::String> rIntMoney(8);


				db::Params parLang(pStLang->allocParams());

				parLang << langID
					<< rLS
					<< rLRN
					<< rDC
					<< rthSep
					<< rDateSep
					<< rTimeSep
					<< rDateSF
					<< rDateLF
					<< rTimeSF
					<< rTimeLF
					<< rAM
					<< rPM
					<< rBC
					<< rTN
					<< rFN
					<< defAlign
					<< _lcid
					<< rLocMoney
					<< rIntMoney;

				rLS = _localeString;
				rLRN = _localeRealName;
				rDC = _decimalPoint;
				rthSep = _thousandSeparator;
				rTN = _trueName;
				rFN = _falseName;
				rTimeSep = _timeSep;
				rDateSep = _dateSep;
				rTimeSF = _timeShortFmt;
				rTimeLF = _timeLongFmt;
				rDateSF = _dateShortFmt;
				rDateLF = _dateLongFmt;
				rAM = _am;
				rPM = _pm;
				rBC = _bc;
				rLocMoney = _localMonetarySymbol;
				rIntMoney = _internationalMonetarySymbol;

				if (!pStLang->execute())
				{
					td::String strErr;
					pStLang->getErrorStr(strErr);
					return false;
				}
			}


			//Insert Lang Days entries

			{
				db::Ref<td::String> refSN(12);
				db::Ref<td::String> refLN(64);
				td::WORD dayID;
				db::IStatementPtr pStLangDays(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO LangDays (LangID, DayID, ShortName, LongName) VALUES (?,?,?,?)"));
				db::Params parLangDays(pStLangDays->allocParams());
				parLangDays << langID
					<< dayID
					<< refSN
					<< refLN;
				for (dayID = 0; dayID < 7; ++dayID)
				{
					refSN = _vecShortDayNames[dayID];
					refLN = _vecLongDayNames[dayID];
					if (!pStLangDays->execute())
						return false;
				}
			}


			//Insert Lang months entries
			{
				db::Ref<td::String> refSN(12);
				db::Ref<td::String> refLN(64);
				td::WORD monthID;
				db::IStatementPtr pStLangMonths(pDB->createStatement(db::IStatement::DBS_INSERT, "INSERT INTO LangMonths (LangID, MonthID, ShortName, LongName) VALUES (?,?,?,?)"));
				db::Params parLangMonths(pStLangMonths->allocParams());
				parLangMonths << langID
					<< monthID
					<< refSN
					<< refLN;
				for (int iMonth = 0; iMonth < 12; ++iMonth)
				{
					refSN = _vecShortMonthNames[iMonth];
					refLN = _vecLongMonthNames[iMonth];
					monthID = iMonth + 1;
					if (!pStLangMonths->execute())
						return false;
				}
			}
			return true;
		}

		template <class OFS>
		void printLocaleInfo(OFS& ofs)
		{
			//boost::locale::generator gen;
			//std::locale loc = gen(_localeString.c_str()); 
			//boost::gregorian::date_facet* ptrFacet1 = new boost::gregorian::date_facet("%x");
			//std::cout.imbue(std::locale(loc, ptrFacet1));

			////std::ofstream ofs(filename);
			//ofs.imbue(std::locale(loc, ptrFacet1));


			ofs << "LocaleString: " << _localeString << 
				 "\nReal Name   : " << _localeRealName << "\n\n";

			for(size_t idx=0; idx < 7; ++idx)
			{
				ofs << "Day: " << idx << " LongDayName: " << getLongDayName(idx) 
					<< " ShortDayName: " << getShortDayName(idx) << std::endl;
			}

			ofs << std::endl;

			for(size_t idx=0; idx < 12; ++idx)
			{
				ofs << "Month: " << idx << " LongMonthName: " << getLongMonthName(idx) 
					<< " ShortMonthName: " << getShortMonthName(idx) << std::endl;	
			}
			
			ofs << "\nLocalMonetarySymbol: " << getLocalMonetarySymbol();
			ofs << "\nInterMonetarySymbol: " << _internationalMonetarySymbol;
			ofs << "\nThousandSeparator  : " << getThousandSeparator();
			ofs << "\nDecimalPoint       : " << getDecimalPoint();
			ofs << "\nTrueName           : " << getTrueName();
			ofs << "\nFalseName          : " << _falseName;
			ofs << "\nTimeShortFmt       : " << _timeShortFmt;
			ofs << "\ntimeLongFmt        : " << _timeLongFmt;
			ofs << "\ntimeSep            : " << _timeSep;
			ofs << "\nAM                 : " << _am;
			ofs << "\nPM                 : " << _pm;
			ofs << "\nBC                 : " << _bc;
			ofs << "\ndateShortFmt       : " << _dateShortFmt;
			ofs << "\ndateLongFmt        : " << _dateLongFmt;
			ofs << "\ndateSep            : " << _dateSep;
			ofs << "\nstringDefHAlign    : " << _stringDefHAlign;
		}

	private:

		void getLocaleNameUTF8(td::String& localeName)
		{
			localeName = _localeString;
			if (localeName.find("_") > 0)
			{
				localeName += ".UTF8";
			}
		}
#ifdef USE_BOOST_LOCALEINFO
		void createCalenderNames()
		{
			td::String localeName;
			getLocaleNameUTF8(localeName);

			time_t now(0 + 24 * 3600 * 3);
			for(size_t idx=0; idx < 7; ++idx)
			{
				boost::locale::generator gen;
				std::locale loc = gen(localeName.c_str());
				boost::gregorian::date_facet* ptrFacet1 = new boost::gregorian::date_facet("%x");
				boost::gregorian::date_facet* ptrFacet2 = new boost::gregorian::date_facet("%x");

				std::ostringstream stream1;
				stream1.imbue(std::locale(loc, ptrFacet1));
				stream1 << boost::locale::as::ftime("%A") << (now + idx*(24*3600)) << std::endl;
				_vecLongDayNames[idx] =stream1.str();

				std::ostringstream stream2;
				stream2.imbue(std::locale(loc, ptrFacet2));
				stream2 << boost::locale::as::ftime("%a") << (now + idx*(24*3600)) << std::endl;
				//_vecShortDayNames.push_back(stream2.str());
				_vecShortDayNames[idx] = stream2.str();
			}

			for(size_t idx=0; idx < 12; ++idx)
			{
				boost::locale::generator gen;
				std::locale loc = gen(localeName.c_str());
				boost::gregorian::date_facet* ptrFacet1 = new boost::gregorian::date_facet("%x");
				boost::gregorian::date_facet* ptrFacet2 = new boost::gregorian::date_facet("%x");

				std::ostringstream stream1;
				stream1.imbue(std::locale(loc, ptrFacet1));
				stream1.imbue(std::locale(loc));
				stream1 << boost::locale::as::ftime("%B") << (now + idx*(24*3600)*31) << std::endl;
				//_vecLongMonthNames.push_back(stream1.str());
				_vecLongMonthNames[idx] = stream1.str();

				std::ostringstream stream2;
				stream2.imbue(std::locale(loc, ptrFacet2));
				stream2.imbue(std::locale(loc));
				stream2 << boost::locale::as::ftime("%b") << (now + idx*(24*3600)*31) << std::endl;
				//_vecShortMonthNames.push_back(stream2.str());
				_vecShortMonthNames[idx] = stream2.str();
			}
		}

		bool readCurrencySymbol()
		{
			td::String localeName;
			getLocaleNameUTF8(localeName);
			try 
			{
				boost::locale::generator gen;
				std::locale loc = gen(localeName.c_str());
				std::ostringstream stream1;
				boost::gregorian::date_facet* ptrFacet = new boost::gregorian::date_facet("%x");
				stream1.imbue(std::locale(loc, ptrFacet));
				stream1 << std::use_facet<std::moneypunct<char, true> >(loc).curr_symbol();
				_localMonetarySymbol = stream1.str();

				//std::ofstream ofs("testDecimal.txt");
				//ofs.imbue(std::locale(loc));
				//// obtain a moneypunct facet for the requested locale
				//const std::moneypunct<char>& mp = std::use_facet<std::moneypunct<char> >(loc);
				//ofs << "moneypunct in locale \"" << loc.name() << "\":\n";
				//ofs << "decimal_point: " << mp.decimal_point() << std::endl;
				//ofs << "thousands_sep: " << mp.thousands_sep() << std::endl;
				//ofs << "grouping: " << mp.grouping() << std::endl;
				//ofs << "curr_symbol: " << mp.curr_symbol() << std::endl;
				//ofs << "positive_sign: " << mp.positive_sign() << std::endl;
				//ofs << "negative_sign: " << mp.negative_sign() << std::endl;
				//ofs << "frac_digits: " << mp.frac_digits() << std::endl;
				//ofs << "pos_format: " << mp.pos_format() << std::endl;
				//ofs << "neg_format: " << mp.neg_format() << std::endl;

			}
			catch (std::runtime_error& e) 
			{
				// a runtime_error will be thrown if the locale cannot be constructed
				std::cerr << " Caught runtime_error: " << e.what() << '\n';
				return false;
			}
			catch (...) 
			{
				std::cerr << "Caught an unknown exception"; 
				return false;
			}
			return true;
		}

		void readThousandSeparator()
		{
			td::String localeName;
			getLocaleNameUTF8(localeName);
			boost::locale::generator gen;
			std::locale loc = gen(localeName.c_str());

			// obtain a numpunct facet for the requested locale
			const std::numpunct<char>& np =  std::use_facet<std::numpunct<char> >(loc);
			std::ostringstream stream1;
			boost::gregorian::date_facet* ptrFacet = new boost::gregorian::date_facet("%x");
			stream1.imbue(std::locale(loc, ptrFacet));
			stream1 << np.thousands_sep ();
			_thousandSeparator = stream1.str();
		}

		void readDecimalPoint()
		{
			td::String localeName;
			getLocaleNameUTF8(localeName);
			boost::locale::generator gen;
			std::locale loc = gen(localeName.c_str());

			// obtain a numpunct facet for the requested locale
			const std::numpunct<char>& np =  std::use_facet<std::numpunct<char> >(loc);
			std::ostringstream stream1;
			boost::gregorian::date_facet* ptrFacet = new boost::gregorian::date_facet("%x");
			stream1.imbue(std::locale(loc, ptrFacet));
			stream1 << np.decimal_point ();
			_decimalPoint = stream1.str();
		}

		void readBooleanValueNames()
		{
			td::String localeName;
			getLocaleNameUTF8(localeName);
			boost::locale::generator gen;
			std::locale loc = gen(localeName.c_str());

			{
				// obtain a numpunct facet for the requested locale
				const std::numpunct<char>& np =  std::use_facet<std::numpunct<char> >(loc);
				std::ostringstream stream1;
				boost::gregorian::date_facet* ptrFacet = new boost::gregorian::date_facet("%x");
				stream1.imbue(std::locale(loc, ptrFacet));
				stream1 << np.truename();
				_trueName = stream1.str();
			}

			{
				// obtain a numpunct facet for the requested locale
				const std::numpunct<char>& np =  std::use_facet<std::numpunct<char> >(loc);
				std::ostringstream stream1;
				boost::gregorian::date_facet* ptrFacet = new boost::gregorian::date_facet("%x");
				stream1.imbue(std::locale(loc, ptrFacet));
				stream1 << np.falsename();
				_falseName = stream1.str();
			}
		}
#else
void createCalenderNames()
{
	//need to be implemented without boost
	assert(false);
}

bool readCurrencySymbol()
{
	//need to be implemented without boost
	assert(false);
	return true;
}

void readThousandSeparator()
{
	//need to be implemented without boost
	assert(false);
}

void readDecimalPoint()
{
	//need to be implemented without boost
	assert(false);
}

void readBooleanValueNames()
{
	//need to be implemented without boost
	assert(false);
}

#endif

	};
}
  


//// working example for currency reading on Linux ...
///* setlocale example */
//#include <stdio.h>
//#include <time.h>
//#include <locale.h>
//
//int main ()
//{
//  time_t rawtime;
//  struct tm * timeinfo;
//  char buffer [80];
//
//  struct lconv * lc;
//
//  time ( &rawtime );
//  timeinfo = localtime ( &rawtime );
//
//  int twice=0;
//
//  do {
//      printf ("Locale is: %s\n", setlocale(LC_ALL, "en_US.UTF-8") );
//
//    strftime (buffer,80,"%c",timeinfo);
//    printf ("Date is: %s\n",buffer);
//
//    lc = localeconv ();
//    printf ("Currency symbol is: %s\n-\n",lc->currency_symbol);
//
//    setlocale (LC_ALL,"");
//  } while (!twice++);
//
//  return 0;
//}


