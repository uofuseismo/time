#ifndef PTIME_UTC_HPP
#define PTIME_UTC_HPP
#include <memory>
#include <pybind11/pybind11.h>
namespace Time
{
class UTC;
}

namespace PTime
{
class UTC
{
public:
    /// Constructor
    UTC();
    /// Copy c'tors
    UTC(const UTC &time);
    UTC(UTC &&time) noexcept;
    UTC(const Time::UTC &time);
    /// Copy assignment
    UTC& operator=(const UTC &time);
    UTC& operator=(UTC &&time) noexcept;
    UTC& operator=(const Time::UTC &time);
    /// Add/subtract operators
    UTC operator+(const UTC &t);
    UTC operator-(const UTC &t);
    /// Add/subtract operators
    UTC operator+(double t);
    UTC operator-(double t);
    /// Destructor
    ~UTC();

    /// Attributes
    void now();

    void setEpoch(double epoch);
    [[nodiscard]] double getEpoch() const;

    void setYear(int year);
    [[nodiscard]] int getYear() const;

    void setDayOfYear(int dayOfYear);
    [[nodiscard]] int getDayOfYear() const;

    void setMonthAndDay(const std::pair<int, int>  &md);
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getDayOfMonth() const;

    void setHour(int hour);
    [[nodiscard]] int getHour() const;

    void setMinute(int minute);
    [[nodiscard]] int getMinute() const;

    void setSecond(int second);
    [[nodiscard]] int getSecond() const;

    void setMicroSecond(int musec);
    [[nodiscard]] int getMicroSecond() const;

    [[nodiscard]] bool isLeapYear() const noexcept;

    [[nodiscard]] Time::UTC getNativeClass() const;

    [[nodiscard]] std::string toString() const noexcept;

private:
    class std::unique_ptr<Time::UTC> mTime;
};
void initializeUTC(pybind11::module &m);
}
#endif
