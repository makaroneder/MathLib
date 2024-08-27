#ifndef Record_H
#define Record_H
#include "../Interfaces/Printable.hpp"

struct Record : Printable {
    String expression;
    bool passed;

    Record(const String& expr = "", const bool& pass = false);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif