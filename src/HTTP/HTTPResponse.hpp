#ifndef HTTPResponse_H
#define HTTPResponse_H
#include "HTTPHeader.hpp"
#include "HTTPStatus.hpp"

struct HTTPResponse : MathLib::Printable {
    HTTPResponse(const MathLib::Sequence<char>& str = ""_M);
    [[nodiscard]] static HTTPResponse FromStatus(HTTPStatus status, const MathLib::Sequence<char>& desc);
    [[nodiscard]] static HTTPResponse FromHTML(const MathLib::Sequence<char>& str);
    [[nodiscard]] MathLib::String GetRaw(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    MathLib::String version;
    MathLib::String status;
    MathLib::String description;
    MathLib::Array<HTTPHeader> headers;
    MathLib::String body;
};

#endif