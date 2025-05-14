#include "LambdaTerm.hpp"

LambdaTerm::LambdaTerm(void) : value(), children(), type(Type::None) {}
LambdaTerm::LambdaTerm(const MathLib::Sequence<char>& value) : value(MathLib::CollectionToString(value)), children(), type(Type::Variable) {}
LambdaTerm::LambdaTerm(const LambdaTerm& func, const LambdaTerm& arg) : value(), children(MathLib::MakeArray<LambdaTerm>(func, arg)), type(Type::Application) {}
LambdaTerm::LambdaTerm(const LambdaTerm& func, const MathLib::Sequence<char>& arg) : value(MathLib::CollectionToString(arg)), children(MathLib::MakeArray<LambdaTerm>(func)), type(Type::Abstraction) {}
LambdaTerm LambdaTerm::Apply(const LambdaTerm& arg) const {
    return type == Type::Abstraction ? LambdaTerm(children.At(0).ApplyInternal(arg, value)) : LambdaTerm(*this, arg);
}
MathLib::String LambdaTerm::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + ToStringInternal(true);
}
bool LambdaTerm::Equals(const MathLib::Comparable& other_) const {
    const LambdaTerm& other = (const LambdaTerm&)other_;
    return type == other.type && value == other.value && children == other.children;
}
LambdaTerm LambdaTerm::ApplyInternal(const LambdaTerm& val, const MathLib::Sequence<char>& arg) const {
    switch (type) {
        case Type::Variable: return value == arg ? val : *this;
        case Type::Application: return children.At(0).ApplyInternal(val, arg).Apply(children.At(1).ApplyInternal(val, arg));
        case Type::Abstraction: return value == arg ? *this : LambdaTerm(children.At(0).ApplyInternal(val, arg), value);
        default: return *this;
    }
}
MathLib::String LambdaTerm::ToStringInternal(bool root) const {
    switch (type) {
        case Type::Variable: return value;
        case Type::Application: {
            const MathLib::String ret = children.At(0).ToStringInternal(children.At(0).type == Type::Application) + ' ' + children.At(1).ToStringInternal(false);
            return root ? ret : '('_M + ret + ')';
        }
        case Type::Abstraction: {
            const MathLib::String ret = 'l'_M + value + '.' + children.At(0).ToStringInternal(true);
            return root ? ret : '('_M + ret + ')';
        }
        default: return "None";
    }
}