#ifndef Term_H
#define Term_H
#include <Interfaces/Comparable.hpp>
#include <Interfaces/Printable.hpp>

struct Term;
struct Term : MathLib::Comparable<Term>, MathLib::Printable {
    enum class Type : uint8_t {
        None,
        Variable,
        Comma,
        Assignment,
        TypeDeclaration,
        TypeAbstraction,
        TypeApplication,
        Abstraction,
        Application,
    };
    MathLib::Array<Term> children;
    MathLib::String name;
    Type type;

    Term(void);
    Term(const MathLib::String& name);
    Term(Type type, const MathLib::Array<Term>& children);
    Term(Type type, const Term& a, const Term& b);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool Contains(const Term& term) const;
    [[nodiscard]] Term Replace(const Term& old, const Term& replacement) const;

    protected:
    [[nodiscard]] virtual bool Equals(const Term& other) const override;

    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const;
};

#endif