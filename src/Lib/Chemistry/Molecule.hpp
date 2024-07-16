#ifndef Chemistry_Molecule_H
#define Chemistry_Molecule_H
#include "Elements.hpp"

template <typename T>
struct ChemicalMolecule : Printable {
    ChemicalMolecule(void) {}
    ChemicalMolecule(const Array<ChemicalElement<T>>& elements_, const size_t& count_) : count(count_), elements(elements_) {}
    constexpr Array<ChemicalElement<T>> GetElements(void) const {
        Array<ChemicalElement<T>> ret = elements;
        for (ChemicalElement<T>& element : ret) element *= count;
        return ret;
    }
    constexpr bool IsCompound(void) const {
        return elements.GetSize() > 1;
    }
    constexpr bool IsWater(void) const {
        return IsCompound() && elements.At(0).GetSymbol() == "H" && elements.At(1).GetSymbol() == "O" && elements.At(0).GetCount() == 2 && elements.At(1).GetCount() == 1;
    }
    constexpr bool IsMetal(void) const {
        return !IsCompound() && elements.At(0).IsMetal();
    }
    constexpr bool IsNonMetal(void) const {
        return !IsCompound() && !elements.At(0).IsMetal();
    }
    constexpr bool IsAcid(void) const {
        return IsCompound() && !IsWater() && elements.At(0).GetSymbol() == "H";
    }
    constexpr bool IsHydroxide(void) const {
        return IsCompound() && !IsWater() && elements.At(elements.GetSize() - 2).GetSymbol() == "O" && elements.At(elements.GetSize() - 1).GetSymbol() == "H" && elements.At(elements.GetSize() - 2).GetCount() == elements.At(elements.GetSize() - 1).GetCount();
    }
    constexpr bool IsOxide(void) const {
        return IsCompound() && !IsWater() && elements.At(elements.GetSize() - 1).GetSymbol() == "O";
    }
    virtual String ToString(const String& padding = "") const override {
        String str = "";
        for (const ChemicalElement<T>& element : elements) str += element.ToString();
        return padding + (count == 1 ? "" : ::ToString(count)) + str;
    }

    size_t count;
    private:
    Array<ChemicalElement<T>> elements;
};

#endif