#ifndef MathLib_Chemistry_ReactionElement_H
#define MathLib_Chemistry_ReactionElement_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    struct ChemicalReactionElement : Printable {
        ChemicalReactionElement(void);
        ChemicalReactionElement(const Sequence<char>& symbol, size_t size);
        void SetCount(const Sequence<size_t>& counts, size_t start);
        [[nodiscard]] size_t GetCoefficients(void) const;
        [[nodiscard]] String GetSymbol(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        Matrix<ssize_t> coefficients;
        private:
        String symbol;
    };
}

#endif