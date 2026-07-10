// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DateTime.h
    @brief Combined date and time value type that inherits from both Date and Time. */
#pragma once
#include <td/Date.h>
#include <td/Time.h>

namespace td
{
    /// @brief Represents a combined date and time point, inheriting storage and accessors from both Date and Time.
    class DateTime : public Time, public Date
	{
	public:
		friend class arch::ArchiveIn;

	protected:
		/// @brief Sets the raw date and time integer values directly.
		/// @param date Encoded date integer value.
		/// @param time Encoded time integer value (fractions of milliseconds).
		void setDateTime(td::INT4 date, td::INT4 time)
		{
			Date::setValue(date);
			Time::setValue(time);
		}
	//	td::INT4 _date;
	//	td::INT4 _time;

	public:
		/// @brief Constructs a DateTime from a Date and a raw fractions-of-second value.
		/// @param date The date component.
		/// @param fractionsOfSecond The time component expressed as fractions of a second (100-microsecond units).
		DateTime(const Date& date, td::INT4 fractionsOfSecond)
		    : Time(fractionsOfSecond)
		    , Date(date)
		{
		}

		/// @brief Constructs a DateTime from individual date and time components.
		/// @param year  The year.
		/// @param month The month (1-12).
		/// @param day   The day of the month (1-31).
		/// @param hour  The hour (0-23).
		/// @param minute The minute (0-59).
		/// @param second The second (0-59).
		/// @param milisecond The millisecond (0-999).
		DateTime(td::INT4 year, td::INT4 month, td::INT4 day, td::INT4 hour, td::INT4 minute, td::INT4 second, td::INT4 milisecond)
		    : Time(hour, minute, second, milisecond)
		    , Date(year, month, day)
		{
		}

		/// @brief Constructs a DateTime from separate Date and Time objects.
		/// @param d The date component.
		/// @param t The time component.
		DateTime(const Date& d, const Time& t)
		    : Time(t)
		    , Date(d)
		{
		}

		/// @brief Constructs a DateTime from a Date with the time set to midnight.
		/// @param d The date component.
		DateTime(const Date& d)
		    : Time(0)
		    , Date(d)
		{
		}

		/// @brief Constructs a DateTime optionally initialised to the current local date and/or time.
		/// @param useCurrentDate If true, sets the date to today.
		/// @param useCurrentTime If true, sets the time to the current local time.
		DateTime(bool useCurrentDate, bool useCurrentTime)
		    : Time(0)
		    , Date(0)
		{
			if (useCurrentDate)
				Date::now();
			if (useCurrentTime)
				Time::now();
		}

		/// @brief Copy constructor.
		/// @param dt The DateTime to copy.
		DateTime(const DateTime& dt)
		    : Time(dt.getTime())
		    , Date(dt.getDate())
		{
		}

		/// @brief Default constructor. Initialises to zero (epoch).
		DateTime()
		    : Time((td::INT4) 0)
		    , Date((td::INT4) 0)
		{
		}

		/// @brief Copy assignment operator.
		/// @param dt The DateTime to assign from.
		/// @return Reference to this object.
		DateTime& operator = (const DateTime& dt)
		{
			Date::setValue(dt.getDate());
			Time::setValue(dt.getTime());
			return *this;
		}

		/// @brief Assignment from a Variant-like object.
		/// @tparam TVAR Type that provides a getValue(DateTime&) overload.
		/// @param var The source variant.
		/// @return Reference to this object.
		template <class TVAR>
		inline DateTime& operator = (const TVAR& var)
		{
			DateTime& dt = *this;
			var.getValue(dt);
			return *this;
		}


		/// @brief Equality comparison.
		/// @param t The DateTime to compare with.
		/// @return True if both date and time are equal.
		bool operator == (const DateTime& t) const
		{
			return (_time == t._time && _date == t._date);
		}

		/// @brief Inequality comparison.
		/// @param t The DateTime to compare with.
		/// @return True if date or time differ.
		bool operator != (const DateTime& t) const
		{
			return (_time != t._time || _date != t._date);
		}

		//returns duration (t2 - t1)
		/// @brief Computes the duration between two DateTime values and stores the result in this object.
		/// @param t2 The end DateTime.
		/// @param t1 The start DateTime.
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

		/// @brief Streams the value as a human-readable duration string.
		/// @tparam TSTREAM Output stream type.
		/// @param s The output stream.
		/// @return Reference to the stream.
		template <class TSTREAM>
		TSTREAM& showAsDuration(TSTREAM& s)
		{
			td::Time time(getTime());
			s << getDay() << " days, " << time << "\n";
			return s;
		}

		/// @brief Less-than comparison.
		/// @param t The DateTime to compare with.
		/// @return True if this is earlier than t.
		bool operator < (const DateTime& t) const
		{
			if (_date != t._date)
				return _date < t._date;
			return _time < t._time;
		}

		/// @brief Less-than-or-equal comparison.
		/// @param t The DateTime to compare with.
		/// @return True if this is earlier than or equal to t.
		bool operator <= (const DateTime& t) const
		{
			if (_date != t._date)
				return _date < t._date;
			return _time <= t._time;
		}

		/// @brief Greater-than comparison.
		/// @param t The DateTime to compare with.
		/// @return True if this is later than t.
		bool operator > (const DateTime& t) const
		{
			if (_date != t._date)
				return _date > t._date;
			return _time > t._time;
		}

		/// @brief Greater-than-or-equal comparison.
		/// @param t The DateTime to compare with.
		/// @return True if this is later than or equal to t.
		bool operator >= (const DateTime& t) const
		{
			if (_date != t._date)
				return _date > t._date;
			return _time >= t._time;
		}

        /// @brief Adds a Time duration to this DateTime, advancing the date if the result passes midnight.
        /// @param t The time duration to add.
        /// @return Reference to this object.
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

		/// @brief Sets this DateTime to the current local date and time.
		void now()
		{
			td::BaseDate bd;
			td::BaseTime bt;
			mu::getCurrentDateTime(bd, bt, false);
			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec/100);
			setAsUTC(false);
		}

		/// @brief Sets this DateTime to the current UTC date and time.
		void nowUTC()
		{
			td::BaseDate bd;
			td::BaseTime bt;
			mu::getCurrentDateTime(bd, bt, true);
			setDate(bd.year, bd.month, bd.day);
			setTime2(bt.hour, bt.min, bt.sec, bt.usec/100);
			setAsUTC(true);
		}

		/// @brief Converts the stored local date/time to UTC in place.
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

		/// @brief Converts the stored UTC date/time to local time in place.
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

		/// @brief Initialises from a Boost ptime-compatible object.
		/// @tparam T A Boost ptime-like type providing date() and time_of_day() accessors.
		/// @param dateTime The Boost datetime value to convert from.
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

		/// @brief Parses a datetime from a C-string in "YYYY-MM-DD HH:MM:SS.ffff" format.
		/// @param str Null-terminated string to parse. If null or empty, the value is zeroed.
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

        /// @brief Resets both the date and the time to zero.
        void clean()
        {
            Date::setValue(0);
            Time::setValue(0);
        }

		/// @brief Sets the value by parsing a C-string (alias for fromString).
		/// @param str Null-terminated datetime string.
		void setValueFromString(const char* str)
		{
			fromString(str);
		}

		/// @brief Parses the datetime from a StringBase object.
		/// @tparam T_CHAR Character type of the string.
		/// @tparam Encoding String encoding tag.
		/// @tparam SPACE_FOR_SIZE Inline size storage.
		/// @tparam EXTERN_ALLOCATOR Whether an external allocator is used.
		/// @param str The source string.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		/// @brief Sets the value by parsing a StringBase object (alias for fromString).
		/// @tparam T_CHAR Character type of the string.
		/// @tparam Encoding String encoding tag.
		/// @tparam SPACE_FOR_SIZE Inline size storage.
		/// @tparam EXTERN_ALLOCATOR Whether an external allocator is used.
		/// @param str The source string.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		/// @brief Formats the datetime into a caller-supplied buffer including milliseconds.
		/// @param buffer Destination character buffer.
		/// @param bufLen Size of the buffer in bytes.
		/// @return Number of characters written (not including the null terminator), or bufLen-1 if truncated.
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

		/// @brief Formats the datetime into a fixed-size buffer including milliseconds.
		/// @tparam size Size of the array buffer (must be at least 25).
		/// @param buffer Destination character array.
		/// @return Number of characters written.
		template <size_t size>
		inline int formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 25, "Too small buffer size in DateTime::formc_str");
			return formc_str(&buffer[0], size);
		}

		/// @brief Returns a td::String representation of this DateTime.
		/// @return String in "YYYY-MM-DD HH:MM:SS.ffff" format.
		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		/// @brief Formats the datetime into a caller-supplied buffer without milliseconds.
		/// @param buffer Destination character buffer.
		/// @param bufLen Size of the buffer in bytes.
		/// @return Number of characters written, or bufLen-1 if truncated.
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

		/// @brief Formats the datetime into a fixed-size buffer without milliseconds.
		/// @tparam size Size of the array buffer (must be at least 20).
		/// @param buffer Destination character array.
		/// @return Number of characters written.
		template <size_t size>
		inline int formc_strWoMS(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in DateTime::formc_strWoMS");
			return formc_strWoMS(&buffer[0], size);
		}

		/// @brief Formats the datetime in a compact "YYYYMMDDHHMMSSffff" form without separators.
		/// @param buffer Destination character buffer.
		/// @param bufLen Size of the buffer in bytes.
		/// @return Number of characters written, or bufLen-1 if truncated.
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

		/// @brief Formats the datetime in compact form into a fixed-size buffer.
		/// @tparam size Size of the array buffer (must be at least 20).
		/// @param buffer Destination character array.
		/// @return Number of characters written.
		template <size_t size>
		inline int simple_formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 20, "Too small buffer size in DateTime::simple_formc_str");
			return simple_formc_str(&buffer[0], size);
		}

		/// @brief Formats the datetime into a fixed-size buffer (alias for formc_str).
		/// @tparam size Size of the array buffer (must be at least 25).
		/// @param buffer Destination character array.
		/// @return Number of characters written.
		template <size_t size>
		inline int c_str(char(&buffer)[size]) const
		{
			static_assert(size >= 25, "Too small buffer size in DateTime::c_str");
			return formc_str(&buffer[0], size);
		}

		/// @brief Formats the datetime using a utility helper object.
		/// @tparam TUTIL Utility type providing a c_str(const DateTime&) method.
		/// @param u The utility helper.
		/// @return Pointer to a null-terminated formatted string owned by u.
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

		/// @brief Returns the raw encoded date integer.
		/// @return Encoded date value.
		td::INT4 getDate() const
		{
			return Date::getValue();
		}

		/// @brief Returns the raw encoded time integer.
		/// @return Encoded time value (100-microsecond units since midnight).
		td::INT4 getTime() const
		{
			return Time::getValue();
		}


		/// @brief Sets the date component from individual year, month and day values.
		/// @param year  The year.
		/// @param month The month (1-12).
		/// @param day   The day of the month (1-31).
		void setDate(td::INT4 year, td::INT4 month, td::INT4 day)
		{
			Date::setDate(year, month, day);
		}

		/// @brief Sets the date component from a Date object.
		/// @param d The date to assign.
		void setDate(const td::Date& d)
		{
			Date::operator= (d);
		}

		/// @brief Sets the time component from a Time object.
		/// @param t The time to assign.
		void setTime(const td::Time& t)
		{
			Time::operator= (t);
		}

		/// @brief Returns the combined 64-bit value with date in the high 32 bits and time in the low 32 bits.
		/// @return 64-bit combined date+time value.
		td::LINT8 getValue() const
		{
			td::LINT8 toRet = getDate();
			toRet = toRet << 32;
			toRet = toRet | Time::getValue();
			return toRet;
		}

		/// @brief Sets the combined date and time from a 64-bit value.
		/// @param val 64-bit value with date in the high 32 bits and time in the low 32 bits.
		void setValue(td::LINT8 val)
		{
			td::INT4 time = (td::INT4) (val & 0xFFFFFFFF);
			td::INT4 date = (td::INT4) (val >> 32);
			Date::setValue(date);
			Time::setValue(time);
		}

		/// @brief Copies the date component into a Date object.
		/// @param d Output Date object to receive the value.
		void getValue(Date& d) const
		{
			d.setValue(getDate());
		}

		/// @brief Copies the time component into a Time object.
		/// @param t Output Time object to receive the value.
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

	/// @brief Returns the DataType tag for DateTime.
	/// @return td::dateTime
	inline DataType getType(const td::DateTime&) {return td::dateTime;}


	//inline DataType getType(const td::DBDate&) {return td::dbDate;}
	//inline DataType getType(const td::DBTime&) {return td::dbTime;}
	//inline DataType getType(const td::DBDateTime&) {return td::dbDateTime;}

};
