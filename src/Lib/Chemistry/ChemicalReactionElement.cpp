#include "ChemicalReactionElement.hpp"

namespace MathLib {
    ChemicalReactionElement::ChemicalReactionElement(void) {}
    ChemicalReactionElement::ChemicalReactionElement(const Sequence<char>& symbol, size_t size) : coefficients(Matrix<ssize_t>(size, 1)), symbol(CollectionToString(symbol)) {}
    void ChemicalReactionElement::SetCount(const Sequence<size_t>& counts, size_t start) {
        for (size_t i = 0; i < coefficients.GetWidth(); i++) coefficients.At(i, 0) *= counts.At(i + start);
    }
    size_t ChemicalReactionElement::GetCoefficients(void) const {
        size_t ret = 0;
        for (size_t i = 0; i < coefficients.GetWidth(); i++) ret += coefficients.At(i, 0);
        return ret;
    }
    String ChemicalReactionElement::GetSymbol(void) const {
        return symbol;
    }
    String ChemicalReactionElement::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + symbol + ": " + coefficients.ToString();
    }
    bool ChemicalReactionElement::operator==(const ChemicalReactionElement& other) const {
        return coefficients == other.coefficients && symbol == other.symbol;
    }
    bool ChemicalReactionElement::operator!=(const ChemicalReactionElement& other) const {
        return !(*this == other);
    }
}