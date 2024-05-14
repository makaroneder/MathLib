#ifndef Chemistry_Molecule_H
#define Chemistry_Molecule_H
#include "Elements.hpp"
#include <vector>

template <typename T>
struct ChemicalMolecule : Printable {
    ChemicalMolecule(std::vector<ChemicalElement<T>> elements_, size_t count_) : count(count_), elements(elements_) {}
    constexpr std::vector<ChemicalElement<T>> GetElements(void) const {
        std::vector<ChemicalElement<T>> ret = elements;
        for (ChemicalElement<T>& element : ret) element *= count;
        return ret;
    }
    constexpr bool IsCompound(void) const {
        return elements.size() > 1;
    }
    constexpr bool IsWater(void) const {
        return IsCompound() && elements.at(0).GetSymbol() == "H" && elements.at(1).GetSymbol() == "O" && elements.at(0).GetCount() == 2 && elements.at(1).GetCount() == 1;
    }
    constexpr bool IsMetal(void) const {
        return !IsCompound() && elements.at(0).IsMetal();
    }
    constexpr bool IsNonMetal(void) const {
        return !IsCompound() && !elements.at(0).IsMetal();
    }
    constexpr bool IsAcid(void) const {
        return IsCompound() && !IsWater() && elements.at(0).GetSymbol() == "H";
    }
    constexpr bool IsHydroxide(void) const {
        return IsCompound() && !IsWater() && elements.at(elements.size() - 2).GetSymbol() == "O" && elements.at(elements.size() - 1).GetSymbol() == "H" && elements.at(elements.size() - 2).GetCount() == elements.at(elements.size() - 1).GetCount();
    }
    constexpr bool IsOxide(void) const {
        return IsCompound() && !IsWater() && elements.at(elements.size() - 1).GetSymbol() == "O";
    }
    virtual std::string ToString(std::string padding = "") const override {
        std::string str = "";
        for (const ChemicalElement<T>& element : elements) str += element.ToString();
        return padding + (count == 1 ? "" : std::to_string(count)) + str;
    }

    size_t count;
    private:
    std::vector<ChemicalElement<T>> elements;
};

#endif