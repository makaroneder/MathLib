#ifndef MathLib_Chemistry_Reaction_H
#define MathLib_Chemistry_Reaction_H
#include "ChemicalReactionElement.hpp"
#include "ChemicalMolecule.hpp"

namespace MathLib {
    struct ChemicalReaction : Printable  {
        ChemicalReaction(void);
        ChemicalReaction(const Sequence<ChemicalMolecule>& left, const Sequence<ChemicalMolecule>& right);
        [[nodiscard]] static Expected<ChemicalReaction> Create(const Sequence<ChemicalMolecule>& input);
        [[nodiscard]] Expected<ChemicalReaction> Balance(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        [[nodiscard]] Array<ChemicalReactionElement> GetReactionElements(bool left_) const;

        Array<ChemicalMolecule> left;
        Array<ChemicalMolecule> right;
    };
}

#endif