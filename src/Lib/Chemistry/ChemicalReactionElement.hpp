#ifndef Chemistry_ReactionElement_H
#define Chemistry_ReactionElement_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    struct ChemicalReactionElement : Printable {
        ChemicalReactionElement(void);
        ChemicalReactionElement(const String& symbol, size_t size);
        void SetCount(const Array<size_t>& counts, size_t start);
        size_t GetCoefficients(void) const;
        String GetSymbol(void) const;
        virtual String ToString(const String& padding = "") const override;

        Matrix<ssize_t> coefficients;

        private:
        String symbol;
    };
}

#endif