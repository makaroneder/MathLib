#ifndef Chemistry_ReactionElement_H
#define Chemistry_ReactionElement_H
#include "../Matrix.hpp"

struct ChemicalReactionElement : Printable {
    String symbol;
    Matrix<ssize_t> coefficients;

    ChemicalReactionElement(void) {}
    ChemicalReactionElement(const String& sym, const size_t& size) : symbol(sym), coefficients(Matrix<ssize_t>(size, 1)) {}
    void SetCount(const Array<size_t>& counts, const size_t& start) {
        for (size_t i = 0; i < coefficients.GetWidth(); i++) coefficients.At(i, 0) *= counts.At(i + start);
    }
    size_t GetCoefficients(void) const {
        size_t ret = 0;
        for (size_t i = 0; i < coefficients.GetWidth(); i++) ret += coefficients.At(i, 0);
        return ret;
    }
    virtual String ToString(const String& padding = "") const override {
        return padding + symbol + ": " + coefficients.ToString();
    }
};

#endif