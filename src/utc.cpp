#include <ostream>
#include <date/date.h>
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <cassert>
#include "time/utc.hpp"

using namespace Time;

class UTC::UTCImpl
{
public:
    /// Sets a date based on the epochal time
    void updateEpoch(const double timeStamp)
    {
        // Figure out the fractional second 
        auto iUTCStamp = static_cast<int64_t> (timeStamp);
        auto fraction = timeStamp - static_cast<double> (iUTCStamp);
        mMicroSecond = static_cast<int> (std::lround(fraction*1.e6));
        // Create the epochal time
        std::chrono::seconds chronoSeconds(iUTCStamp);
        std::chrono::system_clock::time_point timePoint{chronoSeconds};
        // Set the year/month/day
        auto dayPoint = date::floor<date::days> (timePoint);
        date::year_month_day ymd{dayPoint};
        auto year = static_cast<int> (ymd.year());
        auto month = static_cast<int> (unsigned(ymd.month()));
        auto dom = static_cast<int> (unsigned(ymd.day()));
        updateYear(year);
        updateMonth(month);
        updateDayOfMonth(dom);
        // Update hour/minute/second        
        date::hh_mm_ss tod{timePoint - dayPoint};
        auto hour = static_cast<int> (tod.hours().count());
        auto minute = static_cast<int> (tod.minutes().count());
        auto second = static_cast<int> (tod.seconds().count());
        updateHour(hour);
        updateMinute(minute);
        updateSecond(second);
        // I have the epoch
        mEpoch = timeStamp;
        mHaveEpoch = true;
    } 
    /// Updates the year
    void updateYear(const int year)
    {
        mYear = date::year{year};
        mYMD = date::year_month_day{mYear, mMonth, mDay};
#ifndef NDEBUG
        assert(mYMD.ok());
#endif
        updateDayOfYear();
    }
    void updateMonth(const int month)
    {
        mMonth = date::month{static_cast<unsigned int> (month)};
        mYMD = date::year_month_day{mYear, mMonth, mDay};
#ifndef NDEBUG
        assert(mYMD.ok());
#endif
        updateDayOfYear();
    }
    void updateDayOfMonth(const int day)
    {
        mDay = date::day{static_cast<unsigned int> (day)};
        mYMD = date::year_month_day{mYear, mMonth, mDay}; 
#ifndef NDEBUG
        assert(mYMD.ok());
#endif
        updateDayOfYear();
    }
    // Updates the hour
    void updateHour(const int hour)
    {
        mHour = std::chrono::hours{hour};
        mHMS = date::hh_mm_ss<std::chrono::seconds> {mHour + mMinute + mSecond};
        mHaveEpoch = false;
    }
    // Updates the minute
    void updateMinute(const int minute)
    {
        mMinute = std::chrono::minutes{minute};
        mHMS = date::hh_mm_ss<std::chrono::seconds> {mHour + mMinute + mSecond};
        mHaveEpoch = false;
    }
    // Updates the second
    void updateSecond(const int second)
    {
        mSecond = std::chrono::seconds{second};
        mHMS = date::hh_mm_ss<std::chrono::seconds> {mHour + mMinute + mSecond};
        mHaveEpoch = false;
    }
    // Updates the microsecond
    void updateMicroSecond(const int muSec)
    {
        mMicroSecond = muSec;
        mHaveEpoch = false;
    }
    // Updates the day of the year
    void updateDayOfYear()
    {
        mStartOfYear = date::year_month_day{mYear, date::month{1}, date::day{1}};
#ifndef NDEBUG
        assert(mStartOfYear.ok());
#endif
        auto jday = date::sys_days{mYMD} - date::sys_days{mStartOfYear};
        mDayOfYear = static_cast<int> (jday.count()) + 1;
        mHaveEpoch = false;
    }
    // Update the the month and day of month given the day of the year
    void updateMonthAndDay(int doy)
    {
        mYMD = date::year_month_day{date::days{doy - 1}
                                  + date::sys_days{mStartOfYear}};
#ifndef NDEBUG
        assert(mYMD.ok());
#endif
        mMonth = mYMD.month();
        mDay   = mYMD.day();
        mDayOfYear = doy;
        mHaveEpoch = false;
    }

    date::year mYear{1970};
    date::month mMonth{1};
    date::day mDay{1};
    date::year_month_day mYMD{mYear, mMonth, mDay};
    date::year_month_day mStartOfYear{mYear, mMonth, mDay};
    std::chrono::hours mHour{0};
    std::chrono::minutes mMinute{0};
    std::chrono::seconds mSecond{0};
    date::hh_mm_ss<std::chrono::seconds> mHMS{mHour + mMinute + mSecond};
    double mEpoch = 0;
    int mDayOfYear = 1;
    int mMicroSecond = 0;
    bool mHaveEpoch = true;
};

/// C'tor
UTC::UTC() :
    pImpl(std::make_unique<UTCImpl> ())
{
}

UTC::UTC(const double epoch) :
    pImpl(std::make_unique<UTCImpl> ())
{
    setEpoch(epoch);
}

UTC::UTC(const std::string &time) :
    pImpl(std::make_unique<UTCImpl> ())
{
    int year;
    int month;
    int dom;
    int hour;
    int minute;
    int second;
    int microSecond = 0;
    if (time.size() == 26)
    {
        sscanf(time.c_str(),
               "%04d-%02d-%02dT%02d:%02d:%02d.%06d",
               &year, &month, &dom, &hour, &minute, &second, &microSecond); 
    }
    else if (time.size() == 19)
    {
        sscanf(time.c_str(),
               "%04d-%02d-%02dT%02d:%02d:%02d",
               &year, &month, &dom, &hour, &minute, &second); 
    }
    else
    {
        std::string errmsg = "Cannot parse " + time + " with length = "
                           + std::to_string(time.size());
        throw std::invalid_argument(errmsg);
    }
    UTC temp;
    temp.setYear(year);
    temp.setMonthAndDay(std::pair<int, int> (month, dom));
    temp.setHour(hour);
    temp.setMinute(minute);
    temp.setSecond(second);
    temp.setMicroSecond(microSecond);
    *this = std::move(temp);
}



/// Copy assignment
UTC::UTC(const UTC &time)
{
    *this = time;
}

/// Move assignment
UTC::UTC(UTC &&time) noexcept
{
    *this = std::move(time);
}

/// Copy assignment
UTC& UTC::operator=(const UTC &time)
{
    if (&time == this){return *this;}
    pImpl = std::make_unique<UTCImpl> (*time.pImpl);
    return *this;
}

/// Move assignment
UTC& UTC::operator=(UTC &&time) noexcept
{
    if (&time == this){return *this;}
    pImpl = std::move(time.pImpl);
    return *this;
} 

/// Destructor
UTC::~UTC() = default;

/// Reset class by reconstituting initial implementation 
void UTC::clear() noexcept
{
    pImpl = std::make_unique<UTCImpl> ();
}

/// Get epochal time
double UTC::getEpoch() const noexcept
{
    if (!pImpl->mHaveEpoch)
    {
        date::sys_days daysPassed{pImpl->mYMD};
        auto t = daysPassed.time_since_epoch() + pImpl->mHMS.to_duration();
        auto integralEpoch = static_cast<int64_t> (t.count());
        pImpl->mEpoch = integralEpoch + getMicroSecond()*1.e-6;
        pImpl->mHaveEpoch = true;
    }
    return pImpl->mEpoch;
}

void UTC::setEpoch(const double timeStamp) noexcept
{
    pImpl->updateEpoch(timeStamp);
}

/// Year
void UTC::setYear(const int year)
{
    if (year < -1000 || year > 2999)
    {
        throw std::invalid_argument("Year must be in range [-1000,2999]");
    }
    pImpl->updateYear(year);
}

int UTC::getYear() const noexcept
{
    return int(pImpl->mYear);
}

bool UTC::isLeapYear() const noexcept
{
    return pImpl->mYear.is_leap();
}

/// Month and day of month
void UTC::setMonthAndDay(const std::pair<int, int> &monthAndDay)
{
    auto month = monthAndDay.first;
    auto dayOfMonth = monthAndDay.second;
    if (month < 1 || month > 12) 
    {   
        throw std::invalid_argument("Month must be in range [1,12]");
    }
    if (dayOfMonth < 1 || dayOfMonth > 31)
    {
        throw std::invalid_argument("Day of month must be in range [1,31]");
    }
    pImpl->updateMonth(month);
    pImpl->updateDayOfMonth(dayOfMonth);
}

std::pair<int, int> UTC::getMonthAndDay() const noexcept
{
    return std::make_pair<int, int> (getMonth(), getDayOfMonth());
}

int UTC::getMonth() const noexcept
{
    return static_cast<int> (unsigned(pImpl->mMonth));
}

int UTC::getDayOfMonth() const noexcept
{
    return static_cast<int> (unsigned(pImpl->mDay));
}

/// Day of the year
void UTC::setDayOfYear(const int doy)
{
    if (isLeapYear())
    {
        if (doy < 1 || doy > 366)
        {
            throw std::invalid_argument("Day of year must be in range [1,366]");
        }
    }
    else
    {
        if (doy < 1 || doy > 365)
        {
            throw std::invalid_argument("Day of year must be in range [1,365]");
        }
    }
    pImpl->updateMonthAndDay(doy);
}

int UTC::getDayOfYear() const noexcept
{
    return pImpl->mDayOfYear;
}

/// Hour
void UTC::setHour(const int hour)
{
    if (hour < 0 || hour > 23)
    {
       throw std::invalid_argument("Hour must be in range [0,23]");
    }
    pImpl->updateHour(hour);
}

int UTC::getHour() const noexcept
{
    return static_cast<int> (pImpl->mHMS.hours().count());
}

/// Minute
void UTC::setMinute(const int minute)
{
    if (minute < 0 || minute > 59)
    {
        throw std::invalid_argument("Minute must be in range [0,59]");
    }
    pImpl->updateMinute(minute);
}

int UTC::getMinute() const noexcept
{
    return static_cast<int> (pImpl->mHMS.minutes().count());
}

/// Second
void UTC::setSecond(const int second)
{
    if (second < 0 || second > 59)
    {
        throw std::invalid_argument("Second must be in range [0,59]");
    }
    pImpl->updateSecond(second);
}

int UTC::getSecond() const noexcept
{
    return static_cast<int> (pImpl->mHMS.seconds().count());
} 

/// Microsecond
void UTC::setMicroSecond(const int muSec)
{
    if (muSec < 0 || muSec > 999999)
    {
        throw std::invalid_argument("Microsecond must be in range [0,999999]");
    }
    pImpl->mMicroSecond = muSec;
}

int UTC::getMicroSecond() const noexcept
{
    return pImpl->mMicroSecond;
}

/// Swap times
void Time::swap(UTC &lhs, UTC &rhs)
{
    std::swap(lhs.pImpl, rhs.pImpl);
}

/// Add times
UTC Time::operator+(const UTC &x, const UTC &y) 
{
    return x + y.getEpoch();
}

UTC Time::operator+(const UTC &x, const double y)
{
    auto t = x.getEpoch() + y;
    UTC tout(t);
    return tout;
}

/// Subtract times
UTC Time::operator-(const UTC &x, const UTC &y) 
{
    return x - y.getEpoch();
}

UTC Time::operator-(const UTC &x, const double y)
{
    auto t = x.getEpoch() - y;
    UTC tout(t);
    return tout;
}

/// lhs == rhs
bool Time::operator==(const UTC &lhs, const UTC &rhs)
{
    if (lhs.getYear()       != rhs.getYear()){return false;}
    if (lhs.getMonth()      != rhs.getMonth()){return false;}
    if (lhs.getDayOfYear()  != rhs.getDayOfYear()){return false;}
    if (lhs.getDayOfMonth() != rhs.getDayOfMonth()){return false;}
    if (lhs.getHour()       != rhs.getHour()){return false;}
    if (lhs.getMinute()     != rhs.getMinute()){return false;}
    if (lhs.getSecond()     != rhs.getSecond()){return false;}
    return lhs.getMicroSecond() == rhs.getMicroSecond();
}

/// lhs != rhs
bool Time::operator!=(const UTC &lhs, const UTC &rhs)
{
    return !(lhs == rhs);
}

/// lhs > rhs
bool Time::operator>(const UTC &lhs, const UTC &rhs)
{
    return lhs.getEpoch() > rhs.getEpoch();
}

/// lhs < rhs
bool Time::operator<(const UTC &lhs, const UTC &rhs)
{
    return lhs.getEpoch() < rhs.getEpoch();
}

/// Std out
std::ostream&
Time::operator<<(std::ostream &os, const UTC &time)
{
    char result[27];
    std::fill(result, result+27, '\0');
    sprintf(result, "%04d-%02d-%02dT%02d:%02d:%02d.%06d",
            time.getYear(), time.getMonth(), time.getDayOfMonth(),
            time.getHour(), time.getMinute(), time.getSecond(),
            time.getMicroSecond());
    return os << result;
}
