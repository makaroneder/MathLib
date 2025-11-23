#ifndef Formula_H
#define Formula_H
#include "../RationalNumber.hpp"
#include <Interfaces/Dictionary.hpp>

struct Formula;
struct Formula : MathLib::Comparable<Formula>, MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Constant,
        Variable,
        Add,
        Sub,
        Mul,
        Div,
        Square,
    };
    RationalNumber value;
    MathLib::String name;
    MathLib::Array<Formula> children;
    Type type;

    Formula(void);
    Formula(const RationalNumber& value);
    Formula(const MathLib::Sequence<char>& name);
    Formula(Type type, const Formula& a, const Formula& b);
    Formula(Type type, const MathLib::Sequence<Formula>& children);
    [[nodiscard]] static Formula MakeAdd(Formula a, Formula b);
    [[nodiscard]] static Formula MakeSub(Formula a, Formula b);
    [[nodiscard]] static Formula MakeMul(Formula a, Formula b);
    [[nodiscard]] static Formula MakeDiv(Formula a, Formula b);
    [[nodiscard]] static Formula MakeSquare(Formula base);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] size_t GetComplexity(void) const;
    [[nodiscard]] Formula Simplify(void) const;
    Formula Evaluate(MathLib::Dictionary<MathLib::String, Formula>& derivatives) const;
    [[nodiscard]] Formula Substitute(const MathLib::Dictionary<MathLib::String, Formula>& variables) const;

    protected:
    [[nodiscard]] virtual bool Equals(const Formula& other) const override;

    private:
    [[nodiscard]] Formula EvaluateInternal(MathLib::Dictionary<MathLib::String, Formula>& derivatives) const;
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};
namespace MathLib {
    MakeFormatter(Formula, self, padding, { return self.ToString(padding); });
}

#endif