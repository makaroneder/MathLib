#ifndef ChemicalElements_H
#define ChemicalElements_H
#include "SIUnits.hpp"
#include <vector>

template <typename T>
struct ChemicalElement {
    ChemicalElement(size_t atomicNumber, Dalton<T> atomicWeight, std::string sym, size_t count_) {
        number = atomicNumber;
        weight = atomicWeight;
        symbol = sym;
        count = count_;
    }
    constexpr size_t GetCount(void) const {
        return count;
    }
    constexpr size_t GetElectrons(void) const {
        return number * count;
    }
    constexpr size_t GetProtons(void) const {
        return number * count;
    }
    constexpr size_t GetNeutrons(void) const {
        return (std::roundl(weight.GetValue()) - number) * count;
    }
    constexpr std::string GetSymbol(void) const {
        return symbol;
    }
    constexpr std::string ToString(void) const {
        return symbol + (count == 1 ? "" : std::to_string(count));
    }
    constexpr ChemicalElement operator*=(size_t num) {
        count *= num;
        return *this;
    }

    private:
    size_t number;
    Dalton<T> weight;
    std::string symbol;
    size_t count;
};

#define CreateChemicalElement(name, symbol, number, weight)                                     \
template <typename T>                                                                           \
struct name : ChemicalElement<T> {                                                              \
    name<T>(size_t count_) : ChemicalElement<T>(number, Dalton<T>(weight), #symbol, count_) {}  \
}

CreateChemicalElement(Hydrogen, H, 1, 1.008);
CreateChemicalElement(Helium, He, 2, 4.0026);
CreateChemicalElement(Lithium, Li, 3, 6.94);
CreateChemicalElement(Beryllium, Be, 4, 9.0122);
CreateChemicalElement(Boron, B, 5, 10.81);
CreateChemicalElement(Carbon, C, 6, 12.011);
CreateChemicalElement(Nitrogen, N, 7, 14.007);
CreateChemicalElement(Oxygen, O, 8, 15.999);
CreateChemicalElement(Fluorine, F, 9, 18.998);
CreateChemicalElement(Neon, Ne, 10, 20.18);

CreateChemicalElement(Potassium, K, 19, 39.098);
CreateChemicalElement(Manganese, Mn, 25, 54.938);

static constexpr const char* chemicalSymbols[] = {
    "H", "Li", "Na", "K", "Rb", "Cs", "Fr",
    "Be", "Mg", "Ca", "Sr", "Ba", "Ra",
    "Sc", "Y", "Lu", "Lr",
    "Ti", "Zr", "Hf", "Rf",
    "V", "Nb", "Ta", "Db",
    "Cr", "Mo", "W", "Sg",
    "Mn", "Tc", "Re", "Bh",
    "Fe", "Ru", "Os", "Hs",
    "Co", "Rh", "Ir", "Mt",
    "Ni", "Pd", "Pt", "Ds",
    "Cu", "Ag", "Au", "Rg",
    "Zn", "Cd", "Hg", "Cn",
    "B", "Al", "Ga", "In", "Tl", "Nh",
    "C", "Si", "Ge", "Sn", "Pb", "Fl",
    "N", "P", "As", "Sb", "Bi", "Mc",
    "O", "S", "Se", "Te", "Po", "Lv",
    "F", "Cl", "Br", "I", "At", "Ts",
    "He", "Ne", "Ar", "Kr", "Xe", "Rn", "Og",
    "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
    "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No",
};
template <typename T>
struct ChemicalMolecule {
    ChemicalMolecule(std::vector<ChemicalElement<T>> elements_, size_t count_) {
        elements = elements_;
        count = count_;
    }
    constexpr std::vector<ChemicalElement<T>> GetElements(void) const {
        std::vector<ChemicalElement<T>> ret = elements;
        for (ChemicalElement<T>& element : ret) element *= count;
        return ret;
    }
    constexpr std::string ToString(void) const {
        std::string str = "";
        for (const ChemicalElement<T>& element : elements) str += element.ToString();
        return (count == 1 ? "" : std::to_string(count)) + str;
    }

    size_t count;
    private:
    std::vector<ChemicalElement<T>> elements;
};
template <typename T>
struct ChemicalReaction {
    ChemicalReaction<T>(std::vector<ChemicalMolecule<num_t>> l, std::vector<ChemicalMolecule<num_t>> r) {
        left = l;
        right = r;
    }
    constexpr std::vector<std::vector<ChemicalElement<num_t>>> GetLeftList(void) const {
        std::vector<std::vector<ChemicalElement<num_t>>> ret;
        for (const ChemicalMolecule<num_t>& mol : left) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr std::vector<std::vector<ChemicalElement<num_t>>> GetRightList(void) const {
        std::vector<std::vector<ChemicalElement<num_t>>> ret;
        for (const ChemicalMolecule<num_t>& mol : right) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr size_t GetLeftElementCount(std::string symbol) const {
        return GetElementCount(GetLeftList(), symbol);
    }
    constexpr size_t GetRightElementCount(std::string symbol) const {
        return GetElementCount(GetRightList(), symbol);
    }
    constexpr void SetCount(std::vector<size_t> count) {
        for (size_t i = 0; i < count.size(); i++) {
            if (i < left.size()) left.at(i).count = count.at(i);
            else right.at(i - left.size()).count = count.at(i);
        }
    }
    constexpr bool BalanceReaction(size_t maxCoefficient) {
        const size_t size = left.size() + right.size();
        std::vector<size_t> count = std::vector<size_t>(size, 1);
        while (true) {
            ChemicalReaction<T> tmp = *this;
            tmp.SetCount(count);
            bool ok = true;
            for (auto& symbol : chemicalSymbols) {
                if (tmp.GetLeftElementCount(symbol) != tmp.GetRightElementCount(symbol)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                SetCount(count);
                return true;
            }
            ssize_t i;
            for (i = size - 1; i >= 0 && ++count[i] > maxCoefficient; i--) count[i] = 1;
            if (i < 0) return false;
        }
    }
    constexpr std::string ToString(void) const {
        std::string ret = "";
        for (size_t i = 0; i < left.size(); i++)
            ret += left.at(i).ToString() + ((i + 1) == left.size() ? "" : " + ");
        ret += " => ";
        for (size_t i = 0; i < right.size(); i++)
            ret += right.at(i).ToString() + ((i + 1) == right.size() ? "" : " + ");
        return ret;
    }

    private:
    std::vector<ChemicalMolecule<num_t>> left;
    std::vector<ChemicalMolecule<num_t>> right;

    static constexpr size_t GetElementCount(std::vector<std::vector<ChemicalElement<T>>> molecules, std::string symbol) {
        size_t ret = 0;
        for (const std::vector<ChemicalElement<T>>& molecule : molecules) {
            for (const ChemicalElement<T>& element : molecule) {
                if (element.GetSymbol() == symbol) {
                    ret += element.GetCount();
                    break;
                }
            }
        }
        return ret;
    }
};

#endif