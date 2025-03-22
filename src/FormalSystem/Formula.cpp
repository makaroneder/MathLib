#include "Formula.hpp"

Formula::Formula(void) : type(), value(), children() {}
Formula::Formula(bool value) : type(Type::Constant), value(value ? "true" : "false"), children() {}
Formula::Formula(bool constant, const MathLib::Sequence<char>& value) : type(constant ? Type::Constant : Type::Variable), value(MathLib::CollectionToString(value)), children() {}
Formula::Formula(const MathLib::Sequence<char>& value, const MathLib::Sequence<Formula>& children) : type(Type::Operation), value(MathLib::CollectionToString(value)), children(MathLib::CollectionToArray<Formula>(children)) {}
Formula::Formula(const MathLib::Sequence<char>& value, const Formula& left, const Formula& right) : type(Type::Operation), value(MathLib::CollectionToString(value)), children(MathLib::MakeArray<Formula>(left, right)) {}
Formula::Formula(const MathLib::Sequence<char>& value, const Formula& child) : type(Type::Operation), value(MathLib::CollectionToString(value)), children(MathLib::MakeArray<Formula>(child)) {}
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