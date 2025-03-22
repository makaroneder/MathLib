#include "ChemicalElement.hpp"

namespace MathLib {
    ChemicalElement::ChemicalElement(void) : symbol(""), count(0), metal(false) {
        EmptyBenchmark
    }
    ChemicalElement::ChemicalElement(const Sequence<char>& symbol, size_t count, bool metal) : symbol(CollectionToString(symbol)), count(count), metal(metal) {
        EmptyBenchmark
    }
    ChemicalElement::ChemicalElement(const ChemicalElement& self, size_t count) : ChemicalElement(self.symbol, count, self.metal) {
        EmptyBenchmark
    }
    String ChemicalElement::GetSymbol(void) const {
        StartAndReturnFromBenchmark(symbol);
    }
    size_t ChemicalElement::GetCount(void) const {
        StartAndReturnFromBenchmark(count);
    }
    bool ChemicalElement::IsMetal(void) const {
        StartAndReturnFromBenchmark(metal);
    }
    String ChemicalElement::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + symbol + (count == 1 ? "" : MathLib::ToString(count)));
    }
    ChemicalElement& ChemicalElement::operator*=(size_t num) {
        StartBenchmark
        count *= num;
        ReturnFromBenchmark(*this);
    }
}