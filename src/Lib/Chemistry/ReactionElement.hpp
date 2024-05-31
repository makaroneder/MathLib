#ifndef Chemistry_ReactionElement_H
#define Chemistry_ReactionElement_H
#include "../Matrix.hpp"

struct ChemicalReactionElement : Printable {
    String symbol;
    Matrix<ssize_t> coefficients;

    ChemicalReactionElement(void) {}
    ChemicalReactionElement(String sym, size_t size) : symbol(sym), coefficients(Matrix<ssize_t>(size, 1)) {}
    void SetCount(Array<size_t> counts, size_t start) {
        for (size_t i = 0; i < coefficients.GetWidth(); i++) coefficients.At(i, 0) *= counts.At(i + start);
    }
    size_t GetCoefficients(void) const {
        size_t ret = 0;
        for (size_t i = 0; i < coefficients.GetWidth(); i++) ret += coefficients.At(i, 0);
        return ret;
    }
    /// @brief Converts matrix to string
    /// @param padding String to pad with
    /// @return String representation of matrix
    virtual String ToString(String padding = "") const override {
        return padding + symbol + ": " + coefficients.ToString();
    }
};

#endif