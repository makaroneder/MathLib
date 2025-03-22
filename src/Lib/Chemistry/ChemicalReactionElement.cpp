#include "ChemicalReactionElement.hpp"

namespace MathLib {
    ChemicalReactionElement::ChemicalReactionElement(void) {
        EmptyBenchmark
    }
    ChemicalReactionElement::ChemicalReactionElement(const Sequence<char>& symbol, size_t size) : coefficients(Matrix<ssize_t>(size, 1)), symbol(CollectionToString(symbol)) {
        EmptyBenchmark
    }
    void ChemicalReactionElement::SetCount(const Sequence<size_t>& counts, size_t start) {
        StartBenchmark
        for (size_t i = 0; i < coefficients.GetWidth(); i++) coefficients.At(i, 0) *= counts.At(i + start);
        EndBenchmark
    }
    size_t ChemicalReactionElement::GetCoefficients(void) const {
        StartBenchmark
        size_t ret = 0;
        for (size_t i = 0; i < coefficients.GetWidth(); i++) ret += coefficients.At(i, 0);
        ReturnFromBenchmark(ret);
    }
    String ChemicalReactionElement::GetSymbol(void) const {
        StartAndReturnFromBenchmark(symbol);
    }
    String ChemicalReactionElement::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + symbol + ": " + coefficients.ToString());
    }
}