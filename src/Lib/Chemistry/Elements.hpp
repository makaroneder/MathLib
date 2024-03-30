#ifndef Chemistry_Elements_H
#define Chemistry_Elements_H
#include "../Physics/SIUnits.hpp"

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
struct ChemicalElement : Printable {
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
    virtual std::string ToString(std::string padding = "") const override {
        return padding + symbol + (count == 1 ? "" : std::to_string(count));
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
CreateChemicalElement(Sodium, Na, 11, 22.99);
CreateChemicalElement(Magnesium, Mg, 12, 24.305);
CreateChemicalElement(Aluminium, Al, 13, 26.982);
CreateChemicalElement(Silicon, Si, 14, 28.085);
CreateChemicalElement(Phosphorus, P, 15, 30.974);
CreateChemicalElement(Sulfur, S, 16, 32.06);
CreateChemicalElement(Chlorine, Cl, 17, 35.45);
CreateChemicalElement(Argon, Ar, 18, 39.95);
CreateChemicalElement(Potassium, K, 19, 39.098);
CreateChemicalElement(Calcium, Ca, 20, 40.078);
CreateChemicalElement(Scandium, Sc, 21, 44.956);
CreateChemicalElement(Titanium, Ti, 22, 47.867);
CreateChemicalElement(Vanadium, V, 23, 50.942);
CreateChemicalElement(Chromium, Cr, 24, 51.996);
CreateChemicalElement(Manganese, Mn, 25, 54.938);
CreateChemicalElement(Iron, Fe, 26, 55.845);
CreateChemicalElement(Cobalt, Co, 27, 58.933);
CreateChemicalElement(Nickel, Ni, 28, 58.693);
CreateChemicalElement(Copper, Cu, 29, 63.546);
CreateChemicalElement(Zinc, Zn, 30, 65.38);
CreateChemicalElement(Gallium, Ga, 31, 69.723);
CreateChemicalElement(Germanium, Ge, 32, 72.630);
CreateChemicalElement(Arsenic, As, 33, 74.922);
CreateChemicalElement(Selenium, Se, 34, 78.971);
CreateChemicalElement(Bromine, Br, 35, 79.904);
CreateChemicalElement(Krypton, Kr, 36, 83.798);
CreateChemicalElement(Rubidium, Rb, 37, 85.468);
CreateChemicalElement(Strontium, Sr, 38, 87.62);
CreateChemicalElement(Yttrium, Y, 39, 88.906);
CreateChemicalElement(Zirconium, Zr, 40, 91.224);
CreateChemicalElement(Niobium, Nb, 41, 92.906);
CreateChemicalElement(Molybdenum, Mo, 42, 95.95);
CreateChemicalElement(Technetium, Tc, 43, 97);
CreateChemicalElement(Ruthenium, Ru, 44, 101.07);
CreateChemicalElement(Rhodium, Rh, 45, 102.91);
CreateChemicalElement(Palladium, Pd, 46, 106.42);
CreateChemicalElement(Silver, Ag, 47, 107.87);
CreateChemicalElement(Cadmium, Cd, 48, 112.41);
CreateChemicalElement(Indium, In, 49, 114.82);
CreateChemicalElement(Tin, Sn, 50, 118.71);
CreateChemicalElement(Antimony, Sb, 51, 121.76);

#if 0
CreateChemicalElement(Manganese, Mn, 52, 54.938);
CreateChemicalElement(Manganese, Mn, 53, 54.938);
CreateChemicalElement(Manganese, Mn, 54, 54.938);
CreateChemicalElement(Manganese, Mn, 55, 54.938);
CreateChemicalElement(Manganese, Mn, 56, 54.938);
CreateChemicalElement(Manganese, Mn, 57, 54.938);
CreateChemicalElement(Manganese, Mn, 58, 54.938);
CreateChemicalElement(Manganese, Mn, 59, 54.938);
CreateChemicalElement(Manganese, Mn, 60, 54.938);
CreateChemicalElement(Manganese, Mn, 61, 54.938);
CreateChemicalElement(Manganese, Mn, 62, 54.938);
CreateChemicalElement(Manganese, Mn, 63, 54.938);
CreateChemicalElement(Manganese, Mn, 64, 54.938);
CreateChemicalElement(Manganese, Mn, 65, 54.938);
CreateChemicalElement(Manganese, Mn, 66, 54.938);
CreateChemicalElement(Manganese, Mn, 67, 54.938);
CreateChemicalElement(Manganese, Mn, 68, 54.938);
CreateChemicalElement(Manganese, Mn, 69, 54.938);
CreateChemicalElement(Manganese, Mn, 70, 54.938);
CreateChemicalElement(Manganese, Mn, 71, 54.938);
CreateChemicalElement(Manganese, Mn, 72, 54.938);
CreateChemicalElement(Manganese, Mn, 73, 54.938);
CreateChemicalElement(Manganese, Mn, 74, 54.938);
CreateChemicalElement(Manganese, Mn, 75, 54.938);
CreateChemicalElement(Manganese, Mn, 76, 54.938);
CreateChemicalElement(Manganese, Mn, 77, 54.938);
CreateChemicalElement(Manganese, Mn, 78, 54.938);
CreateChemicalElement(Manganese, Mn, 79, 54.938);
CreateChemicalElement(Manganese, Mn, 80, 54.938);
CreateChemicalElement(Manganese, Mn, 81, 54.938);
CreateChemicalElement(Manganese, Mn, 82, 54.938);
CreateChemicalElement(Manganese, Mn, 83, 54.938);
CreateChemicalElement(Manganese, Mn, 84, 54.938);
CreateChemicalElement(Manganese, Mn, 85, 54.938);
CreateChemicalElement(Manganese, Mn, 86, 54.938);
CreateChemicalElement(Manganese, Mn, 87, 54.938);
CreateChemicalElement(Manganese, Mn, 88, 54.938);
CreateChemicalElement(Manganese, Mn, 89, 54.938);
CreateChemicalElement(Manganese, Mn, 90, 54.938);
CreateChemicalElement(Manganese, Mn, 91, 54.938);
CreateChemicalElement(Manganese, Mn, 92, 54.938);
CreateChemicalElement(Manganese, Mn, 93, 54.938);
CreateChemicalElement(Manganese, Mn, 94, 54.938);
CreateChemicalElement(Manganese, Mn, 95, 54.938);
CreateChemicalElement(Manganese, Mn, 96, 54.938);
CreateChemicalElement(Manganese, Mn, 97, 54.938);
CreateChemicalElement(Manganese, Mn, 98, 54.938);
CreateChemicalElement(Manganese, Mn, 99, 54.938);
CreateChemicalElement(Manganese, Mn, 100, 54.938);
CreateChemicalElement(Manganese, Mn, 101, 54.938);
CreateChemicalElement(Manganese, Mn, 102, 54.938);
CreateChemicalElement(Manganese, Mn, 103, 54.938);
CreateChemicalElement(Manganese, Mn, 104, 54.938);
CreateChemicalElement(Manganese, Mn, 105, 54.938);
CreateChemicalElement(Manganese, Mn, 106, 54.938);
CreateChemicalElement(Manganese, Mn, 107, 54.938);
CreateChemicalElement(Manganese, Mn, 108, 54.938);
CreateChemicalElement(Manganese, Mn, 109, 54.938);
CreateChemicalElement(Manganese, Mn, 110, 54.938);
CreateChemicalElement(Manganese, Mn, 111, 54.938);
CreateChemicalElement(Manganese, Mn, 112, 54.938);
CreateChemicalElement(Manganese, Mn, 113, 54.938);
CreateChemicalElement(Manganese, Mn, 114, 54.938);
CreateChemicalElement(Manganese, Mn, 115, 54.938);
CreateChemicalElement(Manganese, Mn, 116, 54.938);
CreateChemicalElement(Manganese, Mn, 117, 54.938);
CreateChemicalElement(Manganese, Mn, 118, 54.938);
#endif

#endif