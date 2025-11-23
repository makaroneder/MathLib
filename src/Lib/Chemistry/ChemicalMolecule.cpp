#include "ChemicalMolecule.hpp"

namespace MathLib {
    ChemicalMolecule::ChemicalMolecule(void) : elements(Array<ChemicalElement>()), count(0) {}
    ChemicalMolecule::ChemicalMolecule(const Sequence<ChemicalElement>& elements, size_t count) : elements(CollectionToArray<ChemicalElement>(elements)), count(count) {}
    Array<ChemicalElement> ChemicalMolecule::GetElements(void) const {
        Array<ChemicalElement> ret = elements;
        for (ChemicalElement& element : ret) element *= count;
        return ret;
    }
    bool ChemicalMolecule::IsCompound(void) const {
        return elements.GetSize() > 1;
    }
    bool ChemicalMolecule::IsWater(void) const {
        return IsCompound() && elements.At(0).GetSymbol() == "H" && elements.At(1).GetSymbol() == "O" && elements.At(0).GetCount() == 2 && elements.At(1).GetCount() == 1;
    }
    bool ChemicalMolecule::IsMetal(void) const {
        return !IsCompound() && elements.At(0).IsMetal();
    }
    bool ChemicalMolecule::IsNonMetal(void) const {
        return !IsCompound() && !elements.At(0).IsMetal();
    }
    bool ChemicalMolecule::IsAcid(void) const {
        return IsCompound() && !IsWater() && elements.At(0).GetSymbol() == "H";
    }
    bool ChemicalMolecule::IsHydroxide(void) const {
        return IsCompound() && !IsWater() && elements.At(elements.GetSize() - 2).GetSymbol() == "O" && elements.At(elements.GetSize() - 1).GetSymbol() == "H" && elements.At(elements.GetSize() - 2).GetCount() == elements.At(elements.GetSize() - 1).GetCount();
    }
    bool ChemicalMolecule::IsOxide(void) const {
        return IsCompound() && !IsWater() && elements.At(elements.GetSize() - 1).GetSymbol() == "O";
    }
    String ChemicalMolecule::ToString(const Sequence<char>& padding) const {
        String str = "";
        for (const ChemicalElement& element : elements) str += element.ToString();
        return CollectionToString(padding) + (count == 1 ? "" : Formatter<size_t>::ToString(count)) + str;
    }
    bool ChemicalMolecule::operator==(const ChemicalMolecule& other) const {
        return count == other.count && elements == other.elements;
    }
    bool ChemicalMolecule::operator!=(const ChemicalMolecule& other) const {
        return !(*this == other);
    }
}