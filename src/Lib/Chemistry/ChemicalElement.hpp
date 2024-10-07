#ifndef Chemistry_Element_H
#define Chemistry_Element_H
#include "../Host.hpp"

struct ChemicalElement : Printable {
    ChemicalElement(void);
    ChemicalElement(const String& symbol, size_t count, bool metal);
    ChemicalElement(const ChemicalElement& self, size_t count);
    String GetSymbol(void) const;
    size_t GetCount(void) const;
    bool IsMetal(void) const;
    virtual String ToString(const String& padding = "") const override;
    ChemicalElement operator*=(size_t num);

    private:
    String symbol;
    size_t count;
    bool metal;
};

#define CreateChemicalElement(name, symbol, metal)                  \
struct name : ChemicalElement {                                     \
    name(size_t count) : ChemicalElement(#symbol, count, metal) {}  \
}

CreateChemicalElement(Hydrogen, H, false);
CreateChemicalElement(Helium, He, false);
CreateChemicalElement(Lithium, Li, true);
CreateChemicalElement(Beryllium, Be, true);
CreateChemicalElement(Boron, B, false);
CreateChemicalElement(Carbon, C, false);
CreateChemicalElement(Nitrogen, N, false);
CreateChemicalElement(Oxygen, O, false);
CreateChemicalElement(Fluorine, F, false);
CreateChemicalElement(Neon, Ne, false);
CreateChemicalElement(Sodium, Na, true);
CreateChemicalElement(Magnesium, Mg, true);
CreateChemicalElement(Aluminium, Al, true);
CreateChemicalElement(Silicon, Si, false);
CreateChemicalElement(Phosphorus, P, false);
CreateChemicalElement(Sulfur, S, false);
CreateChemicalElement(Chlorine, Cl, false);
CreateChemicalElement(Argon, Ar, false);
CreateChemicalElement(Potassium, K, true);
CreateChemicalElement(Calcium, Ca, true);
CreateChemicalElement(Scandium, Sc, true);
CreateChemicalElement(Titanium, Ti, true);
CreateChemicalElement(Vanadium, V, true);
CreateChemicalElement(Chromium, Cr, true);
CreateChemicalElement(Manganese, Mn, true);
CreateChemicalElement(Iron, Fe, true);
CreateChemicalElement(Cobalt, Co, true);
CreateChemicalElement(Nickel, Ni, true);
CreateChemicalElement(Copper, Cu, true);
CreateChemicalElement(Zinc, Zn, true);
CreateChemicalElement(Gallium, Ga, true);
CreateChemicalElement(Germanium, Ge, true);
CreateChemicalElement(Arsenic, As, false);
CreateChemicalElement(Selenium, Se, false);
CreateChemicalElement(Bromine, Br, false);
CreateChemicalElement(Krypton, Kr, false);
CreateChemicalElement(Rubidium, Rb, true);
CreateChemicalElement(Strontium, Sr, true);
CreateChemicalElement(Yttrium, Y, true);
CreateChemicalElement(Zirconium, Zr, true);
CreateChemicalElement(Niobium, Nb, true);
CreateChemicalElement(Molybdenum, Mo, true);
CreateChemicalElement(Technetium, Tc, true);
CreateChemicalElement(Ruthenium, Ru, true);
CreateChemicalElement(Rhodium, Rh, true);
CreateChemicalElement(Palladium, Pd, true);
CreateChemicalElement(Silver, Ag, true);
CreateChemicalElement(Cadmium, Cd, true);
CreateChemicalElement(Indium, In, true);
CreateChemicalElement(Tin, Sn, true);
CreateChemicalElement(Antimony, Sb, true);
CreateChemicalElement(Tellurium, Te, false);
CreateChemicalElement(Iodine, I, false);
CreateChemicalElement(Xenon, Xe, false);
CreateChemicalElement(Caesium, Cs, true);
CreateChemicalElement(Barium, Ba, true);
CreateChemicalElement(Lanthanum, La, true);
CreateChemicalElement(Cerium, Ce, true);
CreateChemicalElement(Praseodymium, Pr, true);
CreateChemicalElement(Neodymium, Nd, true);
CreateChemicalElement(Promethium, Pm, true);
CreateChemicalElement(Samarium, Sm, true);
CreateChemicalElement(Europium, Eu, true);
CreateChemicalElement(Gadolinium, Gd, true);
CreateChemicalElement(Terbium, Tb, true);
CreateChemicalElement(Dysprosium, Dy, true);
CreateChemicalElement(Holmium, Ho, true);
CreateChemicalElement(Erbium, Er, true);
CreateChemicalElement(Thulium, Tm, true);
CreateChemicalElement(Ytterbium, Yb, true);
CreateChemicalElement(Lutetium, Lu, true);
CreateChemicalElement(Hafnium, Hf, true);
CreateChemicalElement(Tantalum, Ta, true);
CreateChemicalElement(Tungsten, W, true);
CreateChemicalElement(Rhenium, Re, true);
CreateChemicalElement(Osmium, Os, true);
CreateChemicalElement(Iridium, Ir, true);
CreateChemicalElement(Platinum, Pt, true);
CreateChemicalElement(Gold, Au, true);
CreateChemicalElement(Mercury, Hg, true);
CreateChemicalElement(Thallium, Tl, true);
CreateChemicalElement(Lead, Pb, true);
CreateChemicalElement(Bismuth, Bi, true);
CreateChemicalElement(Polonium, Po, true);
CreateChemicalElement(Astatine, At, false);
CreateChemicalElement(Radon, Rn, false);
CreateChemicalElement(Francium, Fr, true);
CreateChemicalElement(Radium, Ra, true);
CreateChemicalElement(Actinium, Ac, true);
CreateChemicalElement(Thorium, Th, true);
CreateChemicalElement(Protactinium, Pa, true);
CreateChemicalElement(Uranium, U, true);
CreateChemicalElement(Neptunium, Np, true);
CreateChemicalElement(Plutonium, Pu, true);
CreateChemicalElement(Americium, Am, true);
CreateChemicalElement(Curium, Cm, true);
CreateChemicalElement(Berkelium, Bk, true);
CreateChemicalElement(Californium, Cf, true);
CreateChemicalElement(Einsteinium, Es, true);
CreateChemicalElement(Fermium, Fm, true);
CreateChemicalElement(Mendelevium, Md, true);
CreateChemicalElement(Nobelium, No, true);
CreateChemicalElement(Lawrencium, Lr, true);
CreateChemicalElement(Rutherfordium, Rf, true);
CreateChemicalElement(Dubnium, Db, true);
CreateChemicalElement(Seaborgium, Sg, true);
CreateChemicalElement(Bohrium, Bh, true);
CreateChemicalElement(Hassium, Hs, true);
CreateChemicalElement(Meitnerium, Mt, true);
CreateChemicalElement(Darmstadtium, Ds, true);
CreateChemicalElement(Roentgenium, Rg, true);
CreateChemicalElement(Copernicium, Cn, true);
CreateChemicalElement(Nihonium, Nh, true);
CreateChemicalElement(Flerovium, Fl, true);
CreateChemicalElement(Moscovium, Mc, true);
CreateChemicalElement(Livermorium, Lv, true);
CreateChemicalElement(Tennessine, Ts, true);
CreateChemicalElement(Oganesson, Og, true);

#endif