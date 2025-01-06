#ifndef HTTPResponse_H
#define HTTPResponse_H
#include "HTTPHeader.hpp"
#include "HTTPStatus.hpp"

struct HTTPResponse : MathLib::Printable {
    HTTPResponse(MathLib::String str = "");
    [[nodiscard]] static HTTPResponse FromStatus(HTTPStatus status, MathLib::String desc);
    [[nodiscard]] static HTTPResponse FromHTML(MathLib::String str);
    [[nodiscard]] MathLib::String GetRaw(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    MathLib::String version;
    MathLib::String status;
    MathLib::String description;
    MathLib::Array<HTTPHeader> headers;
    MathLib::String body;
};

#endif