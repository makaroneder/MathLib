#include <Host.hpp>
#include <Interfaces/Comparable.hpp>
#include <iostream>

struct Term : MathLib::Comparable, MathLib::Printable {
    // TODO: Set theory
    enum class Type : uint8_t {
        None,
        LogicValue, Variable,
        Equality,
        UniversalQuantifier, ExistentialQuantifier,
        Not,
        And, Or,
        Implication, Converse,
    };
    MathLib::String value;
    MathLib::Array<Term> children;
    Type type;

    Term(void) : value(), children(), type(Type::None) {}
    Term(Type type, const MathLib::Sequence<Term>& children) : value(), children(MathLib::CollectionToArray<Term>(children)), type(type) {}
    Term(const MathLib::Sequence<char>& value) : value(MathLib::CollectionToString(value)), children(), type(Type::Variable) {}
    Term(bool value) : value(value ? "True" : "False"), children(), type(Type::LogicValue) {}
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        return MathLib::CollectionToString(padding) + ToStringInternal(true);
    }

    protected:
    [[nodiscard]] virtual bool Equals(const MathLib::Comparable& other_) const override {
        const Term& other = (const Term&)other_;
        return type == other.type && value == other.value && children == other.children;
    }
    private:
    [[nodiscard]] MathLib::String ToStringInternal(bool root) const {
        MathLib::String ret;
        switch (type) {
            case Type::None: return "None";
            case Type::LogicValue:
            case Type::Variable: return value;
            case Type::Equality:  {
                ret = children.At(0).ToStringInternal(false) + " = " + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::UniversalQuantifier: {
                ret = "for all "_M + children.At(0).ToStringInternal(false) + ' ' + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::ExistentialQuantifier: {
                ret = "for some "_M + children.At(0).ToStringInternal(false) + ' ' + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::Not: {
                ret = '!'_M + children.At(0).ToStringInternal(false);
                break;
            }
            case Type::And: {
                ret = children.At(0).ToStringInternal(false) + " & " + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::Or: {
                ret = children.At(0).ToStringInternal(false) + " | " + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::Implication: {
                ret = children.At(0).ToStringInternal(false) + " => " + children.At(1).ToStringInternal(false);
                break;
            }
            case Type::Converse: {
                ret = children.At(0).ToStringInternal(false) + " <= " + children.At(1).ToStringInternal(false);
                break;
            }
            default: return "Unknown";
        }
        return root ? ret : '('_M + ret + ')';
    }
};
struct Prover {
    Prover(void) : theorems() {}
    Prover(const MathLib::Sequence<Term>& axioms) : theorems(MathLib::CollectionToArray<Term>(axioms)) {}
    [[nodiscard]] Term Simplify(const Term& term) const {
        for (const Term& theorem : theorems)
            if (theorem == term) return Term(true);
        Term ret = term;
        for (Term& t : ret.children) t = Simplify(t);
        switch (ret.type) {
            case Term::Type::Equality: {
                if (ret.children.At(0) == ret.children.At(1)) return Term(true);
                break;
            }
            case Term::Type::UniversalQuantifier:
            case Term::Type::ExistentialQuantifier: {
                if (ret.children.At(1).type == Term::Type::LogicValue || ret.children.At(1).type == Term::Type::Variable) return ret.children.At(1);
                break;
            }
            case Term::Type::Not: {
                if (ret.children.At(0).type == Term::Type::LogicValue) return Term(ret.children.At(0).value != "True");
                break;
            }
            case Term::Type::And: {
                if (ret.children.At(0).type != Term::Type::LogicValue && !ret.children.Swap(0, 1)) return Term();
                if (ret.children.At(0).type == Term::Type::LogicValue) return ret.children.At(ret.children.At(0).value == "True");
                break;
            }
            case Term::Type::Or: {
                if (ret.children.At(0).type != Term::Type::LogicValue && !ret.children.Swap(0, 1)) return Term();
                if (ret.children.At(0).type == Term::Type::LogicValue) return ret.children.At(ret.children.At(0).value != "True");
                break;
            }
            case Term::Type::Implication: {
                if (ret.children.At(0).type == Term::Type::LogicValue)
                    return ret.children.At(0).value == "True" ? ret.children.At(1) : Term(true);
                if (ret.children.At(1).type == Term::Type::LogicValue)
                    return ret.children.At(1).value == "True" ? Term(true) : Term(Term::Type::Not, MathLib::MakeArray<Term>(ret.children.At(0)));
                break;
            }
            case Term::Type::Converse: {
                if (ret.children.At(0).type == Term::Type::LogicValue)
                    return ret.children.At(0).value == "True" ? Term(true) : Term(Term::Type::Not, MathLib::MakeArray<Term>(ret.children.At(1)));
                if (ret.children.At(1).type == Term::Type::LogicValue)
                    return ret.children.At(1).value == "True" ? ret.children.At(0) : Term(true);
                break;
            }
            default: break;
        }
        return ret;
    }

    private:
    MathLib::Array<Term> theorems;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const Term term = Term(Term::Type::UniversalQuantifier, MathLib::MakeArray<Term>(
            Term('x'_M),
            Term(Term::Type::Equality, MathLib::MakeArray<Term>(
                Term('x'_M),
                Term('x'_M)
            ))
        ));
        Prover prover;
        std::cout << term << std::endl;
        std::cout << prover.Simplify(term) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}