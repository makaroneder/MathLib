#include "ChemicalMolecule.hpp"

ChemicalMolecule::ChemicalMolecule(void) {}
ChemicalMolecule::ChemicalMolecule(const Array<ChemicalElement>& elements, size_t count) : count(count), elements(elements) {}
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
String ChemicalMolecule::ToString(const String& padding) const {
    String str = "";
    for (const ChemicalElement& element : elements) str += element.ToString();
    return padding + (count == 1 ? "" : ::ToString(count)) + str;
}