#include "ChemicalElement.hpp"

namespace MathLib {
    ChemicalElement::ChemicalElement(void) : symbol(""), count(0), metal(false) {}
    ChemicalElement::ChemicalElement(const Sequence<char>& symbol, size_t count, bool metal) : symbol(CollectionToString(symbol)), count(count), metal(metal) {}
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
    String ChemicalElement::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + symbol + (count == 1 ? "" : Formatter<size_t>::ToString(count));
    }
    ChemicalElement& ChemicalElement::operator*=(size_t num) {
        count *= num;
        return *this;
    }
    bool ChemicalElement::operator==(const ChemicalElement& other) const {
        return symbol == other.symbol && count == other.count;
    }
    bool ChemicalElement::operator!=(const ChemicalElement& other) const {
        return !(*this == other);
    }
}