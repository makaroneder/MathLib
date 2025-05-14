#ifndef LambdaTerm_H
#define LambdaTerm_H
#include <Interfaces/Printable.hpp>
#include <Interfaces/Comparable.hpp>

struct LambdaTerm : MathLib::Comparable, MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Variable,
        Abstraction,
        Application,
    };
    MathLib::String value;
    MathLib::Array<LambdaTerm> children;
    Type type;

    LambdaTerm(void);
    LambdaTerm(const MathLib::Sequence<char>& value);
    LambdaTerm(const LambdaTerm& func, const LambdaTerm& arg);
    LambdaTerm(const LambdaTerm& func, const MathLib::Sequence<char>& arg);
    [[nodiscard]] LambdaTerm Apply(const LambdaTerm& arg) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const MathLib::Comparable& other_) const override;

    private:
    [[nodiscard]] LambdaTerm ApplyInternal(const LambdaTerm& val, const MathLib::Sequence<char>& arg) const;
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};

#endif