#include "Context.hpp"

Context::Context(void) : theorems(), parent(nullptr) {}
Context::Context(const MathLib::Array<Term>& theorems) : theorems(theorems), parent(nullptr) {}
Context::Context(const MathLib::Array<Term>& theorems, const Context* const parent) : theorems(theorems), parent(parent) {}
MathLib::String Context::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const MathLib::String padd2 = padd + '\t';
    return "{\n"_M + ToStringInternal(padd2) + padd + '}';
}
bool Context::ContainsTheorem(const Term& term) const {
    if (parent && parent->ContainsTheorem(term)) return true;
    for (const Term& theorem : theorems)
        if (theorem == term) return true;
    return false;
}
bool Context::CheckTypeEqualityWithReplacements(const Term& type1, const Term& type2, const Term& rep1, const Term& rep2, const Context& ignoredTheorems, bool check) const {
    Term tmp = type1.Replace(rep1, rep2);
    if ((check || type1 != tmp) && AreTypesEqualInternal(tmp, type2, ignoredTheorems)) return true;
    tmp = type1.Replace(rep2, rep1);
    if ((check || type1 != tmp) && AreTypesEqualInternal(tmp, type2, ignoredTheorems)) return true;
    tmp = type2.Replace(rep1, rep2);
    if ((check || type2 != tmp) && AreTypesEqualInternal(tmp, type1, ignoredTheorems)) return true;
    tmp = type2.Replace(rep2, rep1);
    if ((check || type2 != tmp) && AreTypesEqualInternal(tmp, type1, ignoredTheorems)) return true;
    return false;
}
bool Context::AreTypesEqualInternal(const Term& type1, const Term& type2, const Context& ignoredTheorems) const {
    if (type1 == type2) return true;
    if (type1.type == Term::Type::TypeAbstraction && type2.type == Term::Type::TypeAbstraction) {
        const Term typeDeclaration1 = type1.children.At(0);
        const Term typeDeclaration2 = type2.children.At(0);
        if (AreTypesEqualInternal(typeDeclaration1.children.At(1), typeDeclaration2.children.At(1), ignoredTheorems)) {
            const Term rest1 = type1.children.At(1);
            const Term rest2 = type2.children.At(1);
            const Term ptype1 = typeDeclaration1.children.At(0);
            const Term ptype2 = typeDeclaration2.children.At(0);
            if (CheckTypeEqualityWithReplacements(rest1, rest2, ptype1, ptype2, ignoredTheorems, true)) return true;
        }
    }
    if (type1.type == Term::Type::Abstraction && type2.type == Term::Type::Abstraction) {
        const Term a1 = type1.children.At(0);
        const Term a2 = type2.children.At(0);
        const Term b1 = type1.children.At(1);
        const Term b2 = type2.children.At(1);
        if (AreTypesEqualInternal(a1, a2, ignoredTheorems) && AreTypesEqualInternal(b1, b2, ignoredTheorems)) return true;
    }
    if (AreComputationallyEqual(RunSingle(type1, true), RunSingle(type2, true))) return true;
    for (const Term& theorem : theorems) {
        if (ignoredTheorems.ContainsTheorem(theorem)) continue;
        switch (theorem.type) {
            case Term::Type::Assignment: {
                const Term def1 = theorem.children.At(0).children.At(0);
                const Term def2 = theorem.children.At(1);
                const Context newIgnoredTheorems = Context(MathLib::MakeArray<Term>(theorem), &ignoredTheorems);
                if (CheckTypeEqualityWithReplacements(type1, type2, def1, def2, newIgnoredTheorems, false)) return true;
                break;
            }
            default: break;
        }
    }
    if (parent && parent->AreTypesEqualInternal(type1, type2, ignoredTheorems)) return true;
    return false;
}
bool Context::AreTypesEqual(const Term& type1, const Term& type2) const {
    return AreTypesEqualInternal(type1, type2, Context());
}
Term Context::GetTrivialTypeOf(const Term& term) const {
    if (parent) {
        const Term tmp = parent->GetTrivialTypeOf(term);
        if (tmp.type != Term::Type::None) return tmp;
    }
    for (const Term& theorem : theorems) {
        // TODO: Infer type from (type) abstraction and (type) application
        switch (theorem.type) {
            case Term::Type::TypeDeclaration: {
                if (theorem.children.At(0) == term) return theorem.children.At(1);
                break;
            }
            case Term::Type::Assignment: {
                const Term typeDeclaration = theorem.children.At(0);
                if (theorem.children.At(1) == term || typeDeclaration.children.At(0) == term) return typeDeclaration.children.At(1);
                break;
            }
            default: break;
        }
    }
    return Term();
}
Term Context::GetTypeOf(const Term& term) const {
    const Term tmp = GetTrivialTypeOf(term);
    if (tmp.type != Term::Type::None) return tmp;
    switch (term.type) {
        case Term::Type::TypeAbstraction: return Term(Term::Type::TypeAbstraction, term.children.At(0), AppendTypeDeclaration(term.children.At(0)).GetTypeOf(term.children.At(1)));
        case Term::Type::TypeApplication: {
            const Term a = GetTypeOf(term.children.At(0));
            const Term a0TypeDeclaration = a.children.At(0);
            if (a0TypeDeclaration.type != Term::Type::TypeDeclaration) return Term();
            const Term a0 = a0TypeDeclaration.children.At(0);
            const Term typeofA0 = a0TypeDeclaration.children.At(1);
            const Term a1 = a.children.At(1);
            const Term b = term.children.At(1);
            const Term typeofB = AppendTypeDeclaration(a0TypeDeclaration).GetTypeOf(b);
            if (!AreTypesEqual(typeofA0, typeofB)) return Term();
            return a1.Replace(a0, b);
        }
        case Term::Type::Abstraction: {
            if (term.children.At(0).type != Term::Type::TypeDeclaration) return Term();
            return Term(Term::Type::Abstraction, term.children.At(0).children.At(1), AppendTypeDeclaration(term.children.At(0)).GetTypeOf(term.children.At(1)));
        }
        case Term::Type::Application: {
            const Term a = GetTypeOf(term.children.At(0));
            if (a.type != Term::Type::Abstraction) return Term();
            const Term b = GetTypeOf(term.children.At(1));
            if (!AreTypesEqual(a.children.At(0), b)) return Term();
            return a.children.At(1);
        }
        default: return Term();
    }
}
bool Context::CanBeTypeOf(const Term& term, const Term& type) const {
    if (term.type == Term::Type::TypeAbstraction && type.type == Term::Type::TypeAbstraction) {
        const Term typeDeclaration1 = term.children.At(0);
        const Term typeDeclaration2 = type.children.At(0);
        if (AreTypesEqual(typeDeclaration1.children.At(1), typeDeclaration2.children.At(1))) {
            const Term rest1 = term.children.At(1);
            const Term rest2 = type.children.At(1);
            const Term ptype1 = typeDeclaration1.children.At(0);
            const Term ptype2 = typeDeclaration2.children.At(0);
            if (AppendTypeDeclaration(typeDeclaration1).CanBeTypeOf(rest1, rest2.Replace(ptype2, ptype1))) return true;
        }
    }
    return AreTypesEqual(GetTypeOf(term), type);
}
Term Context::RunSingle(const Term& term, bool runVariables) const {
    switch (term.type) {
        case Term::Type::Variable: {
            if (!runVariables) return term;
            if (parent) {
                const Term tmp = parent->RunSingle(term, runVariables);
                if (tmp != term) return tmp;
            }
            for (const Term& theorem : theorems) {
                switch (theorem.type) {
                    case Term::Type::Assignment: {
                        const Term def1 = theorem.children.At(0).children.At(0);
                        const Term def2 = theorem.children.At(1);
                        if (term == def1) return def2;
                        if (term == def2) return def1;
                    }
                    default: break;
                }
            }
            return term;
        }
        case Term::Type::Comma: {
            const Term a = term.children.At(0);
            const Term b = term.children.At(1);
            const Term tmp = RunSingle(a, runVariables);
            if (tmp != a) return Term(Term::Type::Comma, tmp, b);
            return Term(Term::Type::Comma, a, RunSingle(b, runVariables));
        }
        case Term::Type::Assignment: return Term(term.type, term.children.At(0), RunSingle(term.children.At(1), runVariables));
        case Term::Type::TypeAbstraction:
        case Term::Type::Abstraction: {
            const Term typeDeclaration = term.children.At(0);
            return Term(term.type, typeDeclaration, AppendTypeDeclaration(typeDeclaration).RunSingle(term.children.At(1), runVariables));
        }
        case Term::Type::TypeApplication:
        case Term::Type::Application: {
            const Term::Type functionTermType = (Term::Type)((size_t)term.type - 1);
            const Term function = term.children.At(0);
            const Term argument = term.children.At(1);
            if (function.type != functionTermType) {
                const Term tmp = RunSingle(function, runVariables);
                if (tmp != function) return Term(term.type, tmp, argument);
                return Term(term.type, function, RunSingle(argument, runVariables));
            }
            const Term argumentTypeDeclaration = function.children.At(0);
            const Term argumentName = argumentTypeDeclaration.children.At(0);
            const Term argumentType = argumentTypeDeclaration.children.At(1);
            const Term functionBody = function.children.At(1);
            if (!CanBeTypeOf(argument, argumentType)) return term;
            return functionBody.Replace(argumentName, argument);
        }
        default: return term;
    }
}
Term Context::Run(const Term& term, size_t depth, bool runVariables) const {
    Term current = term;
    for (size_t i = 0; i < depth; i++) {
        const Term tmp = current;
        current = RunSingle(current, runVariables);
        if (tmp == current) break;
    }
    return current;
}
bool Context::AreComputationallyEqual(const Term& term1, const Term& term2) const {
    Term current1 = term1;
    Term current2 = term2;
    while (true) {
        if (current1 == current2) return true;
        Term tmp = current1;
        current1 = RunSingle(current1, false);
        const bool eq = tmp == current1;
        tmp = current2;
        current2 = RunSingle(current2, false);
        if (eq && tmp == current2) return current1 == current2;
    }
}
bool Context::TypeCheck(const Term& term) {
    switch (term.type) {
        case Term::Type::Comma: return TypeCheck(term.children.At(0)) && TypeCheck(term.children.At(1));
        case Term::Type::Assignment: return AppendTypeDeclaration(term.children.At(0)).CanBeTypeOf(term.children.At(1), term.children.At(0).children.At(1)) && theorems.Add(term);
        case Term::Type::TypeDeclaration: return theorems.Add(term);
        default: return false;
    }
}
MathLib::String Context::ToStringInternal(const MathLib::String& padding) const {
    MathLib::String ret;
    for (const Term& theorem : theorems) ret += theorem.ToString(padding) + '\n';
    if (parent) ret += parent->ToStringInternal(padding);
    return ret;
}
Context Context::AppendTypeDeclaration(const Term& term) const {
    return (term.type != Term::Type::TypeDeclaration || term.children.At(0).name == "_") ? * this : Context(MathLib::MakeArray<Term>(
        term
    ), this);
}