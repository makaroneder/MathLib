#ifndef MathLib_Chemistry_Molecule_H
#define MathLib_Chemistry_Molecule_H
#include "ChemicalElement.hpp"

namespace MathLib {
    struct ChemicalMolecule : Printable {
        ChemicalMolecule(void);
        ChemicalMolecule(const Array<ChemicalElement>& elements, size_t count);
        [[nodiscard]] Array<ChemicalElement> GetElements(void) const;
        [[nodiscard]] bool IsCompound(void) const;
        [[nodiscard]] bool IsWater(void) const;
        [[nodiscard]] bool IsMetal(void) const;
        [[nodiscard]] bool IsNonMetal(void) const;
        [[nodiscard]] bool IsAcid(void) const;
        [[nodiscard]] bool IsHydroxide(void) const;
        [[nodiscard]] bool IsOxide(void) const;
        [[nodiscard]] virtual String ToString(const String& padding = "") const override;

        size_t count;
        private:
        Array<ChemicalElement> elements;
    };
}

#endif