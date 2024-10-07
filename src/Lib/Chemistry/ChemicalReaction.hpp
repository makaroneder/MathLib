#ifndef Chemistry_Reaction_H
#define Chemistry_Reaction_H
#include "ChemicalReactionElement.hpp"
#include "ChemicalMolecule.hpp"

struct ChemicalReaction : Printable  {
    ChemicalReaction(void);
    ChemicalReaction(const Array<ChemicalMolecule>& left, const Array<ChemicalMolecule>& right);
    static Expected<ChemicalReaction> Create(const Array<ChemicalMolecule>& input);
    Expected<ChemicalReaction> Balance(void) const;
    virtual String ToString(const String& padding = "") const override;

    private:
    Array<ChemicalReactionElement> GetReactionElements(bool left_) const;

    Array<ChemicalMolecule> left;
    Array<ChemicalMolecule> right;
};

#endif