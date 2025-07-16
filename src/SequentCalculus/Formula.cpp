#include "Formula.hpp"

Formula::Formula(void) : value(), children(), type(Type::None) {}
Formula::Formula(const MathLib::Sequence<char>& value) : value(MathLib::CollectionToString(value)), children(), type(Type::Variable) {}
Formula::Formula(Type type, const MathLib::Sequence<Formula>& children) : value(), children(MathLib::CollectionToArray<Formula>(children)), type(type) {}
MathLib::String Formula::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + ToStringInternal(true);
}
bool Formula::operator==(const Formula& other) const {
    return type == other.type && value == other.value && children == other.children;
}
bool Formula::operator!=(const Formula& other) const {
    return !(*this == other);
}
MathLib::String Formula::ToStringInternal(bool root) const {
    switch (type) {
        case Type::Variable: return value;
        case Type::Conjunction: {
            const MathLib::String ret = children.At(0).ToStringInternal(false) + " && " + children.At(1).ToStringInternal(false);
            return root ? ret : ('('_M + ret + ')');
        }
        case Type::Disjunction: {
            const MathLib::String ret = children.At(0).ToStringInternal(false) + " || " + children.At(1).ToStringInternal(false);
            return root ? ret : ('('_M + ret + ')');
        }
        case Type::Implication: {
            const MathLib::String ret = children.At(0).ToStringInternal(false) + " -> " + children.At(1).ToStringInternal(false);
            return root ? ret : ('('_M + ret + ')');
        }
        case Type::Negation: return '!'_M + children.At(0).ToStringInternal(false);
        default: return "";
    }
}