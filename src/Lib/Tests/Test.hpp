#ifndef MathLib_Test_H
#define MathLib_Test_H
#include "Record.hpp"

namespace MathLib {
    struct Test : Printable {
        [[nodiscard]] bool ReportRecord(const Record& record);
        [[nodiscard]] bool Passed(void) const;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        Array<Record> records;
    };
    #define TestOperation(test, expr) if (!test.ReportRecord(MathLib::Record(MathLib::String(#expr), expr))) MathLib::Panic("Failed to add test record")

    [[nodiscard]] Test TestSelf(void);
}

#endif