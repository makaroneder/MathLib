#include "Date.hpp"
#include <String.hpp>

bool Date::IsValid(void) const {
    return second < 60 && minute < 60 && hour < 24 && month <= 12 && day <= monthDays[month - 1];
}
bool Date::operator==(const Date& other) const {
    return second == other.second && minute == other.minute && hour == other.hour && day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}
String Date::ToString(const String& padding) const {
    return padding + ::ToString(hour, 10, 2) + ':' + ::ToString(minute, 10, 2) + ':' + ::ToString(second, 10, 2) + ' ' + ::ToString(day, 10, 2) + '.' + ::ToString(month, 10, 2) + '.' + ::ToString(year, 10);
}