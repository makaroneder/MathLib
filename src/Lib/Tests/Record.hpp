#ifndef Tests_Record_H
#define Tests_Record_H
#include "../Interfaces/Printable.hpp"

struct Record : Printable {
    String expression;
    bool passed;

    Record(const String& expression = "", bool passed = false);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif