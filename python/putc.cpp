#include <sstream>
#include <time/utc.hpp>
#include "include/putc.hpp"

using namespace PTime;

/// Constructor
UTC::UTC() :
    mTime(std::make_unique<Time::UTC> ())
{
}

/// Copy c'tor
UTC::UTC(const UTC &time)
{
    *this = time;
}

UTC::UTC(UTC &&time) noexcept
{
    *this = std::move(time);
}

UTC::UTC(const Time::UTC &time)
{
    *this = time;
}

Time::UTC UTC::getNativeClass() const
{
    Time::UTC timeOut(*mTime);
    return timeOut;
}

/// Copy assignment
UTC& UTC::operator=(const UTC &time)
{
    if (&time == this){return *this;}
    mTime = std::make_unique<Time::UTC> (*time.mTime);
    return *this;
}

UTC& UTC::operator=(const Time::UTC &time)
{
    mTime = std::make_unique<Time::UTC> (time);
    return *this;
}

/// Move assignment
UTC& UTC::operator=(UTC &&time) noexcept
{
    if (&time == this){return *this;}
    mTime = std::move(time.mTime);
    return *this;
}

/// Operators
UTC UTC::operator+(const UTC &y)
{
    UTC tout(*mTime + *y.mTime);
    return tout;
}

UTC UTC::operator+(const double y)
{
    UTC tout(*mTime + y);
    return tout;
}

UTC UTC::operator-(const UTC &y)
{
    UTC tout(*mTime - *y.mTime);
    return tout;
}

UTC UTC::operator-(const double y)
{
    UTC tout(*mTime - y);
    return tout;
}

/// Destructor 
UTC::~UTC() = default;

bool UTC::isLeapYear() const noexcept
{
    return mTime->isLeapYear();
}

/// Set time to now
void UTC::now()
{
    mTime->now();
}

/// Get/Set Epochal Time
void UTC::setEpoch(const double time)
{
    mTime->setEpoch(time);
}

double UTC::getEpoch() const
{
    return mTime->getEpoch();
}

/// Get/Set Year
void UTC::setYear(const int year)
{
    mTime->setYear(year);
}

int UTC::getYear() const
{
    return mTime->getYear();
}                            

/// Get/Set day of year
void UTC::setDayOfYear(const int jday)
{
    mTime->setDayOfYear(jday);
}

int UTC::getDayOfYear() const
{
    return mTime->getDayOfYear();
}

/// Get/Set month
void UTC::setMonthAndDay(const std::pair<int, int> &md)
{
    mTime->setMonthAndDay(md);
}

int UTC::getMonth() const
{
    return mTime->getMonth();
}

int UTC::getDayOfMonth() const
{
    return mTime->getDayOfMonth();
}

/// Get/Set Hour
void UTC::setHour(const int hour)
{
    mTime->setHour(hour);
}

int UTC::getHour() const
{
    return mTime->getHour();
}

/// Get/Set Minute
void UTC::setMinute(const int minute)
{
    mTime->setMinute(minute);
}

int UTC::getMinute() const
{
    return mTime->getMinute();
}

/// Get/Set Second
void UTC::setSecond(const int second)
{
    mTime->setSecond(second);
}

int UTC::getSecond() const
{
    return mTime->getSecond();
}

/// Get/Set Microsecond
void UTC::setMicroSecond(const int musec)
{
    mTime->setMicroSecond(musec);
}

int UTC::getMicroSecond() const
{
    return mTime->getMicroSecond();
}

/// Converts a time to string
std::string UTC::toString() const noexcept
{
    std::stringstream ss;
    ss << *mTime;
    return ss.str();
}

/// Creates the class
void PTime::initializeUTC(pybind11::module &m)
{
    pybind11::class_<PTime::UTC> time(m, "UTC");
    time.def(pybind11::init<> ());
    time.def("__copy__", [](const PTime::UTC &self)
    {
        return PTime::UTC(self);
    });
    time.def("__add__", [](PTime::UTC &a, const PTime::UTC &b)
    {
        return a + b;
    }, pybind11::is_operator());
    time.def("__add__", [](PTime::UTC &a, const double b)
    {
        return a + b;
    }, pybind11::is_operator());
    time.def("__sub__", [](PTime::UTC &a, const PTime::UTC &b)
    {
        return a - b;
    }, pybind11::is_operator());
    time.def("__sub__", [](PTime::UTC &a, const double b)
    {
        return a - b;
    }, pybind11::is_operator());
    time.def("__repr__", &PTime::UTC::toString);

    time.doc() = "This module is helpful for converting between epochal and date-times.\n\nProperties:\n\n  epoch : The UTC time in seconds since the epoch (Jan 1, 1970)\n  day_of_year : The day of the year.  This must be in the range [1,366].  Additional error checking may be performed if this is a leap year.\n  month_and_day : Sets the month and day of the month.  This is an alternative to setting the day of year with day_of_year.  The month must be in the range [1,12] and the day must be in the range [1,31].\n  month (readonly) : The month of year.\n  day_of_month (readonly) : The day of the month.\n  hour : The hour of the day.  This must be in the range [0,23]\n  minute : The minute of the hour.  This must be in the range [0,59].\n  second : The second of the minute.  This must be in the range [0,59]\n  microsecond : The microsecond of the second.   This must be in the range [0,999999].\n";
    time.def_property("epoch",
                      &PTime::UTC::getEpoch,
                      &PTime::UTC::setEpoch,
                      "The UTC time in seconds since the epoch (Jan 1, 1970).");
    time.def_property("year",
                      &PTime::UTC::getYear,
                      &PTime::UTC::setYear, 
                      "The four digit year.");
    time.def_property("day_of_year",
                      &PTime::UTC::getDayOfYear,
                      &PTime::UTC::setDayOfYear,
                      "The day of the year.   This must be in the range [1,366] where 366 accounts for leap years.");

    time.def_property("day_of_year",
                      &PTime::UTC::getDayOfYear,
                      &PTime::UTC::setDayOfYear);

    time.def_property("month_and_day",
                      nullptr,
                      &PTime::UTC::setMonthAndDay);
    time.def_property_readonly("month",
                               &PTime::UTC::getMonth);
    time.def_property_readonly("day_of_month",
                               &PTime::UTC::getDayOfMonth);

    time.def_property("hour",
             &PTime::UTC::getHour,
             &PTime::UTC::setHour);
    time.def_property("minute",
             &PTime::UTC::getMinute,
             &PTime::UTC::setMinute);
    time.def_property("second",
             &PTime::UTC::getSecond,
             &PTime::UTC::setSecond);
    time.def_property("microsecond",
             &PTime::UTC::getMicroSecond,
             &PTime::UTC::setMicroSecond);

    time.def("is_leap_year",
             &PTime::UTC::isLeapYear,
             "True indicates that this is a leap year.");
    time.def("now",
             &PTime::UTC::now,
             "Sets the UTC time to now.");
    time.def("to_string",
             &PTime::UTC::toString,
             "Converts a time to a string representation with the format: YYYY-MM-DDTHH:MM::SS.SSSSSS");
}
