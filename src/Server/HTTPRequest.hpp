#ifndef HTTPRequest_H
#define HTTPRequest_H
#include "HTTPHeader.hpp"

struct HTTPRequest : Printable {
    HTTPRequest(String str = "");
    String Raw(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    String method;
    String target;
    String version;
    Array<HTTPHeader> headers;
    String body;
};

#endif