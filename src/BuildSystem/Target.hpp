#ifndef Target_H
#define Target_H
#include <Interfaces/Printable.hpp>

struct Target : Printable {
    String name;
    String deps;
    String command;
    bool phony;

    Target(const String& name, const String& deps, const String& command, bool phony);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};


#endif