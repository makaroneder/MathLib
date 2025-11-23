#include "Date.hpp"
#include <String.hpp>
#include <Host.hpp>

Date::Date(void) {}
Date::Date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, size_t year) : second(second), minute(minute), hour(hour), day(day), month(month), year(year) {}
bool Date::IsValid(void) const {
    return second < 60 && minute < 60 && hour < 24 && month <= 12 && day <= monthDays[month - 1];
}
bool Date::operator==(const Date& other) const {
    return second == other.second && minute == other.minute && hour == other.hour && day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}
MathLib::String Date::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + MathLib::ToString(hour, 10, 2) + ':' + MathLib::ToString(minute, 10, 2) + ':' + MathLib::ToString(second, 10, 2) + ' ' + MathLib::ToString(day, 10, 2) + '.' + MathLib::ToString(month, 10, 2) + '.' + MathLib::ToString(year, 10);
}