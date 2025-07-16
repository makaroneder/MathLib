#ifndef Formula_H
#define Formula_H
#include <Interfaces/Printable.hpp>

struct Formula : MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Variable,
        Conjunction,
        Disjunction,
        Implication,
        Negation,
    };
    MathLib::String value;
    MathLib::Array<Formula> children;
    Type type;

    Formula(void);
    Formula(const MathLib::Sequence<char>& value);
    Formula(Type type, const MathLib::Sequence<Formula>& children);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const Formula& other) const;
    [[nodiscard]] bool operator!=(const Formula& other) const;

    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};

#endif