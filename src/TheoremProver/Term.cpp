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
bool Term::Match(const Term& other, MathLib::Dictionary<size_t, Term*>& variables, size_t depth) const {
    if (type == Type::Variable && value >= depth) {
        const size_t index = value - depth;
        const MathLib::Expected<Term*> tmp = variables.Get(index);
        return tmp.HasValue() ? tmp.Get()->Equals(other) : variables.Add(index, other.Copy());
    }
    if (type != other.type) return false;
    if (type == Type::Symbol && value != other.value) return false;
    if ((left && !other.left) || (!left && other.left)) return false;
    if ((right && !other.right) || (!right && other.right)) return false;
    if (left && !left->Match(*other.left, variables, depth)) return false;
    if (right && !right->Match(*other.right, variables, depth + (type == Type::Abstraction))) return false;
    return true;
}
Term* Term::Substitute(const MathLib::Dictionary<size_t, Term*>& variables, size_t depth) const {
    if (type == Type::Variable && value >= depth) {
        const MathLib::Expected<Term*> tmp = variables.Get(value - depth);
        if (tmp.HasValue()) return tmp.Get()->Copy();
    }
    return new Term(type, value, left ? left->Substitute(variables, depth) : nullptr, right ? right->Substitute(variables, depth + (type == Type::Abstraction)) : nullptr);
}
Term* Term::ApplyEquivalence(const Term& other) const {
    if (other.type != Type::Equivalence) return nullptr;
    MathLib::Dictionary<size_t, Term*> variables;
    Term* newValue = nullptr;
    if (other.left->Match(*this, variables, 0)) newValue = other.right;
    else {
        variables.Map(MathLib::MakeFunctionT<Term*, Term*>([](Term* term) -> Term* {
            if (term) delete term;
            return nullptr;
        }));
        variables = MathLib::Dictionary<size_t, Term*>();
        if (other.right->Match(*this, variables, 0)) newValue = other.left;
        else return nullptr;
    }
    Term* const ret = newValue->Substitute(variables, 0);
    variables.Map(MathLib::MakeFunctionT<Term*, Term*>([](Term* term) -> Term* {
        if (term) delete term;
        return nullptr;
    }));
    return ret;
}
Term* Term::Update(const MathLib::Function<Term*, const Term&>& func, bool applyToLeft) const {
    if (applyToLeft) return left ? new Term(type, value, func(*left), right->Copy()) : nullptr;
    return right ? new Term(type, value, left->Copy(), func(*right)) : nullptr;
}