#ifndef HTTPResponse_H
#define HTTPResponse_H
#include "HTTPHeader.hpp"

enum class HTTPStatus {
    Continue, SwitchingProtocols, Processing, EarlyHints,
    Success, Created, Accepted, NonAuthoritativeInformation, NoContent, ResetContent,
    PartialContent, MultiStatus, AlreadyReported, IMUsed,
    MultipleChoices, MovedPermanently, Found, SeeOther, NotModified, UseProxy, Reserved,
    TemporaryRedirect, PermanentRedirect,
    BadRequest, Unauthorized, PaymentRequired, Forbidden, NotFound, MethodNotAllowed,
    NotAcceptable, ProxyAuthenticationRequired, RequestTimeout, Conflict, Gone, LengthRequired,
    PreconditionFailed, PayloadTooLarge, URITooLong, UnsupportedMediaType, RangeNotSatisfiable,
    ExpectationFailed, Teapot, MisdirectedRequest, UnprocessableContent, Locked, FailedDependecy,
    TooEarly, UpgradeRequired, PreconditionRequired, TooManyRequests, RequestHeaderFieldsTooLarge,
    UnavailableForLegalReasons,
    InternalError, NotImplemented, BadGateway, ServiceUnavailable, GatewayTimeout,
    HTTPVersionNotSupported, VariantAlsoNegotiates, InsufficientStorage, LoopDetected,
    NotExtended, NetworkAuthenticationRequired,
};
static constexpr const char* httpStatusStr[] = {
    "100", "101", "102", "103",
    "200", "201", "202", "203", "204", "205",
    "206", "207", "208", "226",
    "300", "301", "302", "303", "304", "305", "306",
    "307", "308",
    "400", "401", "402", "403", "404", "405",
    "406", "407", "408", "409", "410", "411",
    "412", "413", "414", "415", "416",
    "417", "418", "421", "422", "423", "424",
    "425", "426", "428", "429", "431",
    "451",
    "500", "501", "502", "503", "504",
    "505", "506", "507", "508",
    "510", "511",
};
struct HTTPResponse : MathLib::Printable {
    HTTPResponse(MathLib::String str = "");
    static HTTPResponse FromStatus(HTTPStatus status, MathLib::String desc);
    static HTTPResponse FromHTML(MathLib::String str);
    MathLib::String Raw(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    MathLib::String version;
    MathLib::String status;
    MathLib::String description;
    MathLib::Array<HTTPHeader> headers;
    MathLib::String body;
};

#endif