#ifndef LambdaTerm_H
#define LambdaTerm_H
#include <Interfaces/Printable.hpp>
#include <Interfaces/Comparable.hpp>

struct LambdaTerm;
struct LambdaTerm : MathLib::Comparable<LambdaTerm>, MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Variable,
        Abstraction,
        Application,
        Binding,
        String,
        Pattern,
    };
    MathLib::String value;
    MathLib::Array<LambdaTerm> children;
    Type type;

    LambdaTerm(void);
    LambdaTerm(const MathLib::Sequence<char>& value, bool string);
    LambdaTerm(const LambdaTerm& func, const LambdaTerm& arg);
    LambdaTerm(const LambdaTerm& func, const MathLib::Sequence<char>& arg);
    LambdaTerm(const MathLib::Sequence<char>& name, const LambdaTerm& value);
    LambdaTerm(const MathLib::Sequence<LambdaTerm>& cases);
    [[nodiscard]] bool Match(const LambdaTerm& term, MathLib::WritableSequence<LambdaTerm>& bindings) const;
    [[nodiscard]] LambdaTerm Run(const MathLib::Sequence<LambdaTerm>& bindings) const;
    [[nodiscard]] MathLib::Array<MathLib::String> GetVariables(void) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const LambdaTerm& other) const override;

    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};

#endif