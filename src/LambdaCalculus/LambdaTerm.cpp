#include "LambdaTerm.hpp"

LambdaTerm::LambdaTerm(void) : value(), children(), type(Type::None) {}
LambdaTerm::LambdaTerm(const MathLib::Sequence<char>& value, bool string) : value(MathLib::CollectionToString(value)), children(), type(string ? Type::String : Type::Variable) {}
LambdaTerm::LambdaTerm(const LambdaTerm& func, const LambdaTerm& arg) : value(), children(MathLib::MakeArray<LambdaTerm>(func, arg)), type(Type::Application) {}
LambdaTerm::LambdaTerm(const LambdaTerm& func, const MathLib::Sequence<char>& arg) : value(MathLib::CollectionToString(arg)), children(MathLib::MakeArray<LambdaTerm>(func)), type(Type::Abstraction) {}
LambdaTerm::LambdaTerm(const MathLib::Sequence<char>& name, const LambdaTerm& value) : value(MathLib::CollectionToString(name)), children(MathLib::MakeArray<LambdaTerm>(value)), type(Type::Binding) {}
LambdaTerm::LambdaTerm(const MathLib::Sequence<LambdaTerm>& cases) : value(), children(MathLib::CollectionToArray<LambdaTerm>(cases)), type(Type::Pattern) {}
bool LambdaTerm::Match(const LambdaTerm& term, MathLib::WritableSequence<LambdaTerm>& bindings) const {
    const size_t size = bindings.GetSize();
    switch (type) {
        case Type::Variable: {
            if (value == "_") return true;
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (value == binding.value) return binding.children.At(0).Match(term, bindings);
            }
            return bindings.Add(LambdaTerm(value, term));
        }
        case Type::Abstraction: {
            if (term.type != Type::Abstraction) return false;
            size_t index = SIZE_MAX;
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (value == binding.value) {
                    index = i;
                    break;
                }
            }
            if (index != SIZE_MAX) bindings.At(index).children.At(0) = LambdaTerm(term.value, false);
            else if (!bindings.Add(LambdaTerm(value, LambdaTerm(term.value, false)))) return false;
            return children.At(0).Match(term, bindings);
        }
        case Type::Application: {
            if (term.type != Type::Application) return false;
            for (uint8_t i = 0; i < 2; i++)
                if (!children.At(i).Match(term.children.At(i), bindings)) return false;
            return true;
        }
        case Type::Binding: {
            size_t index = SIZE_MAX;
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (value == binding.value) {
                    index = i;
                    break;
                }
            }
            if (index != SIZE_MAX) bindings.At(index).children.At(0) = children.At(0);
            else if (!bindings.Add(LambdaTerm(value, children.At(0)))) return false;
            return children.At(0).Match(term, bindings);
        }
        default: return *this == term;
    }
}
LambdaTerm LambdaTerm::Run(const MathLib::Sequence<LambdaTerm>& bindings) const {
    const size_t size = bindings.GetSize();
    switch (type) {
        case Type::Variable: {
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (value == binding.value && binding.children.At(0) != *this) return binding.children.At(0).Run(bindings);
            }
            return *this;
        }
        case Type::Binding: {
            MathLib::Array<LambdaTerm> newBindings;
            bool add = true;
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (binding.value == value) {
                    if (!newBindings.Add(LambdaTerm(binding.value, children.At(0)))) return LambdaTerm();
                    add = false;
                }
                else if (!newBindings.Add(binding)) return LambdaTerm();
            }
            return !add || newBindings.Add(LambdaTerm(value, children.At(0))) ? children.At(0).Run(newBindings) : LambdaTerm();
        }
        case Type::Abstraction: {
            MathLib::Array<LambdaTerm> newBindings;
            for (size_t i = 0; i < size; i++) {
                const LambdaTerm binding = bindings.At(i);
                if (binding.value != value && !newBindings.Add(binding)) return LambdaTerm();
            }
            return LambdaTerm(children.At(0).Run(newBindings), value);
        }
        case Type::Application: {
            const LambdaTerm body = children.At(0).Run(bindings);
            const LambdaTerm arg = children.At(1).Run(bindings);
            if (body.type == Type::Abstraction) {
                MathLib::Array<LambdaTerm> newBindings;
                bool add = true;
                for (size_t i = 0; i < size; i++) {
                    const LambdaTerm binding = bindings.At(i);
                    if (binding.value == body.value) {
                        if (!newBindings.Add(LambdaTerm(binding.value, arg))) return LambdaTerm();
                        add = false;
                    }
                    else if (!newBindings.Add(binding)) return LambdaTerm();
                }
                return !add || newBindings.Add(LambdaTerm(body.value, arg)) ? body.children.At(0).Run(newBindings) : LambdaTerm();
            }
            else if (body.type == Type::Pattern) {
                const size_t size = body.children.GetSize();
                for (size_t i = 0; i < size; i += 2) {
                    MathLib::Array<LambdaTerm> vars = MathLib::CollectionToArray<LambdaTerm>(bindings);
                    if (body.children.At(i).Match(arg, vars)) {
                        MathLib::Array<LambdaTerm> tmp;
                        for (size_t i = 0; i < vars.GetSize(); i++)
                            if (!bindings.Contains(vars.At(i)) && !tmp.Add(vars.At(i))) return LambdaTerm();
                        return body.children.At(i + 1).Run(tmp);
                    }
                }
            }
            return LambdaTerm(body, arg);
        }
        case Type::Pattern: {
            MathLib::Array<LambdaTerm> ret = children;
            const size_t childernSize = ret.GetSize();
            for (size_t i = 0; i < childernSize; i += 2) {
                const MathLib::Array<MathLib::String> variables = ret.At(i).GetVariables();
                MathLib::Array<LambdaTerm> newBindings;
                for (size_t j = 0; j < size; j++) {
                    const LambdaTerm binding = bindings.At(j);
                    if (!variables.Contains(binding.value) && binding.children.At(0) != *this && !newBindings.Add(binding)) return LambdaTerm();
                }
                ret.At(i + 1) = ret.At(i + 1).Run(newBindings);
            }
            return LambdaTerm(ret);
        }
        default: return *this;
    }
}
MathLib::Array<MathLib::String> LambdaTerm::GetVariables(void) const {
    switch (type) {
        case Type::Variable: return MathLib::MakeArray<MathLib::String>(value);
        case Type::Abstraction: {
            MathLib::Array<MathLib::String> ret = children.At(0).GetVariables();
            const MathLib::Array<MathLib::String> tmp = children.At(1).GetVariables();
            for (const MathLib::String& str : tmp)
                if (!ret.Contains(str) && !ret.Add(str)) return MathLib::Array<MathLib::String>();
            return ret;
        }
        default: return MathLib::Array<MathLib::String>();
    }
}
MathLib::String LambdaTerm::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + ToStringInternal(true);
}
bool LambdaTerm::Equals(const MathLib::Comparable& other_) const {
    const LambdaTerm& other = (const LambdaTerm&)other_;
    return type == other.type && value == other.value && children == other.children;
}
MathLib::String LambdaTerm::ToStringInternal(bool root) const {
    switch (type) {
        case Type::Variable: return value;
        case Type::String: return '"'_M + value + '"';
        case Type::Binding: return value + " = " + children.At(0).ToStringInternal(true);
        case Type::Application: {
            const MathLib::String ret = children.At(0).ToStringInternal(children.At(0).type == Type::Application) + " . " + children.At(1).ToStringInternal(false);
            return root ? ret : '('_M + ret + ')';
        }
        case Type::Abstraction: {
            const MathLib::String ret = value + " -> " + children.At(0).ToStringInternal(children.At(0).type == Type::Abstraction);
            return root ? ret : '('_M + ret + ')';
        }
        case Type::Pattern: {
            MathLib::String ret = "pattern ("_M;
            const size_t size = children.GetSize();
            for (size_t i = 0; i < size; i += 2) {
                if (i) ret += ", ";
                ret += children.At(i).ToStringInternal(false) + " -> " + children.At(i + 1).ToStringInternal(false);
            }
            ret += ')';
            return root ? ret : '('_M + ret + ')';
        }
        default: return "None";
    }
}