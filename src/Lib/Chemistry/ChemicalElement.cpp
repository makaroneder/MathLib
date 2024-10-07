#include "ChemicalElement.hpp"

ChemicalElement::ChemicalElement(void) {}
ChemicalElement::ChemicalElement(const String& symbol, size_t count, bool metal) : symbol(symbol), count(count), metal(metal) {}
ChemicalElement::ChemicalElement(const ChemicalElement& self, size_t count) : ChemicalElement(self.symbol, count, self.metal) {}
String ChemicalElement::GetSymbol(void) const {
    return symbol;
}
size_t ChemicalElement::GetCount(void) const {
    return count;
}
bool ChemicalElement::IsMetal(void) const {
    return metal;
}
String ChemicalElement::ToString(const String& padding) const {
    return padding + symbol + (count == 1 ? "" : ::ToString(count));
}
ChemicalElement ChemicalElement::operator*=(size_t num) {
    count *= num;
    return *this;
}