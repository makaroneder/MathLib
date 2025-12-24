#ifndef Date_H
#define Date_H
#include <Interfaces/Printable.hpp>
#include <Interfaces/Comparable.hpp>

struct Date;
struct Date : MathLib::Comparable<Date>, MathLib::Printable {
    static constexpr size_t currentYear = 2025;
    static constexpr uint8_t monthDays[] = {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    size_t year;

    Date(void);
    Date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, size_t year);
    [[nodiscard]] bool IsValid(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const Date& other) const override;
};

#endif