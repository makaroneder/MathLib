#ifndef Context_H
#define Context_H
#include "Term.hpp"

struct Context : MathLib::Printable {
    Context(void);
    Context(const MathLib::Array<Term>& theorems);

    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool ContainsTheorem(const Term& term) const;
    [[nodiscard]] bool CheckTypeEqualityWithReplacements(const Term& type1, const Term& type2, const Term& rep1, const Term& rep2, const Context& ignoredTheorems, bool check) const;
    [[nodiscard]] bool AreTypesEqualInternal(const Term& type1, const Term& type2, const Context& ignoredTheorems) const;
    [[nodiscard]] bool AreTypesEqual(const Term& type1, const Term& type2) const;
    [[nodiscard]] Term GetTrivialTypeOf(const Term& term) const;
    [[nodiscard]] Term GetTypeOf(const Term& term) const;
    [[nodiscard]] bool CanBeTypeOf(const Term& term, const Term& type) const;
    [[nodiscard]] Term RunSingle(const Term& term, bool runVariables) const;
    [[nodiscard]] Term Run(const Term& term, size_t depth, bool runVariables) const;
    [[nodiscard]] bool AreComputationallyEqual(const Term& term1, const Term& term2) const;
    [[nodiscard]] bool TypeCheck(const Term& term);

    private:
    Context(const MathLib::Array<Term>& theorems, const Context* const parent);
    [[nodiscard]] MathLib::String ToStringInternal(const MathLib::String& padding) const;
    [[nodiscard]] Context AppendTypeDeclaration(const Term& term) const;

    MathLib::Array<Term> theorems;
    const Context* const parent;
};

#endif