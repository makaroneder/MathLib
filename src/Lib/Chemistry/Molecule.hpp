#ifndef Chemistry_Molecule_H
#define Chemistry_Molecule_H
#include "Elements.hpp"
#include <vector>

template <typename T>
struct ChemicalMolecule : Printable {
    ChemicalMolecule(void) {}
    ChemicalMolecule(Array<ChemicalElement<T>> elements_, size_t count_) : count(count_), elements(elements_) {}
    constexpr Array<ChemicalElement<T>> GetElements(void) const {
        Array<ChemicalElement<T>> ret = elements;
        for (size_t i = 0; i < ret.GetSize(); i++) elements.At(i) *= count;
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
    virtual String ToString(String padding = "") const override {
        String str = "";
        for (size_t i = 0; i < elements.GetSize(); i++) str += elements.At(i).ToString();
        return padding + (count == 1 ? "" : ::ToString(count)) + str;
    }

    size_t count;
    private:
    Array<ChemicalElement<T>> elements;
};

#endif