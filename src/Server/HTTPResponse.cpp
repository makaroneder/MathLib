#include "HTTPResponse.hpp"
#include <String.hpp>
#include <Host.hpp>

HTTPResponse::HTTPResponse(String str) {
    if (!str.IsEmpty()) {
        Array<String> lines = Split(str, "\r\n", true);
        Array<String> statusLine = Split(lines.At(0), " ", true);
        version = SubString(statusLine.At(0), 0, statusLine.At(0).GetSize() - 1);
        status = SubString(statusLine.At(1), 0, statusLine.At(1).GetSize() - 1);
        description = SubString(statusLine.At(2), 0, statusLine.At(2).GetSize() - 1);
        const size_t size = lines.GetSize();
        size_t bodyIndex = size;
        for (size_t i = 1; i < size; i++) {
            String line = SubString(lines.At(i), 0, lines.At(i).GetSize() - 1);
            if (line == "\r") {
                for (const HTTPHeader& header : headers) {
                    if (header.name == "Content-Length") {
                        bodyIndex = i + 1;
                        break;
                    }
                }
                break;
            }
            Array<String> tmp = Split(line, ":", true);
            size_t off = 0;
            while (tmp.At(1).At(off) == ' ') off++;
            headers.Add(HTTPHeader(SubString(tmp.At(0), 0, tmp.At(0).GetSize() - 1), SubString(tmp.At(1), off, tmp.At(1).GetSize() - off)));
        }
        for (size_t i = bodyIndex; i < size; i++) body += lines.At(i) + "\n";
    }
}
HTTPResponse HTTPResponse::FromStatus(HTTPStatus status, String desc) {
    HTTPResponse ret = HTTPResponse();
    ret.version = "HTTP/1.1";
    ret.status = httpStatusStr[(size_t)status];
    ret.description = desc;
    return ret;
}
HTTPResponse HTTPResponse::FromHTML(String str) {
    HTTPResponse ret = FromStatus(HTTPStatus::Success, "OK");
    ret.headers.Add(HTTPHeader("Content-Type", "text/html; charset=utf-8"));
    ret.headers.Add(HTTPHeader("Content-Length", ::ToString(str.GetSize())));
    ret.body = str;
    return ret;
}
String HTTPResponse::Raw(void) const {
    String ret = version + " " + status + " " + description + "\r\n";
    for (const HTTPHeader& header : headers) ret += header.name + ": " + header.value + "\r\n";
    return ret + "\r\n" + body;
}
String HTTPResponse::ToString(const String& padding) const {
    String ret = padding + "Status line:\n";
    ret += padding + "\tHTTP version: " + version + '\n';
    ret += padding + "\tStatus: " + status + '\n';
    ret += padding + "\tDescription: " + description + '\n';
    if (!headers.IsEmpty()) {
        ret += padding + "HTTP headers:\n";
        for (const HTTPHeader& header : headers) ret += header.ToString(padding + '\t') + '\n';
    }
    if (!body.IsEmpty()) ret += padding + "Body:\n" + body + '\n';
    return ret;
}