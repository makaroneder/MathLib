#ifndef Formula_H
#define Formula_H
#include <Interfaces/Printable.hpp>
#include <Interfaces/Comparable.hpp>

struct Formula;
struct Formula : MathLib::Comparable<Formula>, MathLib::Printable {
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

    protected:
    [[nodiscard]] virtual bool Equals(const Formula& other) const override;

    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};

#endif