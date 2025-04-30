#include "Formula.hpp"

Formula::Formula(void) : value(), children(), type() {}
Formula::Formula(bool value) : value(value ? "true" : "false"), children(), type(Type::Constant) {}
Formula::Formula(bool constant, const MathLib::Sequence<char>& value) : value(MathLib::CollectionToString(value)), children(), type(constant ? Type::Constant : Type::Variable) {}
Formula::Formula(const MathLib::Sequence<char>& value, const MathLib::Sequence<Formula>& children) : value(MathLib::CollectionToString(value)), children(MathLib::CollectionToArray<Formula>(children)), type(Type::Operation) {}
Formula::Formula(const MathLib::Sequence<char>& value, const Formula& left, const Formula& right) : value(MathLib::CollectionToString(value)), children(MathLib::MakeArray<Formula>(left, right)), type(Type::Operation) {}
Formula::Formula(const MathLib::Sequence<char>& value, const Formula& child) : value(MathLib::CollectionToString(value)), children(MathLib::MakeArray<Formula>(child)), type(Type::Operation) {}
bool Formula::IsTrue(void) const {
    return ToBool().GetOr(false);
}
MathLib::Expected<bool> Formula::ToBool(void) const {
    if (type != Type::Constant) return MathLib::Expected<bool>();
    else if (value == "true") return true;
    else if (value == "false") return false;
    else return MathLib::Expected<bool>();
}
bool Formula::operator==(const Formula& other) const {
    const size_t size = children.GetSize();
    if (type != other.type || value != other.value || size != other.children.GetSize()) return false;
    for (size_t i = 0; i < size; i++)
        if (children.At(i) != other.children.At(i)) return false;
    return true;
}
bool Formula::operator!=(const Formula& other) const {
    return !(*this == other);
}
MathLib::String Formula::ToString(const MathLib::Sequence<char>& padding) const {
    switch (type) {
        case Formula::Type::Constant:
        case Formula::Type::Variable: return MathLib::CollectionToString(padding) + value;
        case Formula::Type::Operation: {
            MathLib::String ret = MathLib::CollectionToString(padding) + value + '(';
            for (size_t i = 0; i < children.GetSize(); i++) {
                if (i) ret += ", ";
                ret += children.At(i).ToString();
            }
            return ret + ')';
        }
        default: return "";
    }
}