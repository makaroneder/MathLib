#include "Date.hpp"
#include "Host.hpp"
#include "String.hpp"

namespace MathLib {
    Date::Date(void) {}
    Date::Date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, size_t year) : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}
    bool Date::IsValid(void) const {
        return second < 60 && minute < 60 && hour < 24 && month <= 12 && day <= monthDays[month - 1] + (month == 2 && IsLeapYear());
    }
    bool Date::IsLeapYear(void) const {
        return !(year % 4) && (year % 100 || !(year % 400));
    }
    bool Date::LessThanEqual(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        if (day != other.day) return day < other.day;
        if (hour != other.hour) return hour < other.hour;
        if (minute != other.minute) return minute < other.minute;
        return second <= other.second;
    }
    String Date::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + MathLib::ToString(hour, 10, 2) + ':' + MathLib::ToString(minute, 10, 2) + ':' + MathLib::ToString(second, 10, 2) + ' ' + MathLib::ToString(day, 10, 2) + '.' + MathLib::ToString(month, 10, 2) + '.' + MathLib::ToString(year, 10);
    }
}