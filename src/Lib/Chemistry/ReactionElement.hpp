#ifndef Chemistry_ReactionElement_H
#define Chemistry_ReactionElement_H
#include "../Matrix.hpp"

struct ChemicalReactionElement {
    std::string symbol;
    Matrix<ssize_t> coefficients;

    ChemicalReactionElement(std::string sym, size_t size) : symbol(sym), coefficients(Matrix<ssize_t>(size, 1)) {}
    void SetCount(std::vector<size_t> counts, size_t start) {
        for (size_t i = 0; i < coefficients.GetWidth(); i++) coefficients.At(i, 0) *= counts[i + start];
    }
    size_t GetCoefficients(void) const {
        size_t ret = 0;
        for (size_t i = 0; i < coefficients.GetWidth(); i++) ret += coefficients.At(i, 0);
        return ret;
    }
};

#endif