#!/usr/bin/env python3
import pytime

def test_utc():
    """ 
    Tests the time module.
    """
    # Try out the setters
    tf = pytime.UTC()
    tf.year = 2020
    tf.month_and_day = (1, 9)
    #tf.set_day_of_month(9)
    tf.hour = 0
    tf.minute = 12
    tf.second = 8
    tf.microsecond = 800000
    assert tf.to_string() == "2020-01-09T00:12:08.800000", 'string conversion failed'
    assert tf.epoch == 1578528728.8, 'get epoch failed'
    tf2 = tf
    tf2.day_of_year = 9
    assert tf2.epoch == 1578528728.8, 'get epoch failed'
    # Do it the other way
    t = pytime.UTC()
    t.epoch = 1578528728.8
    assert t.year == 2020, 'get year failed'
    assert t.month == 1, 'get month failed'
    assert t.day_of_month == 9, 'get day of month failed'
    assert t.day_of_year == 9, 'get day of year failed'
    assert t.hour == 0, 'get hour failed'
    assert t.minute == 12, 'get minute failed'
    assert t.second == 8, 'get second failed'
    assert t.microsecond == 800000, 'get micro_second failed'
    # Add an hour
    tday = pytime.UTC()
    tday.epoch = 86400.1
    tnew = t + tday 
    assert tnew.year == 2020, 'get year failed'
    assert tnew.month == 1, 'get month failed'
    assert tnew.day_of_month == 10, 'get day of month failed'
    assert tnew.day_of_year == 10, 'get day of year failed'
    assert tnew.hour == 0, 'get hour failed'
    assert tnew.minute == 12, 'get minute failed'
    assert tnew.second == 8, 'get second failed'
    assert tnew.microsecond == 900000, 'get micro_second failed'

    tnew = t + 86400.1 
    assert tnew.year == 2020, 'get year failed - add double'
    assert tnew.month == 1, 'get month failed - add double'
    assert tnew.day_of_month == 10, 'get day of month failed - add double'
    assert tnew.day_of_year == 10, 'get day of year failed - add double'
    assert tnew.hour == 0, 'get hour failed - add double'
    assert tnew.minute == 12, 'get minute failed - add double'
    assert tnew.second == 8, 'get second failed - add double'
    assert tnew.microsecond == 900000, 'get micro_second failed - add double'

    # Subtract half a day
    thalf = pytime.UTC()
    thalf.epoch = 43200
    tsub = tnew - thalf
    assert tsub.year == 2020, 'get year failed'
    assert tsub.month == 1, 'get month failed'
    assert tsub.day_of_month == 9, 'get day of month failed'
    assert tsub.day_of_year == 9, 'get day of year failed'
    assert tsub.hour == 12, 'get hour failed'
    assert tsub.minute == 12, 'get minute failed'
    assert tsub.second == 8, 'get second failed'
    assert tsub.microsecond == 900000, 'get micro_second failed'
    assert tsub.is_leap_year(), 'leap year failed'

    tsub = tnew - 43200
    assert tsub.year == 2020, 'get year failed - sub double'
    assert tsub.month == 1, 'get month failed - sub double'
    assert tsub.day_of_month == 9, 'get day of month failed - sub double'
    assert tsub.day_of_year == 9, 'get day of year failed - sub double'
    assert tsub.hour == 12, 'get hour failed - sub double'
    assert tsub.minute == 12, 'get minute failed - sub double'
    assert tsub.second == 8, 'get second failed - sub double'
    assert tsub.microsecond == 900000, 'get micro_second failed - sub double'

if __name__ == "__main__":
    print(pytime.__doc__ + " v:" + pytime.__version__)
    print(pytime.UTC().__doc__)
    test_utc() 
    print("Passed UTC test")
