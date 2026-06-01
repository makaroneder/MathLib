#ifndef MathLib_Tests_Record_H
#define MathLib_Tests_Record_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct Record;
    struct Record : Comparable<Record>, Printable {
        String expression;
        bool passed;

        Record(const Sequence<char>& expression = ""_M, bool passed = false);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        protected:
        [[nodiscard]] virtual bool Equals(const Record& other) const override;
    };
}

#endif