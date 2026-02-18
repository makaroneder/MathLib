#include "Term.hpp"

[[nodiscard]] MathLib::String WrapInParentheses(const MathLib::String& value, bool wrap) {
    return wrap ? '('_M + value + ')' : value;
}
Term::Term(void) : children(), name(), type(Type::None) {}
Term::Term(const MathLib::String& name) : children(), name(name), type(Type::Variable) {}
Term::Term(Type type, const MathLib::Array<Term>& children) : children(children), name(), type(type) {}
Term::Term(Type type, const Term& a, const Term& b) : children(MathLib::MakeArray<Term>(a, b)), name(), type(type) {}
MathLib::String Term::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + ToStringInternal(true);
}
bool Term::Contains(const Term& term) const {
    if (Equals(term)) return true;
    for (const Term& child : children) 
        if (child.Contains(term)) return true;
    return false;
}
Term Term::Replace(const Term& old, const Term& replacement) const {
    if (Equals(old)) return replacement;
    Term ret = *this;
    for (Term& child : ret.children) child = child.Replace(old, replacement);
    return ret;
}
bool Term::Equals(const Term& other) const {
    return type == other.type && name == other.name && children == other.children;
}
MathLib::String Term::ToStringInternal(bool root) const {
    switch (type) {
        case Type::Variable: return name;
        case Type::Comma: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + ", " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
        case Type::Assignment: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " = " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
        case Type::TypeDeclaration: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " : " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
        case Type::TypeAbstraction: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type > type) + " => " + children.At(1).ToStringInternal(children.At(1).type >= type), !root);
        case Type::TypeApplication: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " ^ " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
        case Type::Abstraction: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type > type) + " -> " + children.At(1).ToStringInternal(children.At(1).type >= type), !root);
        case Type::Application: return WrapInParentheses(children.At(0).ToStringInternal(children.At(0).type >= type) + " . " + children.At(1).ToStringInternal(children.At(1).type > type), !root);
        default: return "";
    }
}