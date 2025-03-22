#include "BNFVariable.hpp"

BNFValue::BNFValue(void) {}
BNFValue::BNFValue(const MathLib::Sequence<char>& value, bool constant) : value(MathLib::CollectionToString(value)), children(), type(constant ? Type::Constant : Type::Variable) {}
BNFValue::BNFValue(const MathLib::Array<BNFValue>& children, bool array) : value(), children(children), type(array ? Type::Array : Type::Or) {}
bool BNFValue::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables, size_t& i) const {
    // FIXME:
    switch (type) {
        case Type::Constant: {
            const size_t size = value.GetSize();
            if (size + i < str.GetSize()) return false;
            for (size_t j = 0; j < size; j++)
                if (str.At(i++) != value.At(j)) return false;
            return true;
        }
        case Type::Variable: {
            const size_t size = variables.GetSize();
            for (size_t j = 0; j < size; j++) {
                const BNFVariable variable = variables.At(j);
                if (variable.name == value) return variable.value.Match(str, variables, i);
            }
            return false;
        }
        case Type::Array: {
            const size_t size = children.GetSize();
            for (size_t j = 0; j < size; j++)
                if (!children.At(j).Match(str, variables, i)) return false;
            return true;
        }
        case Type::Or: {
            const size_t size = children.GetSize();
            for (size_t j = 0; j < size; j++) {
                const size_t save = i;
                if (children.At(j).Match(str, variables, i)) return true;
                i = save;
            }
            return false;
        }
        default: return false;
    }
}
bool BNFValue::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    size_t i = 0;
    return Match(str, variables, i) && i == str.GetSize();
}
bool BNFValue::IsDependentOn(const MathLib::Sequence<char>& variable) const {
    switch (type) {
        case Type::Variable: return value == variable;
        case Type::Array:
        case Type::Or: {
            for (size_t i = 0; i < children.GetSize(); i++)
                if (children.At(i).IsDependentOn(variable)) return true;
            return false;
        }
        default: return false;
    }
}
BNFValue BNFValue::Substitute(const MathLib::Sequence<BNFVariable>& variables) const {
    switch (type) {
        case Type::Array:
        case Type::Or: {
            const size_t size = children.GetSize();
            BNFValue ret = *this;
            for (size_t i = 0; i < size; i++)
                ret.children.At(i) = ret.children.At(i).Substitute(variables);
            return ret;
        }
        case Type::Variable: {
            const size_t size = variables.GetSize();
            for (size_t i = 0; i < size; i++) {
                const BNFVariable variable = variables.At(i);
                if (variable.name == value) return variable.value;
            }
            return *this;
        }
        default: return *this;
    }
}
MathLib::String BNFValue::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    switch (type) {
        case Type::Constant: return padd + '"' + value + '"';
        case Type::Variable: return padd + '<' + value + '>';
        case Type::Array:
        case Type::Or: {
            MathLib::String ret = padd + '(';
            const MathLib::String separator = type == Type::Array ? " " : " | ";
            for (size_t i = 0; i < children.GetSize(); i++) {
                if (i) ret += separator;
                ret += children.At(i).ToString();
            }
            return ret + ')';
        }
        default: return "";
    }
}