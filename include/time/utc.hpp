#ifndef TIME_UTC_HPP
#define TIME_UTC_HPP
#include <ostream>
#include <string>
#include <memory>
namespace Time
{
/// @class UTC "utc.hpp" "time/utc.hpp"
/// @brief Defines a UTC time.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class UTC 
{
public:
    /// @name Constructors
    /// @{
    /// @brief Constructor.
    UTC();
    /// @brief Initializes this class from a time stamp.
    /// @param[in] time   The UTC time stamp measured in seconds from the epoch. 
    /// @sa \c setEpoch()
    explicit UTC(double time);
    /// @brief Initializes a time from string-time stamp.
    /// @param[in] time   The time stamp in YYYY-MM-DD:HH:MM:SS.XXXXXX form.
    /// @throws std::invalid_argument if the string length is unexpected.
    /// @note This can also parse YYYY-MM-DD:HH:MM:SS form.
    explicit UTC(const std::string &time);
    /// @brief Copy constructor.
    /// @param[in] time  The time class from which to initialize this class.
    UTC(const UTC &time);
    /// @brief Move constructor.
    /// @param[in,out] time  The time class from which to initialize this class.
    ///                      On exit, location's behavior is undefined.
    UTC(UTC &&time) noexcept;
    /// @}

    /// @name Operators   
    /// @{
    /// @brief Copy assignment operator.
    /// @param[in] time  The time class to copy to this.
    /// @result A deep copy of the input time.
    UTC& operator=(const UTC &time);
    /// @brief Move assignment operator.
    /// @param[in,out] time  The time class whose memory will be moved to this.
    /// @result The memory from time moved to this.
    UTC& operator=(UTC &&time) noexcept;
    /// @}
     
    /// @brief Sets the seconds since the epoch.
    /// @param[in] timeStamp   The seconds since the epoch (Jan 1 1970).
    void setEpoch(double timeStamp) noexcept;
    /// @result The seconds since the epoch (Jan 1 1970).
    [[nodiscard]] double getEpoch() const noexcept;

    /// @brief Sets the year in which to perform the calculation.
    /// @param[in] year  The year which must be in the range [-1000,2999].
    /// @throws std::invalid_argument if year is not in range.
    void setYear(int year);
    /// @result The year in which to perform the calculation.
    [[nodiscard]] int getYear() const noexcept;
    /// @result True indicates the year is a leap year.
    [[nodiscard]] bool isLeapYear() const noexcept;

    /// @brief Sets the month and day of the month.
    /// @param[in] monthAndDay  monthAndDay.first is the month which must be
    ///                         in the range of [1,12] and monthAndDay.second
    ///                         is the day of the month which must be in the
    ///                         range of valid days for that month. 
    /// @note This will change the day of the year.
    void setMonthAndDay(const std::pair<int, int> &monthAndDay);
    /// @result result.first is the month and result.second is the day of
    ///         the month.
    [[nodiscard]] std::pair<int, int> getMonthAndDay() const noexcept;
    /// @result The month of the year.
    [[nodiscard]] int getMonth() const noexcept;
    /// @result The day of the month.
    [[nodiscard]] int getDayOfMonth() const noexcept;

    /// @brief Sets the day of the year.
    /// @param[in] dayOfYear   The day of the year.  This must be in the range
    ///                        [1,365] if \c isLeapYear() is false or [1,366]
    ///                        if \c isLeapYear() is true.
    /// @throw std::invalid_argument if the day of the year is out of range.
    /// @note This will change the values of the month and day of the month.
    void setDayOfYear(int dayOfYear);
    /// @result The day of the year.
    [[nodiscard]] int getDayOfYear() const noexcept;
   
    /// @brief Sets the hours of the day.
    /// @param[in] hour  The hour of the day.  This must be in the range [0,23].
    /// @throws std::invalid_argument if the hour is out of range.
    void setHour(int hour);
    /// @result The hour of the day.
    [[nodiscard]] int getHour() const noexcept;

    /// @brief Sets the minute of the hour.
    /// @param[in] minute  The minute of the hour.  This must be in the
    ///                    range [0,59].
    /// @throws std::invalid_argument if the minute is out of range.
    void setMinute(int minute);
    /// @result The minute of the hour.
    [[nodiscard]] int getMinute() const noexcept;

    /// @brief Sets the second of the minute.
    /// @param[in] second   The second of the minute.  This must be in the
    ///                     range [0,59].
    /// @throws std::invalid_argument if the second is out of range. 
    void setSecond(int second);
    /// @result The second of the minute.
    [[nodiscard]] int getSecond() const noexcept;


    /// @brief Sets the microsecond.
    /// @param[in] muSec   The additional microseconds to add to the second.
    ///                    This must be in the range [0, 999999].
    void setMicroSecond(const int muSec);
    /// @result The microsecond to add to the second.
    [[nodiscard]] int getMicroSecond() const noexcept;

    /// @name Destructors
    /// @{
    /// @brief Resets the class.
    void clear() noexcept;    
    /// @brief Destructor.
    ~UTC();
    /// @}

    /// @brief Swaps two time classes.
    /// @param[in,out] lhs  Class to exchange with rhs.
    /// @param[in,out] rhs  Class to exchange with lhs.
    friend void swap(UTC &lhs, UTC &rhs);
private:
    class UTCImpl;
    std::unique_ptr<UTCImpl> pImpl;
};
/// @brief Swaps two time classes, lhs and rhs.
/// @param[in,out] lhs  On exit this will contain the information in rhs.
/// @param[in,out] rhs  On exit this will contain the information in lhs.
void swap(UTC &lhs, UTC &rhs);
/// @brief Computes the sum of two times a la: x + y.
/// @param[in] x   The time.
/// @param[in] y   The time to add to x.
/// @result The sum of the two times: x + y.
UTC operator+(const UTC &x, const UTC &y);
/// @brief Adds seconds to a time a la: x + y (seconds).
/// @param[in] x   The time.
/// @param[in] y   The number of seconds to add to x.
/// @result The sum of the time in x with the number of seconds in y: x + y.
UTC operator+(const UTC &x, double y); 
/// @brief Computes the difference between two times a la: x - y.
/// @param[in] x   The time.
/// @param[in] y   The time to subtract from x.
/// @result The difference between the two times: x - y.
UTC operator-(const UTC &x, const UTC &y);
/// @brief Removes seconds from a time a la: x - y (seconds).
/// @param[in] x   The time.
/// @param[in] y   The number of seconds to subtract from to x.
/// @result The difference between the time in x and the
///         number of seconds in y: x - y.
UTC operator-(const UTC &x, double y); 
/// @result True indicates that lhs == rhs, i.e., the times are equal.
bool operator==(const UTC &lhs, const UTC &rhs);
/// @result True indicates that lhs != rhs, i.e., the times are not equal.
bool operator!=(const UTC &lhs, const UTC &rhs);
/// @result True indicates that lhs > rhs, i.e., the lhs is later than the rhs.
bool operator>(const UTC &lhs, const UTC &rhs);
/// @result True indicates that lhs < rhs, i.e, the lhs is earlier than the rhs.
bool operator<(const UTC &lhs, const UTC &rhs);
/// @brief Outputs a time as YYYY-MM-DDTHH:MM:SS.SSSSSS
/// @param[in] os    An output stream object.
/// @param[in] time  The time stamp
/// @return A formatted time.
std::ostream& operator<<(std::ostream &os, const UTC &time);
}
#endif
