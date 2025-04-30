#ifndef MathLib_Chemistry_Molecule_H
#define MathLib_Chemistry_Molecule_H
#include "ChemicalElement.hpp"

namespace MathLib {
    struct ChemicalMolecule : Printable {
        ChemicalMolecule(void);
        ChemicalMolecule(const Sequence<ChemicalElement>& elements, size_t count);
        [[nodiscard]] Array<ChemicalElement> GetElements(void) const;
        [[nodiscard]] bool IsCompound(void) const;
        [[nodiscard]] bool IsWater(void) const;
        [[nodiscard]] bool IsMetal(void) const;
        [[nodiscard]] bool IsNonMetal(void) const;
        [[nodiscard]] bool IsAcid(void) const;
        [[nodiscard]] bool IsHydroxide(void) const;
        [[nodiscard]] bool IsOxide(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const ChemicalMolecule& other) const;
        [[nodiscard]] bool operator!=(const ChemicalMolecule& other) const;

        private:
        Array<ChemicalElement> elements;
        public:
        size_t count;
    };
}

#endif