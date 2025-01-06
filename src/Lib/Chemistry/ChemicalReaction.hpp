#ifndef MathLib_Chemistry_Reaction_H
#define MathLib_Chemistry_Reaction_H
#include "ChemicalReactionElement.hpp"
#include "ChemicalMolecule.hpp"

namespace MathLib {
    struct ChemicalReaction : Printable  {
        ChemicalReaction(void);
        ChemicalReaction(const Array<ChemicalMolecule>& left, const Array<ChemicalMolecule>& right);
        [[nodiscard]] static Expected<ChemicalReaction> Create(const Array<ChemicalMolecule>& input);
        [[nodiscard]] Expected<ChemicalReaction> Balance(void) const;
        [[nodiscard]] virtual String ToString(const String& padding = "") const override;

        private:
        [[nodiscard]] Array<ChemicalReactionElement> GetReactionElements(bool left_) const;

        Array<ChemicalMolecule> left;
        Array<ChemicalMolecule> right;
    };
}

#endif