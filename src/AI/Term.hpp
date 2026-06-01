#ifndef Term_H
#define Term_H
#include <Interfaces/Sequence/TransformSequence.hpp>
#include <Interfaces/Dictionary.hpp>
#include <FunctionT.hpp>

enum class TermType : uint8_t {
    Constant,
    Variable,
    Addition,
    Multiplication,
    Exponentiation,
    NaturalLogarithm,
};
template <typename T>
struct Term;
template <typename T>
struct Term : MathLib::Printable, MathLib::Comparable<Term<T>> {
    Term(void) : children(), value(0), type(TermType::Constant) {}
    Term(TermType type, const MathLib::Array<Term<T>>& children) : children(children), value(0), type(type) {}
    Term(TermType type, const Term<T>& a, const Term<T>& b) : children(MathLib::MakeArray<Term<T>>(a, b)), value(0), type(type) {}
    Term(TermType type, const Term<T>& child) : children(MathLib::MakeArray<Term<T>>(child)), value(0), type(type) {}
    Term(TermType type, const T& value) : children(), value(value), type(type) {}
    [[nodiscard]] Term<T> GetRest(void) const {
        const size_t size = children.GetSize();
        if (size <= 1) return Term<T>();
        Term<T> ret = children.AtUnsafe(1);
        for (size_t i = 2; i < size; i++) {
            switch (type) {
                case TermType::Addition: {
                    ret = ret.Add(children.AtUnsafe(i));
                    break;
                }
                case TermType::Multiplication: {
                    ret = ret.Mul(children.AtUnsafe(i));
                    break;
                }
                case TermType::Exponentiation: {
                    ret = ret.Exp(children.AtUnsafe(i));
                    break;
                }
                default: return Term<T>();
            }
        }
        return ret;
    }
    [[nodiscard]] Term<T> Add(const Term<T>& other) const {
        if (other.type == TermType::Constant && MathLib::FloatsEqual<T>(other.value, 0)) return *this;
        if (type == TermType::Constant) {
            if (MathLib::FloatsEqual<T>(value, 0)) return other;
            if (other.type == TermType::Constant) return Term<T>(TermType::Constant, value + other.value);
        }
        if (Equals(other)) return Mul(Term<T>(TermType::Constant, 2));
        if (type == TermType::Addition) {
            Term<T> ret = *this;
            return ret.children.Add(other) ? ret : Term<T>();
        }
        return Term<T>(TermType::Addition, *this, other);
    }
    [[nodiscard]] Term<T> Sub(const Term<T>& other) const {
        return Add(other.Mul(Term<T>(TermType::Constant, -1)));
    }
    [[nodiscard]] Term<T> Mul(const Term<T>& other) const {
        if (type == TermType::Constant) {
            if (MathLib::FloatsEqual<T>(value, 0)) return Term();
            if (MathLib::FloatsEqual<T>(value, 1)) return other;
            if (other.type == TermType::Constant) return Term<T>(TermType::Constant, value * other.value);
        }
        if (other.type == TermType::Constant) {
            if (MathLib::FloatsEqual<T>(other.value, 0)) return Term();
            if (MathLib::FloatsEqual<T>(other.value, 1)) return *this;
        }
        if (Equals(other)) return Exp(Term<T>(TermType::Constant, 2));
        if (other.type == TermType::Exponentiation) {
            if (Equals(other.children.AtUnsafe(0))) return Exp(Term<T>(TermType::Constant, 1).Add(other.GetRest()));
            if (type == TermType::Exponentiation && children.AtUnsafe(0).Equals(other.children.AtUnsafe(0))) return Exp(GetRest().Add(other.GetRest()));
        }
        if (type == TermType::Multiplication) {
            Term<T> ret = *this;
            return ret.children.Add(other) ? ret : Term<T>();
        }
        return Term<T>(TermType::Multiplication, *this, other);
    }
    [[nodiscard]] Term<T> Div(const Term<T>& other) const {
        return Mul(other.Exp(Term<T>(TermType::Constant, -1)));
    }
    [[nodiscard]] Term<T> Exp(const Term<T>& other) const {
        if (type == TermType::Constant && (MathLib::FloatsEqual<T>(value, 0) || MathLib::FloatsEqual<T>(value, 1))) return *this;
        if (other.type == TermType::Constant) {
            if (MathLib::FloatsEqual<T>(other.value, 0)) return Term<T>(TermType::Constant, 1);
            if (MathLib::FloatsEqual<T>(other.value, 1)) return *this;
            if (type == TermType::Constant) return Term<T>(TermType::Constant, MathLib::Pow(value, other.value));
        }
        if (type == TermType::Exponentiation) return children.At(0).Exp(GetRest().Exp(other));
        return Term<T>(TermType::Exponentiation, *this, other);
    }
    bool GetDerivatives(MathLib::Dictionary<T, Term<T>>& derivatives) const {
        const size_t size = children.GetSize();
        const auto derivative = MathLib::MakeFunctionT<MathLib::Dictionary<T, Term<T>>, Term<T>>([](Term<T> term) -> MathLib::Dictionary<T, Term<T>> {
            MathLib::Dictionary<T, Term<T>> ret;
            return term.GetDerivatives(ret) ? ret : MathLib::Dictionary<T, Term<T>>();
        });
        const auto transform = MathLib::TransformSequence<MathLib::Dictionary<T, Term<T>>, Term<T>>(
            children, derivative
        );
        switch (type) {
            case TermType::Constant: return true;
            case TermType::Variable: return derivatives.AddOrReplace(value, Term<T>(TermType::Constant, 1));
            case TermType::Addition: {
                const auto func = MathLib::MakeFunctionT<Term<T>, Term<T>, Term<T>>([](Term<T> a, Term<T> b) -> Term<T> {
                    return a.Add(b);
                });
                return derivatives.CombineSequence(transform, func, Term<T>());
            }
            case TermType::Multiplication: {
                Term<T> mul = children.At(0);
                for (size_t i = 1; i < size; i++) mul = mul.Mul(children.AtUnsafe(i));
                const auto map = MathLib::MakeFunctionT<MathLib::Dictionary<T, Term<T>>, Term<T>>([&derivative, &mul](Term<T> term) -> MathLib::Dictionary<T, Term<T>> {
                    const auto mulFunc = MathLib::MakeFunctionT<Term<T>, Term<T>>([&mul, &term](Term<T> derivative) -> Term<T> {
                        return derivative.Mul(mul).Div(term);
                    });
                    MathLib::Dictionary<T, Term<T>> ret = derivative(term);
                    ret.Map(mulFunc);
                    return ret;
                });
                const auto transform2 = MathLib::TransformSequence<MathLib::Dictionary<T, Term<T>>, Term<T>>(
                    children, map
                );
                const auto func = MathLib::MakeFunctionT<Term<T>, Term<T>, Term<T>>([](Term<T> a, Term<T> b) -> Term<T> {
                    return a.Add(b);
                });
                return derivatives.CombineSequence(transform2, func, Term<T>());
            }
            case TermType::Exponentiation: {
                const Term<T> base = children.At(0);
                if (!base.GetDerivatives(derivatives)) return false;
                const Term<T> exp = GetRest();
                MathLib::Dictionary<T, Term<T>> dexp;
                if (!exp.GetDerivatives(dexp)) return false;
                const auto func = MathLib::MakeFunctionT<Term<T>, Term<T>, Term<T>>([this, &base, &exp](Term<T> a, Term<T> b) -> Term<T> {
                    return Mul(a.Mul(exp).Div(base).Add(b.Mul(Term<T>(TermType::NaturalLogarithm, base))));
                });
                return derivatives.Combine(dexp, func, Term<T>());
            }
            case TermType::NaturalLogarithm: {
                const Term<T> base = children.At(0);
                if (!base.GetDerivatives(derivatives)) return false;
                const auto func = MathLib::MakeFunctionT<Term<T>, Term<T>>([&base](Term<T> term) -> Term<T> {
                    return term.Div(base);
                });
                derivatives.Map(func);
                return true;
            }
            default: return false;
        }
    }
    Term<T> Simplify(void) const {
        Term<T> ret = *this;
        for (Term<T>& term : ret.children) term = term.Simplify();
        const size_t size = ret.children.GetSize();
        switch (type) {
            case TermType::Addition: {
                Term<T> curr = ret.children.At(0);
                for (size_t i = 1; i < size; i++) curr = curr.Add(ret.children.At(i));
                return curr;
            }
            case TermType::Multiplication: {
                Term<T> curr = ret.children.At(0);
                for (size_t i = 1; i < size; i++) curr = curr.Mul(ret.children.At(i));
                return curr;
            }
            case TermType::Exponentiation: {
                Term<T> curr = ret.children.At(0);
                for (size_t i = 1; i < size; i++) curr = curr.Exp(ret.children.At(i));
                return curr;
            }
            default: return ret;
        }
    }
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const {
        switch (type) {
            case TermType::Constant: return MathLib::ToString(value);
            case TermType::Variable: return "x_"_M + MathLib::ToString(value);
            case TermType::Addition:
            case TermType::Multiplication:
            case TermType::Exponentiation: {
                MathLib::String op;
                switch (type) {
                    case TermType::Addition: {
                        op = " + ";
                        break;
                    }
                    case TermType::Multiplication: {
                        op = " * ";
                        break;
                    }
                    case TermType::Exponentiation: {
                        op = " ^ ";
                        break;
                    }
                    default: return "";
                }
                const size_t size = children.GetSize();
                MathLib::String ret;
                for (size_t i = 0; i < size; i++) {
                    if (i) ret += op;
                    ret += children.AtUnsafe(i).ToStringInternal(false);
                }
                return root ? ret : '('_M + ret + ')';
            }
            case TermType::NaturalLogarithm: return "ln("_M + children.At(0).ToStringInternal(true) + ')';
            default: return "";
        }
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        return MathLib::CollectionToString(padding) + ToStringInternal(true);
    }

    protected:
    [[nodiscard]] virtual bool Equals(const Term<T>& other) const override {
        return type == other.type && MathLib::FloatsEqual<T>(value, other.value) && children == other.children;
    }

    private:
    MathLib::Array<Term<T>> children;
    T value;
    TermType type;
};
namespace MathLib {
    MakeTemplatedFormatter(Term<T>, self, padding, { return self.ToString(padding); }, typename T);
}

#endif