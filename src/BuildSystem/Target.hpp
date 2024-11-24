#ifndef Target_H
#define Target_H
#include <Interfaces/Printable.hpp>

struct Target : MathLib::Printable {
    MathLib::String name;
    MathLib::String deps;
    MathLib::String command;
    bool phony;

    Target(const MathLib::String& name, const MathLib::String& deps, const MathLib::String& command, bool phony);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};


#endif