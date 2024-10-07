#ifndef Date_H
#define Date_H
#include <Interfaces/Printable.hpp>

struct Date : Printable {
    static constexpr size_t currentYear = 2024;
    static constexpr uint8_t monthDays[] = {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    size_t year;

    bool IsValid(void) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif