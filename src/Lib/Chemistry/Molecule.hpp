#ifndef Chemistry_Molecule_H
#define Chemistry_Molecule_H
#include "Elements.hpp"
#include <vector>

template <typename T>
struct ChemicalMolecule : Printable {
    ChemicalMolecule(std::vector<ChemicalElement<T>> elements_, size_t count_) {
        elements = elements_;
        count = count_;
    }
    constexpr std::vector<ChemicalElement<T>> GetElements(void) const {
        std::vector<ChemicalElement<T>> ret = elements;
        for (ChemicalElement<T>& element : ret) element *= count;
        return ret;
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