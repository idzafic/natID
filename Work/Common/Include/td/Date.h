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
#include <string>

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


class Date
{
    friend class arch::ArchiveIn;
    friend class DateTime;
public:
    enum class Format : unsigned char { ShortDM = 0, ShortY2 = 1, ShortY4 = 2, WinShort = 3, WinLong = 4 };

    

protected:
    td::INT4 _date; //bits 0-3: day, 4-8: Month, 9-30: year, 31: if set then BC (negative)

    //td::INT4* getMonthDays(td::INT4 y) const
    //{
    //	static td::INT4 DaysPerMonthLeap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //	static td::INT4 DaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //
    //	//td::INT4* dpm = DaysPerMonth;

    //	if (y%4 == 0 && !(y%100 == 0 && y%400 != 0))
    //		return DaysPerMonthLeap;
    //	else
    //		return DaysPerMonth;
    //}

    void setValue(td::INT4 date)
    {
        _date = date;
    }

    inline bool leapYear(td::INT4 year) const
    {
        if (year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0))
            return true;

        return false;
    }

    inline td::INT4 DoomsdayCentury(td::INT4 century) const
    {
        switch( century % 400 )
        {
        case 0:
            return 2;
        case 100:
            return 0;
        case 200:
            return 5;
        case 300:
            return 3;
        }
        return -1;
    }

    inline td::INT4 DoomsdayMonth(td::INT4 month, td::INT4 year) const
    {
        switch(month)
        {
        case 1:
            if(leapYear(year))
                return 32;
            return 31;
        case 2:
            if(leapYear(year))
                return 29;
            return 28;
        case 3:
            return 7;
        case 4:
            return 4;
        case 5:
            return 9;
        case 6:
            return 6;
        case 7:
            return 11;
        case 8:
            return 8;
        case 9:
            return 5;
        case 10:
            return 10;
        case 11:
            return 7;
        case 12:
            return 12;
        }
        assert(0);
        return 1;
    }

    inline td::INT4 getNumberOfDays(td::INT4 year, td::INT4 month) const
    {
        if (month == 2)
        {
            if (year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0))
                return 29;
            else
                return 28;
        }

        if ( (month == 4) || (month == 6) || (month == 9) || (month == 11) )
            return 30;

        return 31;
    }
    
public:

    Date(td::INT4 year, td::INT4 month, td::INT4 day)
    {
        setDate(year, month, day);
    }

    Date(td::INT4 year, td::INT4 month, td::INT4 day, bool bc)
    {
        setDate(year, month, day);
        if (bc)
        {
            _date |= 0x80000000;
        }
    }

    Date(const Date& d)
        : _date(d.getValue())
    {
    }

    Date()
        : _date(0)
    {
    }

    Date(bool currentDT)
        : _date(0)
    {
        if (currentDT)
            now();
    }

    Date(td::INT4 dateValue)
        : _date(dateValue)
    {
    }

    void setDate(td::INT4 year, td::INT4 month, td::INT4 day)
    {
        if (month <1)
            month = 1;
        
        if (day < 1)
            day = 1;
        
        if (month > 12)
            month = 12;
        
        if (day > 31)
            day = 31;
        
        if (year < 1)
            year = 1;
        
        if (year > 4000)
            year = 4000;
        
        td::INT4 maxNoOfDays = getNumberOfDays(year, month);
        
        if (day > maxNoOfDays)
            day = maxNoOfDays;
        
        _date = day & 0x0000001F;
        _date |= ((month & 0x0000000F) << 5);
        _date |=  ((year & 0x003FFFFF) << 9);
    }

    void setBC(bool bc = true)
    {
        if (bc)
            _date |= 0x80000000;
        else
            _date = _date & 0x7FFFFFFF;
    }

    inline void fromString(const char* str)
    {
        _date = 0;
        if (str == 0)
            return;
        if (*str == 0)
            return;

        td::INT4 year = 1;
        td::INT4 month = 1;
        td::INT4 day = 1;
        SSCANF(str, "%d-%d-%d", &year, &month, &day);
        setDate(year, month, day);
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
        int nLen = SNPRINTF(buffer, bufLen, _TRUNCATE, "%04d-%02d-%02d", getYear(), getMonth(), getDay());
        if (nLen < 0)
        {
            buffer[bufLen-1] = 0;
        }
        return nLen;
    }

    template <size_t size>
    inline int formc_str(char(&buffer)[size]) const
    {
        static_assert(size >= 11, "Too small buffer size in Date::formc_str");
        return formc_str(&buffer[0], size);
    }

    td::StringBase<UTF8, td::EncodingUTF8, 4, false> toString() const;


    template <typename TUTIL>
    const char* c_str(TUTIL& u) const
    {
        return u.c_str(*this);
    }


    td::INT4 getValue() const
    {
        return _date;
    }
    //td::INT4 operator() const
    //{
    //	return _date;
    //}

    td::INT4 getDay() const
    {
        return 0x0000001F & _date;
    }

    td::INT4 getMonth() const
    {
        return ((0x000001E0 & _date) >> 5);
    }

    td::INT4 getYear() const
    {
        return ((0x3FFFFE00 & _date) >> 9);
    }


    int operator - (const td::Date& d)
    {
        td::INT4 yFR = getYear();
        td::INT4 yTO = d.getYear();

        if (yFR == yTO)
            return getNoOfDays() - d.getNoOfDays();

        td::INT4 totalDays = 0;
        if (leapYear(yFR))
            totalDays = 366 - getNoOfDays();
        else
            totalDays = 365 - getNoOfDays();

        for (td::INT4 year(yFR + 1); year < yTO; ++year)
        {
            if (leapYear(year))
                totalDays += 366;
            else
                totalDays += 365;

        }
        totalDays += d.getNoOfDays();
        return totalDays;
    }

    void operator += (td::INT4 nDays)
    {
        if (nDays > 0)
            addDays(nDays);
        else if (nDays < 0)
        {
            nDays *= -1;
            td::INT4 year = getYear();
            //td::INT4* dpm = getMonthDays(year);

            td::INT4 month = getMonth();

            td::INT4 day = getDay();

            //while (nDays > 0)
            td::INT4 remDays = 0;
            do
            {
                remDays = nDays - day;

                if (remDays > 0)
                {
                    if (month >	1)
                    {
                        --month;
                    }
                    else
                    {
                        --year;
                        month = 12;
                    }
                    nDays = remDays;
                    day = getNumberOfDays(year, month);
                }
                else
                {
                    day -= nDays;
                    if (day <= 0)
                    {
                        -- month;
                        if (month <= 0)
                        {
                            month = 12;
                            --year;
                        }
                        day = getNumberOfDays(year, month);
                    }
                }
            }while(remDays > 0);

            setDate(year, month, day);
        }
    }

    void operator -= (td::INT4 nDays)
    {
        operator += (-nDays);
    }

    td::Date operator + (int nDays)
    {
        td::Date d(*this);
        d += nDays;
        return d;
    }

    td::Date operator - (int nDays)
    {
        td::Date d(*this);
        d -= nDays;
        return d;
    }

    void addDays(td::INT4 nDays)
    {
        td::INT4 year = getYear();
        //td::INT4* dpm = getMonthDays(year);

        td::INT4 month = getMonth();

        td::INT4 day = getDay();

        while (nDays > 0)
        {
            td::INT4 monthDays = getNumberOfDays(year, month);
            td::INT4 remDays = monthDays - day + 1;

            if (nDays >= remDays)
            {
                day = 1;
                if (month < 12)
                {
                    month++;
                }
                else
                {
                    year ++;
                    month = 1;
                }
                nDays -= remDays;
            }
            else
            {
                day += nDays;
                nDays = 0;
            }
        }

        setDate(year, month, day);
    }
    
    bool operator == (const Date& dt) const
    {
        return (_date == dt._date);
    }

    bool operator != (const Date& dt) const
    {
        return (_date != dt._date);
    }

    bool operator < (const Date& dt) const
    {
        return (_date < dt._date);
    }

    bool operator <= (const Date& dt) const
    {
        return (_date <= dt._date);
    }

    bool operator > (const Date& dt) const
    {
        return (_date > dt._date);
    }

    bool operator >= (const Date& dt) const
    {
        return (_date >= dt._date);
    }

    inline bool BC() const
    {
        return ((0x80000000 & _date) != 0) ? true : false;
    }

    inline bool isUTC() const
    {
        return ((0x40000000 & _date) != 0) ? true : false;
    }

    inline bool isLocal() const
    {
        return !isUTC();
    }

    inline void setAsUTC(bool utc = true)
    {
        if (utc)
            _date |= 0x40000000;
        else
            _date &= ~(0x40000000);
    }

    Date& operator = (const Date& d)
    {
        _date = d.getValue();
        return *this;
    }

    Date& operator = (td::INT4 d)
    {
        _date = d;
        return *this;
    }

    template <class TVAR>
    inline Date& operator = (const TVAR& var)
    {
        Date& d = *this;
        var.getValue(d);
        return *this;
    }

    bool isNull() const
    {
        return _date == 0;
    }

    td::INT4 getNoOfDays() const
    {
        static td::INT4 total[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
        static td::INT4 leapTotal[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
        td::INT4 nDays = 0;
        if (leapYear(getYear()))
            nDays = leapTotal[getMonth() - 1] + getDay();
        else
            nDays = total[getMonth() - 1] + getDay();

        return nDays;
    }

    td::INT4 getNoOfDays_Old() const
    {
        td::INT4 year = getYear();

        td::INT4 month = getMonth();
        
        td::INT4 nDays = 0;

        for (int i=0; i < month - 1; i++)
        {
            nDays += getNumberOfDays(year, i+1);
        }

        nDays += getDay();

        return nDays;
    }

    void now()
    {
        td::BaseDate bd;
        mu::getCurrentLocalDate(bd);
        setDate(bd.year, bd.month, bd.day);
    }

    template <typename T>
    void fromBoost(T date)
    {
        td::INT4 year = date.year();
        td::INT4 month = date.month();
        td::INT4 day = date.day();
        setDate(year, month, day);
    }

    int getWeekDay() const
    {
        td::INT4 r = -1;
        td::INT4 x = 0, y = 0;
        td::INT4 ddcentury = -1;

        td::INT4 year = getYear();
        td::INT4 month = getMonth();
        td::INT4 day = getDay();

        td::INT4 ddmonth = DoomsdayMonth(month, year);

        if(BC()) // B.C
        {
            td::INT4 dd = -1;
            if( year > 699 )
            {
                dd = (year - (year % 700) + 701) - year;
            }
            else
            {
                dd = (year - (year % 28) + 29) - year;
            }
            if( dd > 0 )
            {
                ddcentury = (((dd - (dd % 100)) / 100) * 6) % 7;
                x = ((dd % 100) % 7) + ((dd % 100)/4) % 7;
                if( ddmonth > day )
                    y = ddmonth + day;
                else
                    y = day - ddmonth;
                y %= 7;
                x = ddcentury + x;
                x %= 7;
                r = (x+y) % 7;
            }
        }
        else // Gregorian Calendar
        {
            td::INT4 century = year - (year % 100);
            ddcentury = DoomsdayCentury(century);
            if( ddcentury < 0 )
                return 0;
            if( ddmonth < 0 )
                return 0;

            if( ddmonth > day)
            {
                r = (7 - ((ddmonth-day) % 7 ) + ddmonth);
            }
            else
            {
                r = day;
            }
            x = (r - ddmonth);
            x %= 7;
            y = ddcentury + (year - century) + (year - century)/4;
            y %= 7;
            r = (x+y) % 7;
        }
        //else // Julian Calendar
        //{
        //	ddcentury = (((year_ - (year_ % 100)) / 100) * 6)%7;
        //	x = ((year_%100)%7) + (int)floor((year_%100)/4)%7;
        //	if( ddmonth > day_ )
        //		y = ddmonth + day_;
        //	else
        //		y = day_ - ddmonth;
        //	y %= 7;
        //	x = ddcentury + x;
        //	x %= 7;
        //	r = (x+y)%7;
        //}
        //weekday = r;
        return r;
    }
protected:
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
        operator<<(std::basic_ostream<CharT, Traits>& os,  const Date& rhs)
    {
        char tmp[64];
        rhs.formc_str(tmp, 64);
        os  << tmp;
        return os;
    }
    
    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>&
        operator>>(std::basic_istream<CharT, Traits>& is, Date& rhs)
    {
        std::string str;
        is >> str;
        rhs.fromString(str.c_str());
        return is;
    }
};

MAINUTILS_API Date::Format dateFormatFromString(const char* fmt);
MAINUTILS_API const char* dateFormatToString(Date::Format fmt);

inline DataType getType(const td::Date&) {return td::date;}

}; //td
