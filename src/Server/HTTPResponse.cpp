#include "HTTPResponse.hpp"
#include <String.hpp>
#include <Host.hpp>

HTTPResponse::HTTPResponse(const MathLib::Sequence<char>& str) {
    if (!str.IsEmpty()) {
        MathLib::Array<MathLib::String> lines = MathLib::Split(str, "\r\n"_M, true);
        MathLib::Array<MathLib::String> statusLine = MathLib::Split(lines.At(0), " "_M, true);
        version = MathLib::SubString(statusLine.At(0), 0, statusLine.At(0).GetSize() - 1);
        status = MathLib::SubString(statusLine.At(1), 0, statusLine.At(1).GetSize() - 1);
        description = MathLib::SubString(statusLine.At(2), 0, statusLine.At(2).GetSize() - 1);
        const size_t size = lines.GetSize();
        size_t bodyIndex = size;
        for (size_t i = 1; i < size; i++) {
            MathLib::String line = MathLib::SubString(lines.At(i), 0, lines.At(i).GetSize() - 1);
            if (line == "\r") {
                for (const HTTPHeader& header : headers) {
                    if (header.name == "Content-Length") {
                        bodyIndex = i + 1;
                        break;
                    }
                }
                break;
            }
            MathLib::Array<MathLib::String> tmp = MathLib::Split(line, ":"_M, true);
            size_t off = 0;
            while (tmp.At(1).At(off) == ' ') off++;
            if (!headers.Add(HTTPHeader(MathLib::SubString(tmp.At(0), 0, tmp.At(0).GetSize() - 1), MathLib::SubString(tmp.At(1), off, tmp.At(1).GetSize() - off)))) MathLib::Panic("Failed to add HTTP header");
        }
        for (size_t i = bodyIndex; i < size; i++) body += lines.At(i) + "\n";
    }
}
HTTPResponse HTTPResponse::FromStatus(HTTPStatus status, const MathLib::Sequence<char>& desc) {
    HTTPResponse ret = HTTPResponse();
    ret.version = "HTTP/1.1";
    ret.status = MathLib::ToString((size_t)status, 10);
    ret.description = MathLib::CollectionToString(desc);
    return ret;
}
HTTPResponse HTTPResponse::FromHTML(const MathLib::Sequence<char>& str) {
    HTTPResponse ret = FromStatus(HTTPStatus::Success, "OK"_M);
    if (!ret.headers.Add(HTTPHeader("Content-Type"_M, "text/html; charset=utf-8"_M))) return HTTPResponse();
    if (!ret.headers.Add(HTTPHeader("Content-Length"_M, MathLib::ToString(str.GetSize())))) return HTTPResponse();
    ret.body = MathLib::CollectionToString(str);
    return ret;
}
MathLib::String HTTPResponse::GetRaw(void) const {
    MathLib::String ret = version + " " + status + " " + description + "\r\n";
    for (const HTTPHeader& header : headers) ret += header.name + ": " + header.value + "\r\n";
    return ret + "\r\n" + body;
}
MathLib::String HTTPResponse::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "Status line:\n";
    ret += MathLib::CollectionToString(padding) + "\tHTTP version: " + version + '\n';
    ret += MathLib::CollectionToString(padding) + "\tStatus: " + status + '\n';
    ret += MathLib::CollectionToString(padding) + "\tDescription: " + description + '\n';
    if (!headers.IsEmpty()) {
        ret += MathLib::CollectionToString(padding) + "HTTP headers:\n";
        for (const HTTPHeader& header : headers) ret += header.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    }
    if (!body.IsEmpty()) ret += MathLib::CollectionToString(padding) + "Body:\n" + body + '\n';
    return ret;
}