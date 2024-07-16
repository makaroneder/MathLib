#include "HTTPRequest.hpp"
#include <String.hpp>

HTTPRequest::HTTPRequest(String str) {
    if (!str.IsEmpty()) {
        Array<String> lines = Split(str, "\r\n");
        Array<String> requestLine = Split(lines.At(0), " ");
        method = SubString(requestLine.At(0), 0, requestLine.At(0).GetSize() - 1);
        target = SubString(requestLine.At(1), 0, requestLine.At(1).GetSize() - 1);
        version = SubString(requestLine.At(2), 0, requestLine.At(2).GetSize() - 1);
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
            Array<String> tmp = Split(line, ":");
            size_t off = 0;
            while (tmp.At(1).At(off) == ' ') off++;
            headers.Add(HTTPHeader(SubString(tmp.At(0), 0, tmp.At(0).GetSize() - 1), SubString(tmp.At(1), off, tmp.At(1).GetSize() - off)));
        }
        for (size_t i = bodyIndex; i < size; i++) body += lines.At(i) + "\n";
    }
}
String HTTPRequest::Raw(void) const {
    String ret = method + " " + target + " " + version + "\r\n";
    for (const HTTPHeader& header : headers) ret += header.name + ": " + header.value + "\r\n";
    return ret + "\r\n" + body;
}
String HTTPRequest::ToString(const String& padding) const {
    String ret = padding + "Request line:\n";
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