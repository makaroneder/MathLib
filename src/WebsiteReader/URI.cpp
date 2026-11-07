#include "URI.hpp"
#include <Utils.hpp>

URI::URI(void) : scheme(), userInfo(), host(), path(), query(), fragment(), port(SIZE_MAX) {}
URI::URI(const MathLib::Sequence<char>& str) : scheme(), userInfo(), host(), path(), query(), fragment(), port(SIZE_MAX) {
    const size_t size = str.GetSize();
    size_t i = 0;
    while (true) {
        if (i == size) MathLib::Panic("Invalid URI");
        const char chr = str.AtUnsafe(i++);
        if (chr == ':') break;
        scheme += chr;
    }
    if (i == size) return;
    if (i + 1 < size && str.AtUnsafe(i) == '/' && str.AtUnsafe(i + 1) == '/') {
        i += 2;
        const size_t userInfoEnd = str.Find('@', i);
        if (userInfoEnd != SIZE_MAX) {
            while (i < userInfoEnd) userInfo += str.AtUnsafe(i++);
            i++;
        }
        while (i < size) {
            const char chr = str.AtUnsafe(i);
            if (chr == ':' || chr == '/' || chr == '?' || chr == '#') break;
            host += chr;
            i++;
        }
        if (i < size && str.AtUnsafe(i) == ':') {
            i++;
            MathLib::String portStr;
            while (MathLib::IsDigit(str.AtUnsafe(i))) portStr += str.AtUnsafe(i++);
            port = MathLib::StringToNumber(portStr, 10);
        }
    }
    while (i < size) {
        const char chr = str.AtUnsafe(i);
        if (chr == '?' || chr == '#') break;
        path += chr;
        i++;
    }
    if (i < size && str.AtUnsafe(i) == '?') {
        i++;
        while (i < size) {
            const char chr = str.AtUnsafe(i);
            if (chr == '#') break;
            query += chr;
            i++;
        }
    }
    if (i < size && str.AtUnsafe(i) == '#') {
        i++;
        while (i < size) fragment += str.AtUnsafe(i++);
    }
}
MathLib::String URI::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + scheme + ':';
    if (host.GetSize()) {
        ret += "//";
        if (userInfo.GetSize()) ret += userInfo + '@';
        ret += host;
        if (port != SIZE_MAX) {
            ret += ':';
            ret += MathLib::ToString(port, 10);
        }
    }
    ret += path;
    if (query.GetSize()) {
        ret += '?';
        ret += query;
    }
    if (fragment.GetSize()) {
        ret += '#';
        ret += fragment;
    }
    return ret;
}