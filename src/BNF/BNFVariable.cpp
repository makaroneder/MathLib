#include "BNFVariable.hpp"

BNFVariable::BNFVariable(void) {}
BNFVariable::BNFVariable(const MathLib::Sequence<char>& name, const BNFValue& value) : value(value), name(MathLib::CollectionToString(name)) {}
bool BNFVariable::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    return value.Match(str, variables);
}
bool BNFVariable::IsRecursive(void) const {
    return value.IsDependentOn(name);
}
BNFVariable BNFVariable::Substitute(const MathLib::Sequence<BNFVariable>& variables) const {
    return BNFVariable(name, value.Substitute(variables));
}
MathLib::String BNFVariable::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + '<' + name + "> ::= " + value.ToString();
}