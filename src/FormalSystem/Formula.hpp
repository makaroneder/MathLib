#ifndef Formula_H
#define Formula_H
#include <Expected.hpp>

struct Formula : MathLib::Printable {
    enum class Type {
        Constant,
        Variable,
        Operation,
    } type;
    MathLib::String value;
    MathLib::Array<Formula> children;

    Formula(void);
    Formula(bool value);
    Formula(bool constant, const MathLib::Sequence<char>& value);
    Formula(const MathLib::Sequence<char>& value, const MathLib::Sequence<Formula>& children);
    Formula(const MathLib::Sequence<char>& value, const Formula& left, const Formula& right);
    Formula(const MathLib::Sequence<char>& value, const Formula& child);
    [[nodiscard]] bool IsTrue(void) const;
    [[nodiscard]] MathLib::Expected<bool> ToBool(void) const;
    [[nodiscard]] bool operator==(const Formula& other) const;
    [[nodiscard]] bool operator!=(const Formula& other) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif