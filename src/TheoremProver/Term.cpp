#include "Term.hpp"
#include <FunctionT.hpp>

Term::Term(void) : left(nullptr), right(nullptr), value(0), type(Type::None) {}
Term::Term(Type type, size_t value) : left(nullptr), right(nullptr), value(value), type(type) {}
Term::Term(Type type, Term* left, Term* right) : left(left), right(right), value(0), type(type) {}
Term::Term(Type type, size_t value, Term* left, Term* right) : left(left), right(right), value(value), type(type) {}
Term::Term(const Term& other) : left(other.left->Copy()), right(other.right->Copy()), value(other.value), type(other.type) {}
Term::Term(Term&& other) : left(other.left), right(other.right), value(other.value), type(other.type) {
    other.left = other.right = nullptr;
}
Term::~Term(void) {
    if (left) delete left;
    if (right) delete right;
}
Term& Term::operator=(const Term& other) {
    left = other.left->Copy();
    right = other.right->Copy();
    value = other.value;
    type = other.type;
    return *this;
}
Term& Term::operator=(Term&& other) {
    left = other.left;
    right = other.right;
    value = other.value;
    type = other.type;
    other.left = other.right = nullptr;
    return *this;
}
bool Term::Equals(const Term& other) const {
    return type == other.type && value == other.value && ((left && other.left && left->Equals(*other.left)) || !(left || other.left)) && ((right && other.right && right->Equals(*other.right)) || !(right || other.right));
}
size_t Term::GetComplexity(void) const {
    switch (type) {
        case Type::Symbol:
        case Type::Variable: return 1;
        case Type::Abstraction:
        case Type::Application:
        case Type::Equivalence: return left->GetComplexity() + right->GetComplexity();
        case Type::TypeDeclaration:
        case Type::Definition: return right->GetComplexity();
        default: return 0;
    }
}
Term* Term::Copy(void) const {
    return new Term(type, value, left ? left->Copy() : nullptr, right ? right->Copy() : nullptr);
}
Term* Term::ApplyEquivalence(const Term& other) const {
    if (other.type != Type::Equivalence) return nullptr;
    if (Equals(*other.left)) return other.right->Copy();
    if (Equals(*other.right)) return other.left->Copy();
    return nullptr;
}
Term* Term::Update(const MathLib::Function<Term*, const Term&>& func, bool applyToLeft) const {
    if (applyToLeft) return left ? new Term(type, value, func(*left), right->Copy()) : nullptr;
    return right ? new Term(type, value, left->Copy(), func(*right)) : nullptr;
}
Term* Term::IncrementVariables(size_t inc) const {
    if (type == Type::Variable) return new Term(type, value + inc);
    return new Term(type, value, left ? left->IncrementVariables(inc) : nullptr, right ? right->IncrementVariables(inc) : nullptr);
}