#ifndef MathLib_Date_H
#define MathLib_Date_H
#include "Interfaces/Orderable.hpp"
#include "Interfaces/Printable.hpp"

namespace MathLib {
    struct Date;
    struct Date : Orderable<Date>, Printable {
        static constexpr size_t currentYear = 2026;
        static constexpr uint8_t monthDays[] = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
        };
        size_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;

        Date(void);
        Date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, size_t year);
        [[nodiscard]] bool IsValid(void) const;
        [[nodiscard]] bool IsLeapYear(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] virtual bool LessThanEqual(const Date& other) const override;
    };
}

#endif