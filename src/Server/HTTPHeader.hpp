#ifndef HTTPHeader_H
#define HTTPHeader_H
#include <Interfaces/Printable.hpp>

struct HTTPHeader : MathLib::Printable {
    MathLib::String name;
    MathLib::String value;

    HTTPHeader(MathLib::String n = "", MathLib::String val = "");
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};

#endif