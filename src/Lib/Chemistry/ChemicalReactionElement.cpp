#include "ChemicalReactionElement.hpp"

ChemicalReactionElement::ChemicalReactionElement(void) {}
ChemicalReactionElement::ChemicalReactionElement(const String& symbol, size_t size) : coefficients(Matrix<ssize_t>(size, 1)), symbol(symbol) {}
void ChemicalReactionElement::SetCount(const Array<size_t>& counts, size_t start) {
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
String ChemicalReactionElement::ToString(const String& padding) const {
    return padding + symbol + ": " + coefficients.ToString();
}