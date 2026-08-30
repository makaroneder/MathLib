#include "Scope.hpp"

[[nodiscard]] MathLib::String WrapInParenthesis(const MathLib::String& str, bool root) {
    return root ? str : '('_M + str + ')';
}
Scope::Scope(void) : axioms(), symbols(), parent(nullptr) {}
Scope::Scope(const MathLib::Array<MathLib::String>& symbols) : axioms(), symbols(symbols), parent(nullptr) {}
Scope::Scope(const MathLib::Array<MathLib::String>& symbols, const Scope* parent) : axioms(), symbols(symbols), parent(parent) {}
Scope::Scope(const Scope& other) : axioms(other.axioms), symbols(other.symbols), parent(other.parent) {
    for (Term*& axiom : axioms) axiom = axiom->Copy();
}
Scope::Scope(Scope&& other) : axioms(other.axioms), symbols(other.symbols), parent(other.parent) {
    other.axioms = MathLib::Array<Term*>();
}
Scope::~Scope(void) {
    for (Term*& axiom : axioms) delete axiom;
}
Scope& Scope::operator=(const Scope& other) {
    axioms = other.axioms;
    symbols = other.symbols;
    parent = other.parent;
    for (Term*& axiom : axioms) axiom = axiom->Copy();
    return *this;
}
Scope& Scope::operator=(Scope&& other) {
    axioms = other.axioms;
    symbols = other.symbols;
    parent = other.parent;
    other.axioms = MathLib::Array<Term*>();
    return *this;
}
MathLib::String Scope::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const MathLib::String padd2 = padd + '\t';
    MathLib::String ret = "{\n";
    for (const Term* const& axiom : axioms) ret += padd2 + TermToString(*axiom) + '\n';
    return ret + padd + '}';
}
size_t Scope::StringToSymbol(const MathLib::String& string) const {
    const size_t size = symbols.GetSize();
    size_t ret = symbols.Find(string);
    if (ret != SIZE_MAX) return size - ret - 1;
    return parent ? parent->StringToSymbol(string) + size : SIZE_MAX;
}
MathLib::String Scope::SymbolToString(size_t symbol) const {
    const size_t size = symbols.GetSize();
    if (symbol < size) return symbols.AtUnsafe(size - symbol - 1);
    return parent ? parent->SymbolToString(symbol - size) : "";
}
MathLib::String Scope::TermToString(const Term& term) const {
    return TermToStringInternal(term, true);
}
MathLib::String Scope::TermToStringInternal(const Term& term, bool root) const {
    switch (term.type) {
        case Term::Type::Symbol: return SymbolToString(term.value);
        case Term::Type::Variable: return MathLib::ToString(term.value, 10);
        case Term::Type::Abstraction: return WrapInParenthesis(TermToStringInternal(*term.left, term.left->type == Term::Type::Application || term.left->type == Term::Type::Equivalence) + " -> " + TermToStringInternal(*term.right, term.right->type == Term::Type::Abstraction || term.right->type == Term::Type::Application || term.right->type == Term::Type::Equivalence), root);
        case Term::Type::Application: return WrapInParenthesis(TermToStringInternal(*term.left, term.left->type == Term::Type::Application) + " . " + TermToStringInternal(*term.right, false), root);
        case Term::Type::Equivalence: return WrapInParenthesis(TermToStringInternal(*term.left, term.left->type == Term::Type::Application) + " <=> " + TermToStringInternal(*term.right, term.right->type == Term::Type::Application), root);
        case Term::Type::TypeDeclaration: return TermToStringInternal(*term.left, true) + " : " + TermToStringInternal(*term.right, true);
        case Term::Type::Definition: return TermToStringInternal(*term.left, true) + " = " + TermToStringInternal(*term.right, true);
        default: return "";
    }
}
Term* Scope::ApplyAxiom(const Term& term, const Term& axiom) const {
    // TODO: Evaluate axioms
    if (axiom.type != Term::Type::Symbol) return nullptr;
    for (const Term* const& tmp : axioms) {
        if (tmp->type != Term::Type::TypeDeclaration) continue;
        if (tmp->left->value != axiom.value) continue;
        return term.ApplyEquivalence(*tmp->right);
    }
    return nullptr;
}
bool Scope::AddAxiom(Term* axiom) {
    return axioms.Add(axiom);
}