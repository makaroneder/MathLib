#include "Date.hpp"
#include <String.hpp>
#include <Host.hpp>

Date creationData = Date::FromTimestampMacro(__TIMESTAMP__);
Date::Date(void) {}
Date::Date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, size_t year) : second(second), minute(minute), hour(hour), day(day), month(month), year(year) {}
Date Date::FromTimestampMacro(const char* timeStamp) {
    const MathLib::Array<MathLib::String> split1 = MathLib::Split(timeStamp, " ", false);
    const MathLib::Array<MathLib::String> split2 = MathLib::Split(split1.At(3), ":", false);
    uint8_t month = 0;
    if (split1.At(1) == "Jan") month = 1;
    else if (split1.At(1) == "Feb") month = 2;
    else if (split1.At(1) == "Mar") month = 3;
    else if (split1.At(1) == "Apr") month = 4;
    else if (split1.At(1) == "May") month = 5;
    else if (split1.At(1) == "Jun") month = 6;
    else if (split1.At(1) == "Jul") month = 7;
    else if (split1.At(1) == "Aug") month = 8;
    else if (split1.At(1) == "Sep") month = 9;
    else if (split1.At(1) == "Oct") month = 10;
    else if (split1.At(1) == "Nov") month = 11;
    else if (split1.At(1) == "Dec") month = 12;
    return Date(MathLib::StringToNumber(split2.At(2)), MathLib::StringToNumber(split2.At(1)), MathLib::StringToNumber(split2.At(0)), MathLib::StringToNumber(split1.At(2)), month, MathLib::StringToNumber(split1.At(4)));
}
bool Date::IsValid(void) const {
    return second < 60 && minute < 60 && hour < 24 && month <= 12 && day <= monthDays[month - 1];
}
bool Date::operator==(const Date& other) const {
    return second == other.second && minute == other.minute && hour == other.hour && day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}
MathLib::String Date::ToString(const MathLib::String& padding) const {
    return padding + MathLib::ToString(hour, 10, 2) + ':' + MathLib::ToString(minute, 10, 2) + ':' + MathLib::ToString(second, 10, 2) + ' ' + MathLib::ToString(day, 10, 2) + '.' + MathLib::ToString(month, 10, 2) + '.' + MathLib::ToString(year, 10);
}