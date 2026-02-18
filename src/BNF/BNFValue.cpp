#include "BNFVariable.hpp"

BNFValue::BNFValue(void) {}
BNFValue::BNFValue(const MathLib::Interval<char>& range) : value(MathLib::String(range.GetMin()) + range.GetMax()), children(), type(Type::Range) {}
BNFValue::BNFValue(const MathLib::Sequence<char>& value, bool constant) : value(MathLib::CollectionToString(value)), children(), type(constant ? Type::Constant : Type::Variable) {}
BNFValue::BNFValue(const MathLib::Array<BNFValue>& children, bool array) : value(), children(children), type(array ? Type::Array : Type::Or) {}
BNFValue::BNFValue(Type type, const BNFValue& child) : value(), children(MathLib::MakeArray<BNFValue>(child)), type(type) {}
bool BNFValue::IsConstant(void) const {
    return type == BNFValue::Type::Constant;
}
MathLib::String BNFValue::GetValue(void) const {
    return value;
}
bool BNFValue::Add(const BNFValue& child) {
    return children.Add(child);
}
bool BNFValue::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables, size_t& i, MathLib::Tree<MathLib::String>& tree) const {
    switch (type) {
        case Type::Constant: {
            const size_t size = value.GetSize();
            if (size + i > str.GetSize()) return false;
            for (size_t j = 0; j < size; j++) {
                tree.data += str.At(i);
                if (str.At(i++) != value.At(j)) return false;
            }
            return true;
        }
        case Type::Variable: {
            const size_t size = variables.GetSize();
            for (size_t j = 0; j < size; j++) {
                const BNFVariable variable = variables.At(j);
                if (variable.CheckName(value)) {
                    MathLib::Tree<MathLib::String> tmp = MathLib::Tree<MathLib::String>(value, "");
                    return variable.GetValue().Match(str, variables, i, tmp) && tree.Add(tmp);
                }
            }
            return false;
        }
        case Type::Array: {
            const size_t size = children.GetSize();
            for (size_t j = 0; j < size; j++)
                if (!children.At(j).Match(str, variables, i, tree)) return false;
            return true;
        }
        case Type::Or: {
            const size_t size = children.GetSize();
            for (size_t j = 0; j < size; j++) {
                MathLib::Tree<MathLib::String> tmp = tree;
                const size_t save = i;
                if (children.At(j).Match(str, variables, i, tmp)) {
                    tree = tmp;
                    return true;
                }
                i = save;
            }
            return false;
        }
        case Type::Optional: {
            const size_t save = i;
            if (children.At(0).Match(str, variables, i, tree)) return true;
            i = save;
            return true;
        }
        case Type::KleeneStar:
        case Type::KleenePlus: {
            const bool min = type == Type::KleenePlus;
            bool match = false;
            while (true) {
                const size_t save = i;
                if (!children.At(0).Match(str, variables, i, tree)) {
                    i = save;
                    if (min && !match) return false;
                    return true;
                }
                else match = true;
            }
        }
        case Type::Range: {
            if (i >= str.GetSize()) return false;
            const char chr = str.At(i++);
            tree.data += chr;
            return IsBetween(chr, value.At(0), value.At(1));
        }
        default: return false;
    }
}
bool BNFValue::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    size_t i = 0;
    MathLib::Tree<MathLib::String> tree;
    return Match(str, variables, i, tree) && i == str.GetSize();
}
MathLib::Tree<MathLib::String> BNFValue::BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const {
    size_t i = 0;
    MathLib::Tree<MathLib::String> tree;
    return Match(str, variables, i, tree) && i == str.GetSize() ? tree : MathLib::Tree<MathLib::String>();
}
size_t BNFValue::GetDependenceCount(const MathLib::Sequence<char>& variable, const MathLib::Sequence<BNFVariable>& variables) const {
    switch (type) {
        case Type::Variable: {
            if (value == variable) return 1;
            const size_t size = variables.GetSize();
            for (size_t j = 0; j < size; j++) {
                const BNFVariable var = variables.At(j);
                if (var.CheckName(value)) return var.GetValue().GetDependenceCount(variable, variables);
            }
            return 0;
        }
        case Type::Array:
        case Type::Optional:
        case Type::KleeneStar:
        case Type::KleenePlus:
        case Type::Or: {
            const size_t size = children.GetSize();
            size_t ret = 0;
            for (size_t i = 0; i < size; i++)
                ret += children.At(i).GetDependenceCount(variable, variables);
            return ret;
        }
        default: return 0;
    }
}
BNFValue BNFValue::Substitute(const BNFVariable& variable) const {
    switch (type) {
        case Type::Array:
        case Type::Optional:
        case Type::KleeneStar:
        case Type::KleenePlus:
        case Type::Or: {
            const size_t size = children.GetSize();
            BNFValue ret = *this;
            for (size_t i = 0; i < size; i++)
                ret.children.At(i) = ret.children.At(i).Substitute(variable);
            return ret;
        }
        case Type::Variable: return variable.CheckName(value) ? variable.GetValue() : *this;
        default: return *this;
    }
}
BNFValue BNFValue::Substitute(const MathLib::Sequence<BNFVariable>& variables) const {
    switch (type) {
        case Type::Array:
        case Type::Optional:
        case Type::KleeneStar:
        case Type::KleenePlus:
        case Type::Or: {
            const size_t size = children.GetSize();
            BNFValue ret = *this;
            for (size_t i = 0; i < size; i++)
                ret.children.At(i) = ret.children.At(i).Substitute(variables);
            return ret;
        }
        case Type::Variable: {
            const size_t size = variables.GetSize();
            for (size_t i = 0; i < size; i++)
                if (variables.At(i).CheckName(value)) return variables.At(i).GetValue().Substitute(variables);
            return *this;
        }
        default: return *this;
    }
}
BNFValue BNFValue::Simplify(void) const {
    switch (type) {
        case Type::Array: {
            const size_t size = children.GetSize();
            BNFValue ret;
            MathLib::String prev;
            bool combinable = true;
            for (size_t i = 0; i < size; i++) {
                const BNFValue tmp = children.At(i).Simplify();
                if (tmp.IsConstant()) prev += tmp.value;
                else {
                    if (tmp.type != BNFValue::Type::Array) combinable = false;
                    if ((!prev.IsEmpty() && !ret.Add(BNFValue(prev, true))) || !ret.Add(tmp)) return BNFValue();
                    prev = "";
                }
            }
            if (!prev.IsEmpty() && !ret.Add(BNFValue(prev, true))) return BNFValue();
            const size_t tmp = ret.children.GetSize();
            if (!tmp) return BNFValue();
            if (tmp == 1) return ret.children.At(0);
            if (combinable) {
                MathLib::Array<BNFValue> values;
                for (size_t i = 0; i < size; i++) {
                    if (children.At(i).IsConstant()) values += children.At(i);
                    else values += children.At(i).children;
                }
                return BNFValue(values, true).Simplify();
            }
            ret.type = BNFValue::Type::Array;
            return ret;
        }
        case Type::Or: {
            const size_t size = children.GetSize();
            BNFValue ret;
            for (size_t i = 0; i < size; i++) {
                const BNFValue tmp = children.At(i).Simplify();
                if (!ret.children.Contains(tmp) && !ret.Add(tmp)) return BNFValue();
            }
            return ret;
        }
        case Type::Optional:
        case Type::KleeneStar:
        case Type::KleenePlus: {
            const size_t size = children.GetSize();
            BNFValue ret = *this;
            for (size_t i = 0; i < size; i++)
                ret.children.At(i) = ret.children.At(i).Simplify();
            return ret;
        }
        default: return *this;
    }
}
size_t BNFValue::GetDataSize(size_t variableSize) const {
    switch (type) {
        case Type::Constant: return value.GetSize();
        case Type::Variable: return variableSize;
        case Type::Range: return 1;
        case Type::Array:
        case Type::Or:
        case Type::Optional:
        case Type::KleeneStar:
        case Type::KleenePlus: {
            size_t ret = 0;
            for (size_t i = 0; i < children.GetSize(); i++)
                ret += children.At(i).GetDataSize(variableSize);
            return ret;
        }
        default: return 0;
    }
}
MathLib::String BNFValue::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    switch (type) {
        case Type::Constant: {
            MathLib::String ret = padd + '"';
            for (const char& chr : value) {
                switch (chr) {
                    case '\\': {
                        ret += "\\\\";
                        break;
                    }
                    case '\t': {
                        ret += "\\t";
                        break;
                    }
                    case '\n': {
                        ret += "\\n";
                        break;
                    }
                    case '\r': {
                        ret += "\\r";
                        break;
                    }
                    default: ret += chr;
                }
            }
            return ret + '"';
        }
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
        case Type::Optional: return padd + '[' + children.At(0).ToString() + ']';
        case Type::KleeneStar: return padd + children.At(0).ToString() + '*';
        case Type::KleenePlus: return padd + children.At(0).ToString() + '+';
        case Type::Range: return padd + '"' + value.At(0) + "\" ... \"" + value.At(1) + '"';
        default: return "";
    }
}
bool BNFValue::operator==(const BNFValue& other) const {
    if (type != other.type || value != other.value) return false;
    const size_t size = children.GetSize();
    for (size_t i = 0; i < size; i++)
        if (children.At(i) != other.children.At(i)) return false;
    return true;
}
bool BNFValue::operator!=(const BNFValue& other) const {
    return !(*this == other);
}