#include "ChemicalElement.hpp"

namespace MathLib {
    ChemicalElement::ChemicalElement(void) : symbol(""), count(0), metal(false) {
        EmptyBenchmark
    }
    ChemicalElement::ChemicalElement(const String& symbol, size_t count, bool metal) : symbol(symbol), count(count), metal(metal) {
        EmptyBenchmark
    }
    ChemicalElement::ChemicalElement(const ChemicalElement& self, size_t count) : ChemicalElement(self.symbol, count, self.metal) {
        EmptyBenchmark
    }
    String ChemicalElement::GetSymbol(void) const {
        StartBenchmark
        ReturnFromBenchmark(symbol);
    }
    size_t ChemicalElement::GetCount(void) const {
        StartBenchmark
        ReturnFromBenchmark(count);
    }
    bool ChemicalElement::IsMetal(void) const {
        StartBenchmark
        ReturnFromBenchmark(metal);
    }
    String ChemicalElement::ToString(const String& padding) const {
        StartBenchmark
        ReturnFromBenchmark(padding + symbol + (count == 1 ? "" : MathLib::ToString(count)));
    }
    ChemicalElement ChemicalElement::operator*=(size_t num) {
        StartBenchmark
        count *= num;
        ReturnFromBenchmark(*this);
    }
}