#ifndef Term_H
#define Term_H
#include <Interfaces/Dictionary.hpp>

struct Term;
struct Term : MathLib::Comparable<Term> {
    enum class Type : uint8_t {
        None,
        Symbol,
        Variable,
        Abstraction,
        Application,
        Equivalence,
        TypeDeclaration,
        Definition,
    };
    Term* left;
    Term* right;
    size_t value;
    Type type;

    Term(void);
    Term(Type type, size_t value);
    Term(Type type, Term* left, Term* right);
    Term(Type type, size_t value, Term* left, Term* right);
    Term(const Term& other);
    Term(Term&& other);
    ~Term(void);
    Term& operator=(const Term& other);
    Term& operator=(Term&& other);
    [[nodiscard]] virtual bool Equals(const Term& other) const override;
    [[nodiscard]] size_t GetComplexity(void) const;
    [[nodiscard]] Term* Copy(void) const;
    [[nodiscard]] bool Match(const Term& other, MathLib::Dictionary<size_t, Term*>& variables, size_t depth) const;
    [[nodiscard]] Term* Substitute(const MathLib::Dictionary<size_t, Term*>& variables, size_t depth) const;
    [[nodiscard]] Term* ApplyEquivalence(const Term& other) const;
    [[nodiscard]] Term* Update(const MathLib::Function<Term*, const Term&>& func, bool applyToLeft) const;
};

#endif