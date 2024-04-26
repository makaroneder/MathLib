#ifndef Chemistry_Elements_H
#define Chemistry_Elements_H
#include "../Physics/SIUnits.hpp"

template <typename T>
struct ChemicalElement : Printable {
    ChemicalElement(size_t atomicNumber, Dalton<T> atomicWeight, std::string sym, size_t count_, bool metal_) {
        number = atomicNumber;
        weight = atomicWeight;
        symbol = sym;
        count = count_;
        metal = metal_;
    }
    ChemicalElement(ChemicalElement<T> self, size_t count_) {
        number = self.number;
        weight = self.weight;
        symbol = self.symbol;
        metal = self.metal;
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
    constexpr bool IsMetal(void) const {
        return metal;
    }
    virtual std::string ToString(std::string padding = "") const override {
        return padding + symbol + (count == 1 ? "" : std::to_string(count));
    }
    constexpr ChemicalElement<T> operator*=(size_t num) {
        count *= num;
        return *this;
    }

    private:
    size_t number;
    Dalton<T> weight;
    std::string symbol;
    size_t count;
    bool metal;
};

#define CreateChemicalElement(name, symbol, number, weight, metal)                                      \
template <typename T>                                                                                   \
struct name : ChemicalElement<T> {                                                                      \
    name<T>(size_t count_) : ChemicalElement<T>(number, Dalton<T>(weight), #symbol, count_, metal) {}   \
}

CreateChemicalElement(Hydrogen, H, 1, 1.008, false);
CreateChemicalElement(Helium, He, 2, 4.0026, false);
CreateChemicalElement(Lithium, Li, 3, 6.94, true);
CreateChemicalElement(Beryllium, Be, 4, 9.0122, true);
CreateChemicalElement(Boron, B, 5, 10.81, false);
CreateChemicalElement(Carbon, C, 6, 12.011, false);
CreateChemicalElement(Nitrogen, N, 7, 14.007, false);
CreateChemicalElement(Oxygen, O, 8, 15.999, false);
CreateChemicalElement(Fluorine, F, 9, 18.998, false);
CreateChemicalElement(Neon, Ne, 10, 20.18, false);
CreateChemicalElement(Sodium, Na, 11, 22.99, true);
CreateChemicalElement(Magnesium, Mg, 12, 24.305, true);
CreateChemicalElement(Aluminium, Al, 13, 26.982, true);
CreateChemicalElement(Silicon, Si, 14, 28.085, false);
CreateChemicalElement(Phosphorus, P, 15, 30.974, false);
CreateChemicalElement(Sulfur, S, 16, 32.06, false);
CreateChemicalElement(Chlorine, Cl, 17, 35.45, false);
CreateChemicalElement(Argon, Ar, 18, 39.95, false);
CreateChemicalElement(Potassium, K, 19, 39.098, true);
CreateChemicalElement(Calcium, Ca, 20, 40.078, true);
CreateChemicalElement(Scandium, Sc, 21, 44.956, true);
CreateChemicalElement(Titanium, Ti, 22, 47.867, true);
CreateChemicalElement(Vanadium, V, 23, 50.942, true);
CreateChemicalElement(Chromium, Cr, 24, 51.996, true);
CreateChemicalElement(Manganese, Mn, 25, 54.938, true);
CreateChemicalElement(Iron, Fe, 26, 55.845, true);
CreateChemicalElement(Cobalt, Co, 27, 58.933, true);
CreateChemicalElement(Nickel, Ni, 28, 58.693, true);
CreateChemicalElement(Copper, Cu, 29, 63.546, true);
CreateChemicalElement(Zinc, Zn, 30, 65.38, true);
CreateChemicalElement(Gallium, Ga, 31, 69.723, true);
CreateChemicalElement(Germanium, Ge, 32, 72.630, true);
CreateChemicalElement(Arsenic, As, 33, 74.922, false);
CreateChemicalElement(Selenium, Se, 34, 78.971, false);
CreateChemicalElement(Bromine, Br, 35, 79.904, false);
CreateChemicalElement(Krypton, Kr, 36, 83.798, false);
CreateChemicalElement(Rubidium, Rb, 37, 85.468, true);
CreateChemicalElement(Strontium, Sr, 38, 87.62, true);
CreateChemicalElement(Yttrium, Y, 39, 88.906, true);
CreateChemicalElement(Zirconium, Zr, 40, 91.224, true);
CreateChemicalElement(Niobium, Nb, 41, 92.906, true);
CreateChemicalElement(Molybdenum, Mo, 42, 95.95, true);
CreateChemicalElement(Technetium, Tc, 43, 97, true);
CreateChemicalElement(Ruthenium, Ru, 44, 101.07, true);
CreateChemicalElement(Rhodium, Rh, 45, 102.91, true);
CreateChemicalElement(Palladium, Pd, 46, 106.42, true);
CreateChemicalElement(Silver, Ag, 47, 107.87, true);
CreateChemicalElement(Cadmium, Cd, 48, 112.41, true);
CreateChemicalElement(Indium, In, 49, 114.82, true);
CreateChemicalElement(Tin, Sn, 50, 118.71, true);
CreateChemicalElement(Antimony, Sb, 51, 121.76, true);

#if 0
CreateChemicalElement(Manganese, Mn, 52, 54.938, false);
CreateChemicalElement(Manganese, Mn, 53, 54.938, false);
CreateChemicalElement(Manganese, Mn, 54, 54.938, false);
CreateChemicalElement(Manganese, Mn, 55, 54.938, true);
CreateChemicalElement(Manganese, Mn, 56, 54.938, true);
CreateChemicalElement(Manganese, Mn, 57, 54.938, true);
CreateChemicalElement(Manganese, Mn, 58, 54.938, true);
CreateChemicalElement(Manganese, Mn, 59, 54.938, true);
CreateChemicalElement(Manganese, Mn, 60, 54.938, true);
CreateChemicalElement(Manganese, Mn, 61, 54.938, true);
CreateChemicalElement(Manganese, Mn, 62, 54.938, true);
CreateChemicalElement(Manganese, Mn, 63, 54.938, true);
CreateChemicalElement(Manganese, Mn, 64, 54.938, true);
CreateChemicalElement(Manganese, Mn, 65, 54.938, true);
CreateChemicalElement(Manganese, Mn, 66, 54.938, true);
CreateChemicalElement(Manganese, Mn, 67, 54.938, true);
CreateChemicalElement(Manganese, Mn, 68, 54.938, true);
CreateChemicalElement(Manganese, Mn, 69, 54.938, true);
CreateChemicalElement(Manganese, Mn, 70, 54.938, true);
CreateChemicalElement(Manganese, Mn, 71, 54.938, true);
CreateChemicalElement(Manganese, Mn, 72, 54.938, true);
CreateChemicalElement(Manganese, Mn, 73, 54.938, true);
CreateChemicalElement(Manganese, Mn, 74, 54.938, true);
CreateChemicalElement(Manganese, Mn, 75, 54.938, true);
CreateChemicalElement(Manganese, Mn, 76, 54.938, true);
CreateChemicalElement(Manganese, Mn, 77, 54.938, true);
CreateChemicalElement(Manganese, Mn, 78, 54.938, true);
CreateChemicalElement(Manganese, Mn, 79, 54.938, true);
CreateChemicalElement(Manganese, Mn, 80, 54.938, true);
CreateChemicalElement(Manganese, Mn, 81, 54.938, true);
CreateChemicalElement(Manganese, Mn, 82, 54.938, true);
CreateChemicalElement(Manganese, Mn, 83, 54.938, true);
CreateChemicalElement(Manganese, Mn, 84, 54.938, true);
CreateChemicalElement(Manganese, Mn, 85, 54.938, false);
CreateChemicalElement(Manganese, Mn, 86, 54.938, false);
CreateChemicalElement(Manganese, Mn, 87, 54.938, true);
CreateChemicalElement(Manganese, Mn, 88, 54.938, true);
CreateChemicalElement(Manganese, Mn, 89, 54.938, true);
CreateChemicalElement(Manganese, Mn, 90, 54.938, true);
CreateChemicalElement(Manganese, Mn, 91, 54.938, true);
CreateChemicalElement(Manganese, Mn, 92, 54.938, true);
CreateChemicalElement(Manganese, Mn, 93, 54.938, true);
CreateChemicalElement(Manganese, Mn, 94, 54.938, true);
CreateChemicalElement(Manganese, Mn, 95, 54.938, true);
CreateChemicalElement(Manganese, Mn, 96, 54.938, true);
CreateChemicalElement(Manganese, Mn, 97, 54.938, true);
CreateChemicalElement(Manganese, Mn, 98, 54.938, true);
CreateChemicalElement(Manganese, Mn, 99, 54.938, true);
CreateChemicalElement(Manganese, Mn, 100, 54.938, true);
CreateChemicalElement(Manganese, Mn, 101, 54.938, true);
CreateChemicalElement(Manganese, Mn, 102, 54.938, true);
CreateChemicalElement(Manganese, Mn, 103, 54.938, true);
CreateChemicalElement(Manganese, Mn, 104, 54.938, true);
CreateChemicalElement(Manganese, Mn, 105, 54.938, true);
CreateChemicalElement(Manganese, Mn, 106, 54.938, true);
CreateChemicalElement(Manganese, Mn, 107, 54.938, true);
CreateChemicalElement(Manganese, Mn, 108, 54.938, true);
CreateChemicalElement(Manganese, Mn, 109, 54.938, true);
CreateChemicalElement(Manganese, Mn, 110, 54.938, true);
CreateChemicalElement(Manganese, Mn, 111, 54.938, true);
CreateChemicalElement(Manganese, Mn, 112, 54.938, true);
CreateChemicalElement(Manganese, Mn, 113, 54.938, true);
CreateChemicalElement(Manganese, Mn, 114, 54.938, true);
CreateChemicalElement(Manganese, Mn, 115, 54.938, true);
CreateChemicalElement(Manganese, Mn, 116, 54.938, true);
CreateChemicalElement(Manganese, Mn, 117, 54.938, true);
CreateChemicalElement(Manganese, Mn, 118, 54.938, true);
#endif

#endif