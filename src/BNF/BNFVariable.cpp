#include "BNFVariable.hpp"

BNFVariable::BNFVariable(void) {}
BNFVariable::BNFVariable(const MathLib::Sequence<char>& name, const BNFValue& value) : value(value), name(MathLib::CollectionToString(name)) {}
bool BNFVariable::CheckName(const MathLib::Sequence<char>& name_) const {
    return name == name_;
}
MathLib::String BNFVariable::GetName(void) const {
    return name;
}
BNFValue BNFVariable::GetValue(void) const {
    return value;
}
bool BNFVariable::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    return value.Match(str, variables);
}
MathLib::Tree<MathLib::String> BNFVariable::BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    MathLib::Tree<MathLib::String> ret = value.BuildAST(str, variables);
    ret.name = name;
    return ret;
}
size_t BNFVariable::GetDependenceCount(const MathLib::Sequence<char>& variable, const MathLib::Sequence<BNFVariable>& variables) const {
    return value.GetDependenceCount(variable, variables);
}
bool BNFVariable::IsRecursive(const MathLib::Sequence<BNFVariable>& variables) const {
    return value.GetDependenceCount(name, variables);
}
BNFVariable BNFVariable::Substitute(const BNFVariable& variable) const {
    return BNFVariable(name, value.Substitute(variable));
}
BNFVariable BNFVariable::Substitute(const MathLib::Sequence<BNFVariable>& variables) const {
    return BNFVariable(name, value.Substitute(variables));
}
BNFVariable BNFVariable::Simplify(void) const {
    return BNFVariable(name, value.Simplify());
}
size_t BNFVariable::GetDataSize(size_t variableSize) const {
    return value.GetDataSize(variableSize);
}
MathLib::String BNFVariable::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + '<' + name + "> ::= " + value.ToString();
}
bool BNFVariable::operator==(const BNFVariable& other) const {
    return name == other.name && value == other.value;
}
bool BNFVariable::operator!=(const BNFVariable& other) const {
    return !(*this == other);
}