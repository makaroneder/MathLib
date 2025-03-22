#include "FormalSystem.hpp"

FormalSystem::FormalSystem(const MathLib::Array<Operation*>& operations, const MathLib::Array<Theory>& axioms) : operations(operations), theories(axioms) {}
FormalSystem::~FormalSystem(void) {
    for (Operation*& operation : operations) delete operation;
}
bool FormalSystem::Prove(const Theory& proof) {
    const size_t size = proof.data.GetSize();
    for (size_t i = 1; i < size; i++)
        if (!IsLogicalConsequence(proof.data.At(i - 1), proof.data.At(i))) return false;
    return proof.data.At(proof.data.GetSize() - 1).IsTrue() && theories.Add(proof);
}
MathLib::Expected<bool> FormalSystem::IsLogicalConsequence(Formula& prev, Formula& curr, const Formula& formula) const {
    for (const Operation* const& operation : operations) {
        if (curr.type == Formula::Type::Operation && curr.value == operation->name) {
            const MathLib::Array<Formula> tmp = (*operation->func)(curr.children);
            if (tmp.IsEmpty()) return MathLib::Expected<bool>();
            const Formula save = curr;
            for (const Formula& x : tmp) {
                curr = x;
                if (formula == prev) return true;
            }
            curr = save;
        }
        for (Formula& child : curr.children) {
            const MathLib::Expected<bool> tmp = IsLogicalConsequence(prev, child, formula);
            if (!tmp.HasValue()) return MathLib::Expected<bool>();
            if (tmp.Get()) return true;
        }
    }
    return false;
}
bool FormalSystem::IsLogicalConsequence(Formula prev, const Formula& formula) const {
    if (formula.type == Formula::Type::Operation && formula.value == "ApplyTheorem") return true;
    if (prev.type == Formula::Type::Operation && prev.value == "ApplyTheorem") {
        const Formula info = prev.children.At(0);
        Theory theory;
        bool found = false;
        for (const Theory& t : theories) {
            if (t.name == info.value) {
                theory = t;
                found = true;
                break;
            }
        }
        if (!found) return false;
        Theory sub;
        for (const Formula& arg : info.children)
            if (!sub.variables.Add(arg.value) || !sub.data.Add(arg.children.At(0))) return false;
        theory = theory.Substitute(sub);
        const Formula next = prev.children.At(1);
        found = false;
        for (const Formula& form : theory.data)
            if (form == next) found = true;
        return found && IsLogicalConsequence(next, formula);
    }
    return IsLogicalConsequence(prev, prev, formula).GetOr(false);
}