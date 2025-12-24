#include "Sequent.hpp"
#include <String.hpp>

size_t uniqueName;
[[nodiscard]] MathLib::String MakeUniqueName(void) {
    return MathLib::ToString(uniqueName++, 10);
}
Sequent::Sequent(void) : left(), right() {}
Sequent::Sequent(const MathLib::Sequence<Formula>& left, const MathLib::Sequence<Formula>& right) : left(MathLib::CollectionToArray<Formula>(left)), right(MathLib::CollectionToArray<Formula>(right)) {}
MathLib::Array<Sequent> Sequent::Simplify(void) const {
    MathLib::Array<Sequent> ret = MathLib::Array<Sequent>(1);
    bool modified = false;
    for (const Formula& formula : right) {
        switch (modified ? Formula::Type::Variable : formula.type) {
            case Formula::Type::Conjunction: {
                if (!ret.Add(ret.At(0))) return MathLib::Array<Sequent>();
                for (uint8_t i = 0; i < 2; i++)
                    if (!ret.At(i).right.Add(formula.children.At(i))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Disjunction: {
                for (uint8_t i = 0; i < 2; i++)
                    if (!ret.At(0).right.Add(formula.children.At(i))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Implication: {
                if (!ret.At(0).left.Add(formula.children.At(0)) || !ret.At(0).right.Add(formula.children.At(1))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Negation: {
                if (!ret.At(0).left.Add(formula.children.At(0))) return MathLib::Array<Sequent>();
                break;
            }
            default: for (Sequent& sequent : ret)
                if (!sequent.right.Add(formula)) return MathLib::Array<Sequent>();
        }
        modified |= formula.type != Formula::Type::Variable;
    }
    for (const Formula& formula : left) {
        switch (modified ? Formula::Type::Variable : formula.type) {
            case Formula::Type::Conjunction: {
                for (uint8_t i = 0; i < 2; i++)
                    if (!ret.At(0).left.Add(formula.children.At(i))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Disjunction: {
                if (!ret.Add(ret.At(0))) return MathLib::Array<Sequent>();
                for (uint8_t i = 0; i < 2; i++)
                    if (!ret.At(i).left.Add(formula.children.At(i))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Implication: {
                if (!ret.Add(ret.At(0))|| !ret.At(0).right.Add(formula.children.At(0)) || !ret.At(1).left.Add(formula.children.At(1))) return MathLib::Array<Sequent>();
                break;
            }
            case Formula::Type::Negation: {
                if (!ret.At(0).right.Add(formula.children.At(0))) return MathLib::Array<Sequent>();
                break;
            }
            default: for (Sequent& sequent : ret)
                if (!sequent.left.Add(formula)) return MathLib::Array<Sequent>();
        }
        modified |= formula.type != Formula::Type::Variable;
    }
    return ret;
}
bool Sequent::ProveInternal(MathLib::Tree<Sequent>& tree) const {
    tree = MathLib::Tree<Sequent>(MakeUniqueName(), *this);
    for (const Formula& formula : right)
        if (left.Contains(formula)) return true;
    const MathLib::Array<Sequent> next = Simplify();
    for (const Sequent& seq : next) {
        MathLib::Tree<Sequent> add;
        if (!seq.ProveInternal(add) || !tree.Add(add)) return false;
    }
    return true;
}
MathLib::Tree<Sequent> Sequent::Prove(void) const {
    uniqueName = 0;
    MathLib::Tree<Sequent> ret;
    return ProveInternal(ret) ? ret : MathLib::Tree<Sequent>();
}
MathLib::String Sequent::ToString(const MathLib::Sequence<char>& padding) const {
    const size_t leftSize = left.GetSize();
    const size_t rightSize = right.GetSize();
    MathLib::String ret = MathLib::CollectionToString(padding);
    for (size_t i = 0; i < leftSize; i++) {
        if (i) ret += ", ";
        ret += left.At(i).ToString();
    }
    if (leftSize) ret += ' ';
    ret += "|-";
    if (rightSize) ret += ' ';
    for (size_t i = 0; i < rightSize; i++) {
        if (i) ret += ", ";
        ret += right.At(i).ToString();
    }
    return ret;
}
bool Sequent::Equals(const Sequent& other) const {
    return left == other.left && right == other.right;
}