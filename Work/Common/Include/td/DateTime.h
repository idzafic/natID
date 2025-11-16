// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Date.h>
#include <td/Time.h>

namespace td
{
    class DateTime : public Time, public Date
	{
	public:		
		friend class arch::ArchiveIn;

	protected:
		void setDateTime(td::INT4 date, td::INT4 time)
		{
			Date::setValue(date);
			Time::setValue(time);
		}
	//	td::INT4 _date;
	//	td::INT4 _time;

	public:
		DateTime(const Date& date, td::INT4 fractionsOfSecond)
		    : Time(fractionsOfSecond)
		    , Date(date)
		{
		}

		DateTime(td::INT4 year, td::INT4 month, td::INT4 day, td::INT4 hour, td::INT4 minute, td::INT4 second, td::INT4 milisecond)	
		    : Time(hour, minute, second, milisecond)
		    , Date(year, month, day)
		{			
		}

		DateTime(const Date& d, const Time& t)
		    : Time(t)
		    , Date(d)
		{
		}

		DateTime(const Date& d)
		    : Time(0)
		    , Date(d)
		{
		}

		DateTime(bool useCurrentDate, bool useCurrentTime)
		    : Time(0)
		    , Date(0)
		{
			if (useCurrentDate)
				Date::now();
			if (useCurrentTime)
				Time::now();
		}

		DateTime(const DateTime& dt)
		    : Time(dt.getTime())
		    , Date(dt.getDate())
		{
		}

		DateTime()
		    : Time((td::INT4) 0)
		    , Date((td::INT4) 0)
		{
		}

		DateTime& operator = (const DateTime& dt)
		{
			Date::setValue(dt.getDate());
			Time::setValue(dt.getTime());
			return *this;
		}

		template <class TVAR>
		inline DateTime& operator = (const TVAR& var)
		{
			DateTime& dt = *this;
			var.getValue(dt);
			return *this;
		}
		

		bool operator == (const DateTime& t) const
		{
			return (_time == t._time && _date == t._date);
		}

		bool operator != (const DateTime& t) const
		{
			return (_time != t._time || _date != t._date);
		}

		//returns duration (t2 - t1)
		void calcDuration(const DateTime& t2, const DateTime t1)
		{
			td::Date d2(t2.getDate());
			td::Date d1(t1.getDate());
			td::INT4  dayDiff(d2 - d1);			
			td::INT4 timeDiff(t2.getTime() - t1.getTime()); 
			if (timeDiff < 0)
			{
				if (dayDiff > 0)
				{
					--dayDiff;
					timeDiff += 24 * 60 * 60 * 10000;
				}
			}
			Date dRet(dayDiff, 0, 0);
			setDateTime(dRet.getValue(), timeDiff);			
		}

		template <class TSTREAM>
		TSTREAM& showAsDuration(TSTREAM& s)
		{
			td::Time time(getTime());
			s << getDay() << " days, " << time << "\n";
			return s;
		}

		bool operator < (const DateTime& t) const
		{
			if (_date != t._date)
				return _date < t._date;
			return _time < t._time;
		}

		bool operator <= (const DateTime& t) const
		{
			if (_date != t._date)
				return _date < t._date;
			return _time <= t._time;
		}

		bool operator > (const DateTime& t) const
		{
			if (_date != t._date)
				return _date > t._date;
			return _time > t._time;
		}

		bool operator >= (const DateTime& t) const
		{
			if (_date != t._date)
				return _date > t._date;
			return _time >= t._time;
		}
        
        const DateTime& operator += (const Time& t)
        {
            auto res = _time + t._time;
            const td::INT4 maxVal = 24*60*60*10000;
            if (res >= maxVal)
            {
                res -= maxVal;
                _time = res;
                addDays(1);
            }
            _time = res;
            return *this;
        }

		void now()
		{
			td::BaseDate bd;
			td::BaseTime bt;
			mu::getCurrentDateTime(bd, bt, false);
			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec/100);
			setAsUTC(false);
		}

		void nowUTC()
		{
			td::BaseDate bd;
			td::BaseTime bt;
			mu::getCurrentDateTime(bd, bt, true);
			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec/100);
			setAsUTC(true);
		}
		
		void toUTC()
		{
			if (isUTC())
				return;

			td::BaseDate bd;
			
			bd.year = td::UINT2(Date::getYear());
			bd.month = td::BYTE(Date::getMonth());
			bd.day = td::BYTE(Date::getDay());

			td::BaseTime bt;
			bt.hour = td::BYTE(Time::getHour());
			bt.min = td::BYTE(Time::getMinute());
			bt.sec = td::BYTE(Time::getSecond());
			bt.usec = td::UINT2(Time::getFraction()) * 100;

			mu::convertTime(bd, bt, true);

			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec / 100);
			setAsUTC(true);
		}

		void toLocal()
		{
			if (isLocal())
				return;

			td::BaseDate bd;

			bd.year = td::UINT2(Date::getYear());
			bd.month = td::BYTE(Date::getMonth());
			bd.day = td::BYTE(Date::getDay());

			td::BaseTime bt;
			bt.hour = td::BYTE(Time::getHour());
			bt.min = td::BYTE(Time::getMinute());
			bt.sec = td::BYTE(Time::getSecond());
			bt.usec = td::UINT2(Time::getFraction()) * 100;

			mu::convertTime(bd, bt, false);

			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec / 100); 
			setAsUTC(false);
		}

		template <typename T>
		void fromBoost(T dateTime)
		{			
			//boost::gregorian::date date = dateTime.date();
			auto date = dateTime.date();

			td::INT4 year = date.year();
			td::INT4 month = date.month();
			td::INT4 day = date.day();
                        Date::setDate(year, month, day);

			//boost::posix_time::time_duration t = dateTime.time_of_day();
			auto t = dateTime.time_of_day();

			td::INT4 hour = t.hours();
			td::INT4 minute = t.minutes();
			td::INT4 second = t.seconds();
			td::INT4 milisecond = (td::INT4) t.fractional_seconds() / 1000;
                        Time::setTime(hour, minute, second, milisecond);
		}

		void fromString(const char* str)
		{
			if (str == 0)
			{
				_date = 0;
				_time = 0;
				return;
			}
			
			if (*str == 0)
			{
				_date = 0;
				_time = 0;
				return;
			}

			td::INT4 year = 1;
			td::INT4 month = 1;
			td::INT4 day = 1;

			td::INT4 hour = 0;
			td::INT4 minute = 0;
			td::INT4 sec = 0;
			td::INT4 fraction = 0;
			
			SSCANF(str, "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &sec, &fraction);
			Date::setDate(year, month, day);
			setTime2(hour, minute, sec, fraction);
		}
        
        void clean()
        {
            Date::setValue(0);
            Time::setValue(0);
        }

		void setValueFromString(const char* str)
		{
			fromString(str);
		}

		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		int formc_str(char* buffer, int bufLen) const
		{
			int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d.%04d", getYear(), getMonth(), getDay(), getHour(), getMinute(), getSecond(), getFraction());
			if (nLen < 0)
			{
				buffer[bufLen-1] = 0;
				return bufLen -1;
			}
			return nLen;
		}

		template <size_t size>  
		inline int formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 25, "Too small buffer size in DateTime::formc_str");
			return formc_str(&buffer[0], size);
		}

		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		int formc_strWoMS(char* buffer, int bufLen) const
		{
			int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d", getYear(), getMonth(), getDay(), getHour(), getMinute(), getSecond());
			if (nLen < 0)
			{
				buffer[bufLen - 1] = 0;
				return bufLen - 1;
			}
			return nLen;
		}

		template <size_t size>
		inline int formc_strWoMS(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in DateTime::formc_strWoMS");
			return formc_strWoMS(&buffer[0], size);
		}

		int simple_formc_str(char* buffer, int bufLen) const
		{
			int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%04d%02d%02d-%02d%02d%02d%04d", getYear(), getMonth(), getDay(), getHour(), getMinute(), getSecond(), getFraction());
			if (nLen < 0)
			{
				buffer[bufLen - 1] = 0;
				return bufLen - 1;
			}
			return nLen;
		}

		template <size_t size>
		inline int simple_formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in DateTime::simple_formc_str");
			return simple_formc_str(&buffer[0], size);
		}

		template <size_t size>
		inline int c_str(char(&buffer)[size]) const
		{
			static_assert(size >= 25, "Too small buffer size in DateTime::c_str");
			return formc_str(&buffer[0], size);
		}

		template <typename TUTIL>
		const char* c_str(TUTIL& u) const
		{
			return u.c_str(*this);
		}

		//const char* toString() const
		//{
		//	char buffer[32];
		//	int nLen = SNPRINTF(buffer, 32, _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d.%04d", getYear(), getMonth(), getDay(), getHour(), getMinute(), getSecond(), getFraction());
		//	if (nLen < 0)
		//	{
		//		buffer[31] = 0;				
		//	}
		//	return buffer;
		//}
		//DateTime(td::INT4 date, td::INT4 miliseconds)
		//	: _date(date)
		//	, _time(miliseconds)
		//{
		//}

		//DateTime(td::INT4 year, td::INT4 month, td::INT4 day, td::INT4 hour, td::INT4 minute, td::INT4 second, td::INT4 milisecond)			
		//{
		//	Date d(year,month, day);
		//	_date = d.getValue();
		//	Time t(hour, minute, second, milisecond);
		//	_time = t.getValue();
		//}

		//DateTime(const Date& d, const Time& t)
		//	: _date(d.getValue())
		//	, _time(t.getValue())
		//{
		//}

		td::INT4 getDate() const
		{
			return Date::getValue();
		}

		td::INT4 getTime() const
		{
			return Time::getValue();
		}


		void setDate(td::INT4 year, td::INT4 month, td::INT4 day)
		{
			Date::setDate(year, month, day);
		}

		void setDate(const td::Date& d)
		{
			Date::operator= (d);
		}

		void setTime(const td::Time& t)
		{
			Time::operator= (t);
		}

		td::LINT8 getValue() const
		{
			td::LINT8 toRet = getDate();
			toRet = toRet << 32;
			toRet = toRet | Time::getValue();
			return toRet;
		}

		void setValue(td::LINT8 val)
		{
			td::INT4 time = (td::INT4) (val & 0xFFFFFFFF);
			td::INT4 date = (td::INT4) (val >> 32);
			Date::setValue(date);
			Time::setValue(time);
		}

		void getValue(Date& d) const
		{			
			d.setValue(getDate());
		}

		void getValue(Time& t) const
		{
			t.setValue(getTime());
		}

	protected:
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os,  const DateTime& rhs)
		{
			char tmp[64];
			rhs.formc_str(tmp, 64);
			os  << tmp;
			return os;
		}
	};

	//class DBDate : public Date
	//{
	//public:
	//	DBDate(const Date& d)
	//		: Date(d)
	//	{
	//	}
	//};

	//class DBTime : public Time
	//{
	//public:
	//	DBTime(const Time& t)
	//		: Time(t)
	//	{
	//	}
	//};

	//class DBDateTime : public Date, public Time
	//{
	//public:
	//	DBDateTime(const DateTime& dt)
	//		: Date(dt.getDate())
	//		, Time(dt.getTime())
	//	{
	//	}
	//};

	inline DataType getType(const td::DateTime&) {return td::dateTime;}
	

	//inline DataType getType(const td::DBDate&) {return td::dbDate;}
	//inline DataType getType(const td::DBTime&) {return td::dbTime;}
	//inline DataType getType(const td::DBDateTime&) {return td::dbDateTime;}
	
};
