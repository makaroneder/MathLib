#ifndef MathLib_Tests_Record_H
#define MathLib_Tests_Record_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct Record : Printable {
        String expression;
        bool passed;

        Record(const Sequence<char>& expression = ""_M, bool passed = false);
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
    };
}

#endif