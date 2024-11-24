#ifndef Test_H
#define Test_H
#include "Record.hpp"

namespace MathLib {
    struct Test : Printable {
        bool ReportRecord(const Record& record);
        size_t GetRecordCount(void) const;
        size_t GetPassed(void) const;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        virtual String ToString(const String& padding = "") const override;

        private:
        Array<Record> records;
    };
    #define TestOperation(test, expr) if (!test.ReportRecord(MathLib::Record(#expr, expr))) MathLib::Panic("Failed to add test record")

    Test TestSelf(void);
}

#endif