#include <iostream>
#include <string>
#include "time/utc.hpp"
#include <gtest/gtest.h>

namespace
{

TEST(UTC, BaseTests)
{
    Time::UTC time;
    time.setYear(2012);
    time.setMonthAndDay( std::pair(5, 7) );
    time.setHour(15);
    time.setMinute(13);
    time.setSecond(58);
    time.setMicroSecond(100);
 
    EXPECT_TRUE(time.isLeapYear());
    EXPECT_EQ(time.getYear(), 2012);
    EXPECT_EQ(time.getMonthAndDay().first, 5);
    EXPECT_EQ(time.getMonthAndDay().second, 7);

    EXPECT_EQ(time.getHour(), 15);
    EXPECT_EQ(time.getMinute(), 13);
    EXPECT_EQ(time.getSecond(), 58);
    EXPECT_EQ(time.getMicroSecond(), 100);

    EXPECT_EQ(time.getDayOfYear(), 128);
    EXPECT_NEAR(time.getEpoch(), 1336403638.0001, 1.e-10);

    Time::UTC tCopy(time);
    EXPECT_TRUE(tCopy.isLeapYear());
    EXPECT_EQ(tCopy.getYear(), 2012);
    EXPECT_EQ(tCopy.getMonthAndDay().first, 5); 
    EXPECT_EQ(tCopy.getMonthAndDay().second, 7); 
    EXPECT_EQ(tCopy.getHour(), 15);
    EXPECT_EQ(tCopy.getMinute(), 13);
    EXPECT_EQ(tCopy.getSecond(), 58);
    EXPECT_EQ(tCopy.getMicroSecond(), 100);
    EXPECT_NEAR(tCopy.getEpoch(), 1336403638.0001, 1.e-10);

    EXPECT_NO_THROW(time.setDayOfYear(366));
    EXPECT_EQ(time.getDayOfYear(), 366);
    EXPECT_EQ(time.getMonthAndDay().first, 12);
    EXPECT_EQ(time.getMonthAndDay().second, 31);

    Time::UTC epochalTime(1336403638.0001);
    EXPECT_TRUE(epochalTime.isLeapYear());
    EXPECT_EQ(epochalTime.getYear(), 2012);
    EXPECT_EQ(epochalTime.getMonthAndDay().first, 5); 
    EXPECT_EQ(epochalTime.getMonthAndDay().second, 7); 
    EXPECT_EQ(epochalTime.getHour(), 15);
    EXPECT_EQ(epochalTime.getMinute(), 13);
    EXPECT_EQ(epochalTime.getSecond(), 58);
    EXPECT_EQ(epochalTime.getMicroSecond(), 100);
    EXPECT_NEAR(epochalTime.getEpoch(), 1336403638.0001, 1.e-10);
}

TEST(UTC, EpochToCalendar)
{
    Time::UTC time(1408117832.844000);
    //time.setEpochalTime(1408117832.844000);
    EXPECT_EQ(2014, time.getYear());
    EXPECT_EQ(8, time.getMonth());
    EXPECT_EQ(227, time.getDayOfYear());
    EXPECT_EQ(15, time.getDayOfMonth());
    EXPECT_EQ(15, time.getHour());
    EXPECT_EQ(50, time.getMinute());
    EXPECT_EQ(32, time.getSecond());
    EXPECT_EQ(844000, time.getMicroSecond());
    EXPECT_FALSE(time.isLeapYear());
    // Test the move constructor
    Time::UTC moveTime(std::move(time));
    EXPECT_EQ(2014, moveTime.getYear());
    EXPECT_EQ(8, moveTime.getMonth());
    EXPECT_EQ(15, moveTime.getDayOfMonth());
    EXPECT_EQ(15, moveTime.getHour());
    EXPECT_EQ(50, moveTime.getMinute());
    EXPECT_EQ(32, moveTime.getSecond());
    EXPECT_EQ(844000, moveTime.getMicroSecond());
    EXPECT_FALSE(moveTime.isLeapYear());

    Time::UTC tTest1(1230784385.5);
    EXPECT_EQ(tTest1.getYear(), 2009);
    EXPECT_EQ(tTest1.getMonth(), 1); 
    EXPECT_EQ(tTest1.getDayOfMonth(), 1); 
    EXPECT_EQ(tTest1.getDayOfYear(), 1); 
    EXPECT_EQ(tTest1.getHour(), 4); 
    EXPECT_EQ(tTest1.getMinute(), 33);
    EXPECT_EQ(tTest1.getSecond(), 5); 
    EXPECT_EQ(tTest1.getMicroSecond(), 500000);
    EXPECT_FALSE(tTest1.isLeapYear());
}

TEST(UTC, CalendarToEpoch)
{
    Time::UTC time;
    // Build time from day of month
    time.setYear(2016);
    time.setMonthAndDay(std::pair(4, 11));
    time.setHour(19);
    time.setMinute(13);
    time.setSecond(45);
    time.setMicroSecond(255000);
    EXPECT_NEAR(1460402025.255, time.getEpoch(), 1.e-4);
    EXPECT_TRUE(time.isLeapYear());
    // Build time from julian day
    Time::UTC timej;
    timej.setYear(2016);
    timej.setDayOfYear(102); // Add 1 for leap year
    timej.setHour(19);
    timej.setMinute(13);
    timej.setSecond(45);
    timej.setMicroSecond(255000);
    // Test the copy constructor
    Time::UTC copyTime(timej);
    EXPECT_NEAR(1460402025.255, copyTime.getEpoch(), 1.e-4);
    EXPECT_TRUE(copyTime.isLeapYear());
}

TEST(UTC, clear)
{
    Time::UTC time(1408117832.844000);
    time.clear();
    EXPECT_EQ(1970, time.getYear());
    EXPECT_EQ(1,    time.getDayOfYear());
    EXPECT_EQ(1,    time.getMonth());
    EXPECT_EQ(1,    time.getDayOfMonth());
    EXPECT_EQ(0,    time.getHour());
    EXPECT_EQ(0,    time.getMinute());
    EXPECT_EQ(0,    time.getSecond());
    EXPECT_EQ(0,    time.getMicroSecond());
    EXPECT_NEAR(0,  time.getEpoch(), 1.e-4);
    EXPECT_FALSE(time.isLeapYear());
}

TEST(UTC, CompareTime)
{
    Time::UTC time1(1460402025.255);
    Time::UTC time2(1460402425.255);
    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time1 == time1);
    EXPECT_TRUE(time1 != time2);
}


TEST(UTC, swap)
{
    Time::UTC time1(1408117832.844000);
    auto time1Ref = time1;
    Time::UTC time2(1408117834.123000);
    auto time2Ref = time2;
    swap(time1, time2);
    EXPECT_TRUE(time1 == time2Ref);
    EXPECT_TRUE(time2 == time1Ref);
}

TEST(Time, TimeOperators)
{
    Time::UTC time1; // 1578513045.372
    time1.setYear(2020);
    time1.setMonthAndDay(std::pair(1, 8));
    time1.setHour(19);
    time1.setMinute(50);
    time1.setSecond(45);
    time1.setMicroSecond(372000);

    Time::UTC day(86400); // Add 2 days
    auto time3 = time1 + day + day;
    EXPECT_NEAR(time3.getEpoch(), 1578513045.372 + 86400*2, 1.e-4);
    auto time4 = time3 - day; // Now subtract a day
    EXPECT_NEAR(time4.getEpoch(), 1578513045.372 + 86400, 1.e-4);
}

TEST(Time, StringConstructor)
{
    // Test the string constructor
    std::string s("2020-03-17T08:01:33.009000"); // St Patty's Day 2020
    Time::UTC strTime(s);
    EXPECT_EQ(strTime.getYear(), 2020);
    EXPECT_EQ(strTime.getMonth(), 3); 
    EXPECT_EQ(strTime.getDayOfMonth(), 17);
    EXPECT_EQ(strTime.getHour(), 8); 
    EXPECT_EQ(strTime.getMinute(), 1); 
    EXPECT_EQ(strTime.getSecond(), 33);
    EXPECT_EQ(strTime.getMicroSecond(), 9000); 

    std::string s2("2020-03-17T08:01:33"); // For when people violate the rules
    Time::UTC strTime2(s2);
    EXPECT_EQ(strTime2.getYear(), 2020);
    EXPECT_EQ(strTime2.getMonth(), 3); 
    EXPECT_EQ(strTime2.getDayOfMonth(), 17);
    EXPECT_EQ(strTime2.getHour(), 8); 
    EXPECT_EQ(strTime2.getMinute(), 1); 
    EXPECT_EQ(strTime2.getSecond(), 33);
    EXPECT_EQ(strTime2.getMicroSecond(), 0); 
}

}
