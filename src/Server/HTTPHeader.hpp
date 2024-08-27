#ifndef HTTPHeader_H
#define HTTPHeader_H
#include <Interfaces/Printable.hpp>

struct HTTPHeader : Printable {
    String name;
    String value;

    HTTPHeader(String n = "", String val = "");
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif