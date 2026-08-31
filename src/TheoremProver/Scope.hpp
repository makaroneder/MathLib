#ifndef Scope_H
#define Scope_H
#include "Term.hpp"

struct Scope : MathLib::Printable {
    Scope(void);
    Scope(const MathLib::Array<MathLib::String>& symbols);
    Scope(const MathLib::Array<MathLib::String>& symbols, const Scope* parent);
    Scope(const Scope& other);
    Scope(Scope&& other);
    ~Scope(void);
    Scope& operator=(const Scope& other);
    Scope& operator=(Scope&& other);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] size_t StringToSymbol(const MathLib::String& string) const;
    [[nodiscard]] MathLib::String SymbolToString(size_t symbol) const;
    [[nodiscard]] MathLib::String TermToString(const Term& term) const;
    [[nodiscard]] Term* GetTypeOf(const Term& term, const MathLib::Sequence<const Term*>& variableTypes) const;
    [[nodiscard]] Term* ApplyAxiom(const Term& term, const Term& axiom) const;
    [[nodiscard]] bool AddAxiom(Term* axiom);

    private:
    [[nodiscard]] MathLib::String TermToStringInternal(const Term& term, bool root) const;
    [[nodiscard]] Term* GetTypeOfSymbol(const Term& term) const;

    MathLib::Array<Term*> axioms;
    MathLib::Array<MathLib::String> symbols;
    const Scope* parent;
};

#endif