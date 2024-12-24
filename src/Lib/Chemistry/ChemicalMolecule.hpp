#ifndef MathLib_Chemistry_Molecule_H
#define MathLib_Chemistry_Molecule_H
#include "ChemicalElement.hpp"

namespace MathLib {
    struct ChemicalMolecule : Printable {
        ChemicalMolecule(void);
        ChemicalMolecule(const Array<ChemicalElement>& elements, size_t count);
        Array<ChemicalElement> GetElements(void) const;
        bool IsCompound(void) const;
        bool IsWater(void) const;
        bool IsMetal(void) const;
        bool IsNonMetal(void) const;
        bool IsAcid(void) const;
        bool IsHydroxide(void) const;
        bool IsOxide(void) const;
        virtual String ToString(const String& padding = "") const override;

        size_t count;
        private:
        Array<ChemicalElement> elements;
    };
}

#endif