#ifndef Sequent_H
#define Sequent_H
#include "Formula.hpp"
#include <Graph/Tree.hpp>

struct Sequent : MathLib::Printable {
    MathLib::Array<Formula> left;
    MathLib::Array<Formula> right;

    Sequent(void);
    Sequent(const MathLib::Sequence<Formula>& left, const MathLib::Sequence<Formula>& right);
    [[nodiscard]] MathLib::Array<Sequent> Simplify(void) const;
    [[nodiscard]] MathLib::Tree<Sequent> Prove(void) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const Sequent& other) const;
    [[nodiscard]] bool operator!=(const Sequent& other) const;

    private:
    [[nodiscard]] bool ProveInternal(MathLib::Tree<Sequent>& tree) const;
};

#endif