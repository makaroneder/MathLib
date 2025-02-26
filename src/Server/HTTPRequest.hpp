#ifndef HTTPRequest_H
#define HTTPRequest_H
#include "HTTPHeader.hpp"

struct HTTPRequest : MathLib::Printable {
    HTTPRequest(MathLib::String str = "");
    [[nodiscard]] MathLib::String GetRaw(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    MathLib::String method;
    MathLib::String target;
    MathLib::String version;
    MathLib::Array<HTTPHeader> headers;
    MathLib::String body;
};

#endif