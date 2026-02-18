#include "Formula.hpp"
#include <Interfaces/PartialFunctionApplication.hpp>
#include <FunctionPointer.hpp>
#include <FunctionT.hpp>

Formula::Formula(void) : value(), name(), children(), type(Type::None) {}
Formula::Formula(const RationalNumber& value) : value(value), name(), children(), type(Type::Constant) {}
Formula::Formula(const MathLib::Sequence<char>& name) : value(), name(MathLib::CollectionToString(name)), children(), type(Type::Variable) {}
Formula::Formula(Type type, const Formula& a, const Formula& b) : value(), name(), children(MathLib::MakeArray<Formula>(a, b)), type(type) {}
Formula::Formula(Type type, const MathLib::Sequence<Formula>& children) : value(), name(), children(MathLib::CollectionToArray<Formula>(children)), type(type) {}
Formula Formula::MakeAdd(Formula a, Formula b) {
    a = a.Simplify();
    b = b.Simplify();
    if (a.type == Type::None || b.type == Type::None) return Formula();
    if (b.type == Type::Constant) MathLib::Swap<Formula>(a, b);
    if (a.type == Type::Constant) {
        if (a.value == RationalNumber()) return b;
        if (b.type == Type::Constant) return Formula(a.value + b.value);
        if (b.type == Type::Add) for (uint8_t i = 0; i < 2; i++)
            if (b.children.AtUnsafe(i).type == Type::Constant)
                return MakeAdd(b.children.AtUnsafe(!i), Formula(a.value + b.children.AtUnsafe(i).value));
    }
    if (b.type == Type::Sub) MathLib::Swap<Formula>(a, b);
    if (a.type == Type::Sub && a.children.At(1) == b) return a.children.At(0);
    return Formula(Type::Add, a, b);
}
Formula Formula::MakeSub(Formula a, Formula b) {
    a = a.Simplify();
    b = b.Simplify();
    if (a.type == Type::None || b.type == Type::None) return Formula();
    if (a == b) return RationalNumber();
    if (b.type == Type::Constant) {
        if (b.value == RationalNumber()) return a;
        if (a.type == Type::Constant) return Formula(a.value - b.value);
        if (a.type == Type::Add) for (uint8_t i = 0; i < 2; i++)
            if (a.children.AtUnsafe(i).type == Type::Constant)
                return MakeAdd(a.children.AtUnsafe(!i), Formula(a.children.AtUnsafe(i).value - b.value));
    }
    return Formula(Type::Sub, a, b);
}
Formula Formula::MakeMul(Formula a, Formula b) {
    a = a.Simplify();
    b = b.Simplify();
    if (a.type == Type::None || b.type == Type::None) return Formula();
    if (a == b) return MakeSquare(a);
    if (b.type == Type::Constant) MathLib::Swap<Formula>(a, b);
    if (a.type == Type::Constant) {
        if (a.value == RationalNumber()) return a;
        if (a.value == RationalNumber(NaturalNumber::FromT<uint8_t>(1))) return b;
        if (b.type == Type::Constant) return Formula(a.value * b.value);
        if (b.type == Type::Mul) for (uint8_t i = 0; i < 2; i++)
            if (b.children.AtUnsafe(i).type == Type::Constant)
                return MakeMul(b.children.AtUnsafe(!i), Formula(a.value * b.children.AtUnsafe(i).value));
    }
    if (b.type == Type::Div) MathLib::Swap<Formula>(a, b);
    if (a.type == Type::Div && a.children.At(1) == b) return a.children.At(0);
    if (b.type == Type::Add || b.type == Type::Sub) MathLib::Swap<Formula>(a, b);
    if (a.type == Type::Add || a.type == Type::Sub) {
        const Formula x = MakeMul(b, a.children.At(0));
        const Formula y = MakeMul(b, a.children.At(1));
        return a.type == Type::Add ? MakeAdd(x, y) : MakeSub(x, y);
    }
    return Formula(Type::Mul, a, b);
}
Formula Formula::MakeDiv(Formula a, Formula b) {
    a = a.Simplify();
    b = b.Simplify();
    if (a.type == Type::None || b.type == Type::None) return Formula();
    if (a == b) return RationalNumber(NaturalNumber::FromT<uint8_t>(1));
    if (b.type == Type::Constant) {
        if (b.value == RationalNumber(NaturalNumber::FromT<uint8_t>(1))) return a;
        if (a.type == Type::Constant) return Formula(a.value / b.value);
    }
    if (a.type == Type::Constant && a.value == RationalNumber(NaturalNumber())) return a;
    return Formula(Type::Div, a, b);
}
Formula Formula::MakeSquare(Formula base) {
    base = base.Simplify();
    if (base.type == Type::None) return Formula();
    if (base.type == Type::Constant) return Formula(base.value * base.value);
    if (base.type == Type::Add || base.type == Type::Sub) {
        const Formula a = MakeAdd(MakeSquare(base.children.At(0)), MakeSquare(base.children.At(1)));
        const Formula b = MakeMul(RationalNumber(NaturalNumber::FromT<uint8_t>(2)), MakeMul(base.children.At(0), base.children.At(1)));
        return base.type == Type::Add ? MakeAdd(a, b) : MakeSub(a, b);
    }
    return Formula(Type::Square, MathLib::MakeArray<Formula>(base));
}
MathLib::String Formula::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + ToStringInternal(true);
}
size_t Formula::GetComplexity(void) const {
    size_t ret = 1;
    for (const Formula& child : children) ret += child.GetComplexity();
    return ret;
}
Formula Formula::Simplify(void) const {
    switch (type) {
        case Type::Add: return MakeAdd(children.At(0), children.At(1));
        case Type::Sub: return MakeSub(children.At(0), children.At(1));
        case Type::Mul: return MakeMul(children.At(0), children.At(1));
        case Type::Div: return MakeDiv(children.At(0), children.At(1));
        case Type::Square: return MakeSquare(children.At(0));
        default: return *this;
    }
}
Formula Formula::Evaluate(MathLib::Dictionary<MathLib::String, Formula>& derivatives) const {
    derivatives.FillValues(Formula());
    return EvaluateInternal(derivatives);
}
Formula Formula::Substitute(const MathLib::Dictionary<MathLib::String, Formula>& variables) const {
    if (type == Type::Variable) {
        const MathLib::Expected<Formula> tmp = variables.Get(name);
        return tmp.HasValue() ? tmp.Get() : *this;
    }
    Formula ret = *this;
    for (Formula& child : ret.children) child = child.Substitute(variables);
    return ret;
}
bool Formula::Equals(const Formula& other) const {
    return type == other.type && name == other.name && value == other.value && children == other.children;
}
Formula Formula::EvaluateInternal(MathLib::Dictionary<MathLib::String, Formula>& derivatives) const {
    switch (type) {
        case Type::Constant: return *this;
        case Type::Variable: return derivatives.AddOrReplace(MathLib::DictionaryElement<MathLib::String, Formula>(name, RationalNumber(NaturalNumber::FromT<uint8_t>(1)))) ? *this : Formula();
        case Type::Add: {
            const Formula output1 = children.At(0).EvaluateInternal(derivatives);
            MathLib::Dictionary<MathLib::String, Formula> tmp;
            const Formula output2 = children.At(1).EvaluateInternal(tmp);
            return derivatives.Combine<Formula>(tmp, MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeAdd), Formula(RationalNumber())) ? MakeAdd(output1, output2) : Formula();
        }
        case Type::Sub: {
            const Formula output1 = children.At(0).EvaluateInternal(derivatives);
            MathLib::Dictionary<MathLib::String, Formula> tmp;
            const Formula output2 = children.At(1).EvaluateInternal(tmp);
            return derivatives.Combine<Formula>(tmp, MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeSub), Formula(RationalNumber())) ? MakeSub(output1, output2) : Formula();
        }
        case Type::Mul: {
            const MathLib::FunctionPointer<Formula, Formula, Formula> func = MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeMul);
            const Formula output1 = children.At(0).EvaluateInternal(derivatives);
            MathLib::Dictionary<MathLib::String, Formula> tmp;
            const Formula output2 = children.At(1).EvaluateInternal(tmp);
            derivatives.Map(MathLib::PartialFunctionApplication<Formula, Formula, Formula>(func, output2));
            tmp.Map(MathLib::PartialFunctionApplication<Formula, Formula, Formula>(func, output1));
            return derivatives.Combine<Formula>(tmp, MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeAdd), Formula(RationalNumber())) ? MakeMul(output1, output2) : Formula();
        }
        case Type::Div: {
            const MathLib::FunctionPointer<Formula, Formula, Formula> func = MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeMul);
            const Formula output1 = children.At(0).EvaluateInternal(derivatives);
            MathLib::Dictionary<MathLib::String, Formula> tmp;
            const Formula output2 = children.At(1).EvaluateInternal(tmp);
            derivatives.Map(MathLib::PartialFunctionApplication<Formula, Formula, Formula>(func, output2));
            tmp.Map(MathLib::PartialFunctionApplication<Formula, Formula, Formula>(func, output1));
            if (!derivatives.Combine<Formula>(tmp, MathLib::FunctionPointer<Formula, Formula, Formula>(&MakeSub), Formula(RationalNumber()))) return Formula();
            derivatives.Map(MathLib::MakeFunctionT<Formula, Formula>([&output2](Formula a) -> Formula {
                return MakeDiv(a, MakeMul(output2, output2));
            }));
            return MakeDiv(output1, output2);
        }
        case Type::Square: {
            const Formula ret = children.At(0).EvaluateInternal(derivatives);
            derivatives.Map(MathLib::MakeFunctionT<Formula, Formula>([](Formula a) -> Formula {
                return MakeMul(RationalNumber(NaturalNumber::FromT<uint8_t>(2)), a);
            }));
            return MakeSquare(ret);
        }
        default: return Formula();
    }
}
MathLib::String Formula::ToStringInternal(bool root) const {
    switch (type) {
        case Type::Constant: return value.ToString();
        case Type::Variable: return name;
        case Type::Add:
        case Type::Sub:
        case Type::Mul:
        case Type::Div: {
            const char operation[] = {
                '+', '-', '*', '/',
            };
            const MathLib::String ret = children.At(0).ToStringInternal(children.At(0).type >= type) + ' ' + operation[(uint8_t)type - (uint8_t)Type::Add] + ' ' + children.At(1).ToStringInternal(children.At(1).type >= type);
            return root ? ret : '('_M + ret + ')';
        }
        case Type::Square: return children.At(0).ToStringInternal(false) + "^2";
        default: return "";
    }
}