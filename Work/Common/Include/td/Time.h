// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

	class Time
	{
		friend class arch::ArchiveIn;
		friend class DateTime;

	public:		
		
        enum class Format : unsigned char { ShortHMM = 0, ShortHMMSS = 1, ShortHMMSSu = 2, WinShort = 3, WinLong = 4} ;
		
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
		td::INT4 _time;

		void setValue(td::INT4 time)
		{
			_time = time;
		}

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
		Time(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 msec)
			: _time(calcFractions(hour, minute, sec, msec))
		{	
			
		}

		Time(const Time& t)
			: _time(t.getValue())
		{
		}

		Time(bool currentDT)
			: _time(0)
		{
			if (currentDT)
				now();
		}

		void setTime(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 msec)
		{			
			_time = calcFractions(hour, minute, sec, msec);
		}

		void setTime2(td::INT4 hour, td::INT4 minute, td::INT4 sec, td::INT4 fractions)
		{			
			_time = calcFractions2(hour, minute, sec, fractions);
		}

		Time(td::INT4 fractionsOfMiliseconds)
			: _time(fractionsOfMiliseconds)
		{
		}

		Time()
			: _time(0)
		{
		}

		td::INT4 getValue() const
		{
			return _time;
		}

		Time& operator = (const Time& t)
		{
			_time = t.getValue();
			return *this;
		}

		Time& operator = (td::INT4 t)
		{
			_time = t;
			return *this;
		}

		//for Variant 
		template <class TVAR>
		inline Time& operator = (const TVAR& var)
		{
			Time& t = *this;
			var.getValue(t);
			return *this;
		}

		bool operator == (const Time& t) const
		{
			return (_time == t._time);
		}

		bool operator != (const Time& t) const
		{
			return (_time != t._time);
		}

		bool operator < (const Time& t) const
		{
			return (_time < t._time);
		}

		bool operator <= (const Time& t) const
		{
			return (_time <= t._time);
		}

		bool operator > (const Time& t) const
		{
			return (_time > t._time);
		}

		bool operator >= (const Time& t) const
		{
			return (_time >= t._time);
		}

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
			int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%02d:%02d:%02d.%04d", getHour(), getMinute(), getSecond(), getFraction());
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
			static_assert(size >= 15, "Too small buffer size in Time::formc_str");
			return formc_str(&buffer[0], size);
		}

		td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;

		template <typename TUTIL>
		const char* c_str(TUTIL& u) const
		{
			return u.c_str(*this);
		}

		td::INT4 getHour() const
		{
			return _time / (60 * 60 * 10000);
		}

		td::INT4 getMinute() const
		{
			td::INT4 min = _time % (60 * 60 * 10000);
			return min / (60 * 10000);
		}

		td::INT4 getSecond() const
		{
			td::INT4 sec = _time % (60 * 10000);
			return sec / 10000;
		}

		td::INT4 getMilisecond() const
		{	
			td::INT4 lastDec = _time % 10;
			if (lastDec >= 5)
				return (((_time % 10000) / 10) + 1);
			
			return ((_time % 10000) / 10);
		}

		td::INT4 getFraction() const
		{			
			return _time % 10000;
		}

		void now()
		{
			td::BaseTime bt;
			mu::getCurrentLocalTime(bt);

			setTime2(bt.hour, bt.min, bt.sec, bt.usec / 100);
		}

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
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os,  const Time& rhs)
		{
			char tmp[64];
			rhs.formc_str(tmp, 64);
			os  << tmp;
			return os;
		}
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

	inline DataType getType(const td::Time&) {return td::time;}
	
};
