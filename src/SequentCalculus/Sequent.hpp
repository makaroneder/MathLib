#ifndef Sequent_H
#define Sequent_H
#include "Formula.hpp"
#include <Graph/Tree.hpp>

struct Sequent;
struct Sequent : MathLib::Comparable<Sequent>, MathLib::Printable {
    MathLib::Array<Formula> left;
    MathLib::Array<Formula> right;

    Sequent(void);
    Sequent(const MathLib::Sequence<Formula>& left, const MathLib::Sequence<Formula>& right);
    [[nodiscard]] MathLib::Array<Sequent> Simplify(void) const;
    [[nodiscard]] MathLib::Tree<Sequent> Prove(void) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const Sequent& other) const override;

    private:
    [[nodiscard]] bool ProveInternal(MathLib::Tree<Sequent>& tree) const;
};
namespace MathLib {
    MakeFormatter(Sequent, self, padding, { return self.ToString(padding); });
}

#endif