// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Time.h
    @brief Defines the Time class for storing and manipulating time-of-day values with sub-millisecond precision. */
#pragma once
#include <td/Types.h>
#include <mu/mu.h>
//#include <math.h>
#include <cassert>

namespace arch
{
	class ArchiveIn;
}

namespace td
{
	//class Variant;
	class DateTime;

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	class StringBase; // < T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>;

	/// @brief Stores a time-of-day value with 100-microsecond precision as a single INT4.
	class Time
	{
		friend class arch::ArchiveIn;
		friend class DateTime;

	public:

        /// @brief Enumerates the available string display formats for a Time value.
        enum class Format : unsigned char {
            ShortHMM      = 0, ///< Short format showing only hours and minutes (H:MM).
            ShortHMMSS    = 1, ///< Short format showing hours, minutes, and seconds (H:MM:SS).
            ShortHMMSSu   = 2, ///< Short format with fractional seconds (H:MM:SS.ffff).
            WinShort      = 3, ///< Windows locale short time format.
            WinLong       = 4  ///< Windows locale long time format.
        };

		/// @brief Parses a Format value from its string name.
		/// @param fmt Null-terminated string name of the format.
		/// @return The matching Format enum value, defaulting to WinLong if unrecognised.
		static Format formatFromString(const char* fmt)
		{
			if (strcmp(fmt, "TimeOwnShortHMM") == 0)
				return Format::ShortHMM;
			else if (strcmp(fmt, "TimeOwnShortHMMSS") == 0)
				return Format::ShortHMMSS;
			else if (strcmp(fmt, "TimeOwnShortHMMSSu") == 0)
				return Format::ShortHMMSSu;
			else if (strcmp(fmt, "TimeWinShort") == 0)
				return Format::WinShort;
			else if (strcmp(fmt, "TimeWinLong") == 0)
				return Format::WinLong;
			return Format::WinLong;
		}

		/// @brief Converts a Format value to its canonical string name.
		/// @param fmt The Format enum value.
		/// @return A null-terminated string naming the format.
		static const char* formatToString(Format fmt)
		{
			if (fmt == Format::ShortHMM)
				return "TimeOwnShortHMM";
			else if (fmt == Format::ShortHMMSS)
				return "TimeOwnShortHMMSS";
			else if (fmt == Format::ShortHMMSSu)
				return "TimeOwnShortHMMSSu";
			else if (fmt == Format::WinShort)
				return "TimeWinShort";
			else if (fmt == Format::WinLong)
				return "TimeWinLong";
			return "TimeWinShort";
		}

	protected:
		td::INT4 _time; ///< Internal time storage: total 100-microsecond fractions since midnight.

		/// @brief Sets the raw internal time value directly.
		/// @param time Raw fractions value to store.
		void setValue(td::INT4 time)
		{
			_time = time;
		}

		/// @brief Computes the packed fractions value from hour, minute, second, and millisecond components.
		/// @param hour   Hour component (0–23).
		/// @param minute Minute component (0–59).
		/// @param sec    Second component (0–59).
		/// @param msec   Millisecond component (0–999).
		/// @return Packed fractions value (units of 100 microseconds).
		inline td::INT4 calcFractions(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 msec) const
		{
            if (hour < 0)
                hour =0;
            if (hour > 23)
                hour = 23;
            if (minute < 0)
                minute = 0;
            if (minute > 59)
                minute = 59;
            if (sec < 0)
                sec = 0;
            if (sec > 59)
                sec = 59;

			//time precision 100microSeconds
			assert(hour >= 0 && hour < 24);
			assert(minute >= 0 && minute < 60);
			assert(sec >= 0 && sec < 60);
			assert(msec >= 0 && msec < 1000);
			return (((hour * 60 + minute) * 60  + sec) * 1000 + msec)*10;
		}

		/// @brief Computes the packed fractions value from hour, minute, second, and sub-millisecond fractions.
		/// @param hour      Hour component (0–23).
		/// @param minute    Minute component (0–59).
		/// @param sec       Second component (0–59).
		/// @param fractions Sub-millisecond fractions (0–9999, units of 100 microseconds).
		/// @return Packed fractions value.
		inline td::INT4 calcFractions2(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 fractions) const
		{
            if (hour < 0)
                hour = 0;
            if (hour > 23)
                hour = 23;
            if (minute < 0)
                minute = 0;
            if (minute > 59)
                minute = 59;
            if (sec < 0)
                sec = 0;
            if (sec > 59)
                sec = 59;

			//time precision 100microSeconds
			assert(hour >= 0 && hour < 24);
			assert(minute >= 0 && minute < 60);
			assert(sec >= 0 && sec < 60);
			assert(fractions >= 0 && fractions < 10000);
			return ((hour * 60 + minute) * 60  + sec) * 10000 + fractions;
		}

	public:
		/// @brief Constructs a Time from explicit hour, minute, second, and millisecond components.
		/// @param hour   Hour (0–23).
		/// @param minute Minute (0–59).
		/// @param sec    Second (0–59).
		/// @param msec   Millisecond (0–999).
		Time(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 msec)
			: _time(calcFractions(hour, minute, sec, msec))
		{

		}

		/// @brief Copy constructor.
		/// @param t The Time to copy from.
		Time(const Time& t)
			: _time(t.getValue())
		{
		}

		/// @brief Constructs a Time optionally initialised to the current local time.
		/// @param currentDT If true, sets the value to the current local time.
		Time(bool currentDT)
			: _time(0)
		{
			if (currentDT)
				now();
		}

		/// @brief Sets the time from explicit hour, minute, second, and millisecond components.
		/// @param hour   Hour (0–23).
		/// @param minute Minute (0–59).
		/// @param sec    Second (0–59).
		/// @param msec   Millisecond (0–999).
		void setTime(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 msec)
		{
			_time = calcFractions(hour, minute, sec, msec);
		}

		/// @brief Sets the time from explicit hour, minute, second, and sub-millisecond fraction components.
		/// @param hour      Hour (0–23).
		/// @param minute    Minute (0–59).
		/// @param sec       Second (0–59).
		/// @param fractions Sub-millisecond fractions (0–9999).
		void setTime2(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 fractions)
		{
			_time = calcFractions2(hour, minute, sec, fractions);
		}

		/// @brief Constructs a Time directly from the raw internal fractions value.
		/// @param fractionsOfMiliseconds Raw packed fractions value.
		Time(td::INT4 fractionsOfMiliseconds)
			: _time(fractionsOfMiliseconds)
		{
		}

		/// @brief Default constructor; initialises the time to midnight (00:00:00.0000).
		Time()
			: _time(0)
		{
		}

		/// @brief Returns the raw internal fractions value.
		/// @return The packed integer representing the time.
		td::INT4 getValue() const
		{
			return _time;
		}

		/// @brief Assigns from another Time.
		/// @param t The source Time.
		/// @return Reference to this Time.
		Time& operator = (const Time& t)
		{
			_time = t.getValue();
			return *this;
		}

		/// @brief Assigns from a raw fractions integer.
		/// @param t The raw packed fractions value.
		/// @return Reference to this Time.
		Time& operator = (td::INT4 t)
		{
			_time = t;
			return *this;
		}

		//for Variant
		/// @brief Assigns from a Variant value.
		/// @tparam TVAR Variant type that provides a getValue(Time&) member.
		/// @param var The source Variant.
		/// @return Reference to this Time.
		template <class TVAR>
		inline Time& operator = (const TVAR& var)
		{
			Time& t = *this;
			var.getValue(t);
			return *this;
		}

		/// @brief Tests equality with another Time.
		/// @param t The Time to compare against.
		/// @return true if both represent the same instant.
		bool operator == (const Time& t) const
		{
			return (_time == t._time);
		}

		/// @brief Tests inequality with another Time.
		/// @param t The Time to compare against.
		/// @return true if the two Times differ.
		bool operator != (const Time& t) const
		{
			return (_time != t._time);
		}

		/// @brief Less-than comparison.
		/// @param t The Time to compare against.
		/// @return true if this Time is earlier than t.
		bool operator < (const Time& t) const
		{
			return (_time < t._time);
		}

		/// @brief Less-than-or-equal comparison.
		/// @param t The Time to compare against.
		/// @return true if this Time is earlier than or equal to t.
		bool operator <= (const Time& t) const
		{
			return (_time <= t._time);
		}

		/// @brief Greater-than comparison.
		/// @param t The Time to compare against.
		/// @return true if this Time is later than t.
		bool operator > (const Time& t) const
		{
			return (_time > t._time);
		}

		/// @brief Greater-than-or-equal comparison.
		/// @param t The Time to compare against.
		/// @return true if this Time is later than or equal to t.
		bool operator >= (const Time& t) const
		{
			return (_time >= t._time);
		}

        /// @brief Adds another Time to this one, wrapping around midnight if necessary.
        /// @param t The Time duration to add.
        /// @return true if the result wrapped past midnight.
        bool operator += (const Time& t)
        {
            auto res = _time + t._time;
            const td::INT4 maxVal = 24*60*60*10000;
            if (res >= maxVal)
            {
                res -= maxVal;
                _time = res;
                return true;
            }
            _time = res;
            return false;
        }

		/// @brief Parses a time string in "HH:MM:SS.ffff" format.
		/// @param str Null-terminated input string.
		void fromString(const char* str)
		{
			_time = 0;
			if (str == 0)
				return;
			if (*str == 0)
				return;

			td::INT4 hour = 0;
			td::INT4 minute = 0;
			td::INT4 sec = 0;
			td::INT4 fraction = 0;
			SSCANF(str, "%d:%d:%d.%d", &hour, &minute, &sec, &fraction);

			setTime2(hour, minute, sec, fraction);
		}

		/// @brief Sets the time by parsing a "HH:MM:SS.ffff" formatted C-string.
		/// @param str Null-terminated input string.
		void setValueFromString(const char* str)
		{
			fromString(str);
		}

		/// @brief Parses a time from a StringBase.
		/// @tparam T_CHAR Character type of the string.
		/// @tparam Encoding String encoding identifier.
		/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
		/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
		/// @param str The source string to parse.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		/// @brief Sets the time value by parsing a StringBase.
		/// @tparam T_CHAR Character type of the string.
		/// @tparam Encoding String encoding identifier.
		/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
		/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
		/// @param str The source string to parse.
		template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
		void setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str);

		/// @brief Formats the time as "HH:MM:SS.ffff" into a caller-supplied buffer.
		/// @param buffer Destination character buffer.
		/// @param bufLen Size of the destination buffer in bytes.
		/// @return Number of characters written, or bufLen-1 on truncation.
		int formc_str(char* buffer, int bufLen) const
		{
			int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%02d:%02d:%02d.%04d", getHour(), getMinute(), getSecond(), getFraction());
			if (nLen < 0)
			{
				buffer[bufLen-1] = 0;
				return bufLen -1;
			}
			return nLen;
		}

		/// @brief Formats the time into a fixed-size character array.
		/// @tparam size The size of the destination array (must be >= 15).
		/// @param buffer Destination character array.
		/// @return Number of characters written.
		template <size_t size>
		inline int formc_str(char(&buffer)[size]) const
		{
			static_assert(size >= 15, "Too small buffer size in Time::formc_str");
			return formc_str(&buffer[0], size);
		}

		/// @brief Converts the time to a td::String.
		/// @return String representation in "HH:MM:SS.ffff" format.
		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		/// @brief Formats the time using a utility object's c_str method.
		/// @tparam TUTIL Utility type providing a c_str(const Time&) member.
		/// @param u The utility object.
		/// @return Null-terminated formatted string.
		template <typename TUTIL>
		const char* c_str(TUTIL& u) const
		{
			return u.c_str(*this);
		}

		/// @brief Returns the hour component of the time.
		/// @return Hour (0–23).
		td::INT4 getHour() const
		{
			return _time / (60 * 60 * 10000);
		}

		/// @brief Returns the minute component of the time.
		/// @return Minute (0–59).
		td::INT4 getMinute() const
		{
			td::INT4 min = _time % (60 * 60 * 10000);
			return min / (60 * 10000);
		}

		/// @brief Returns the second component of the time.
		/// @return Second (0–59).
		td::INT4 getSecond() const
		{
			td::INT4 sec = _time % (60 * 10000);
			return sec / 10000;
		}

		/// @brief Returns the millisecond component of the time.
		/// @return Millisecond (0–999), rounded from sub-millisecond fractions.
		td::INT4 getMilisecond() const
		{
			td::INT4 lastDec = _time % 10;
			if (lastDec >= 5)
				return (((_time % 10000) / 10) + 1);

			return ((_time % 10000) / 10);
		}

		/// @brief Returns the raw sub-millisecond fraction component of the time.
		/// @return Fraction in units of 100 microseconds (0–9999).
		td::INT4 getFraction() const
		{
			return _time % 10000;
		}

		/// @brief Sets this Time to the current local time.
		void now()
		{
			td::BaseTime bt;
			mu::getCurrentLocalTime(bt);

			setTime2(bt.hour, bt.min, bt.sec, bt.usec / 100);
		}

		/// @brief Sets this Time from a Boost.DateTime time-of-day value.
		/// @tparam T Boost time type providing time_of_day().
		/// @param time The Boost time value to convert from.
		template <typename T>
		void fromBoost(T time)
		{
			//boost::posix_time::time_duration t = time.time_of_day();
			auto t = time.time_of_day();

			td::INT4 hour = t.hours();
			td::INT4 minute = t.minutes();
			td::INT4 second = t.seconds();
			td::INT4 fraction = (td::INT4) t.fractional_seconds() / 100;
			setTime2(hour, minute, second, fraction);
		}
	protected:
		/// @brief Stream insertion operator.
		/// @tparam CharT Character type of the stream.
		/// @tparam Traits Traits type of the stream.
		/// @param os The output stream.
		/// @param rhs The Time to write.
		/// @return The output stream.
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os,  const Time& rhs)
		{
			char tmp[64];
			rhs.formc_str(tmp, 64);
			os  << tmp;
			return os;
		}
        /// @brief Stream extraction operator; parses a time from a stream.
        /// @tparam CharT Character type of the stream.
        /// @tparam Traits Traits type of the stream.
        /// @param is The input stream.
        /// @param rhs The Time to populate.
        /// @return The input stream.
        template<class CharT, class Traits>
        friend std::basic_istream<CharT, Traits>&
            operator>>(std::basic_istream<CharT, Traits>& is, Time& rhs)
        {
            std::string str;
            is >> str;
            rhs.fromString(str.c_str());
            return is;
        }
	};

	/// @brief Returns the DataType identifier for a Time value.
	/// @return td::time.
	inline DataType getType(const td::Time&) {return td::time;}

};
