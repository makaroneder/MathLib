#include "HTTPRequest.hpp"
#include <String.hpp>

HTTPRequest::HTTPRequest(MathLib::String str) {
    if (!str.IsEmpty()) {
        MathLib::Array<MathLib::String> lines = MathLib::Split(str, "\r\n", true);
        MathLib::Array<MathLib::String> requestLine = MathLib::Split(lines.At(0), " ", true);
        method = MathLib::SubString(requestLine.At(0), 0, requestLine.At(0).GetSize() - 1);
        target = MathLib::SubString(requestLine.At(1), 0, requestLine.At(1).GetSize() - 1);
        version = MathLib::SubString(requestLine.At(2), 0, requestLine.At(2).GetSize() - 1);
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
            MathLib::Array<MathLib::String> tmp = MathLib::Split(line, ":", true);
            size_t off = 0;
            while (tmp.At(1).At(off) == ' ') off++;
            if (!headers.Add(HTTPHeader(MathLib::SubString(tmp.At(0), 0, tmp.At(0).GetSize() - 1), MathLib::SubString(tmp.At(1), off, tmp.At(1).GetSize() - off)))) MathLib::Panic("Failed to add HTTP header");
        }
        for (size_t i = bodyIndex; i < size; i++) body += lines.At(i) + "\n";
    }
}
MathLib::String HTTPRequest::GetRaw(void) const {
    MathLib::String ret = method + " " + target + " " + version + "\r\n";
    for (const HTTPHeader& header : headers) ret += header.name + ": " + header.value + "\r\n";
    return ret + "\r\n" + body;
}
MathLib::String HTTPRequest::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "Request line:\n";
    ret += padding + "\tMethod: " + method + '\n';
    ret += padding + "\tTarget: " + target + '\n';
    ret += padding + "\tHTTP version: " + version + '\n';
    if (!headers.IsEmpty()) {
        ret += padding + "HTTP headers:\n";
        for (const HTTPHeader& header : headers) ret += header.ToString(padding + '\t') + '\n';
    }
    if (!body.IsEmpty()) ret += padding + "Body:\n" + body + '\n';
    return ret;
}