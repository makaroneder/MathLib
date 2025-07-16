#include "Path.hpp"

namespace MathLib {
    String RemoveLastPathElement(const Sequence<char>& path) {
        const size_t size = path.GetSize();
        String ret;
        String tmp;
        bool add = false;
        for (size_t i = 0; i < size; i++) {
            const char chr = path.At(i);
            if (chr == '/' && i + 1 != size) {
                if (add) ret += '/';
                add = true;
                ret += tmp;
                if (!tmp.Reset()) return "";
            }
            else tmp += chr;
        }
        return ret;
    }
    String AppendPath(const Sequence<char>& path1, const Sequence<char>& path2) {
        const size_t size = path2.GetSize();
        String ret = CollectionToString(path1);
        String tmp;
        bool add = !ret.IsEmpty() && ret.At(ret.GetSize() - 1) != '/';
        for (size_t i = 0; i < size; i++) {
            const char chr = path2.At(i);
            if (chr == '/' || i + 1 == size) {
                if (chr != '/') tmp += chr;
                if (tmp == "..") ret = RemoveLastPathElement(ret);
                else {
                    if (add) ret += '/';
                    ret += tmp;
                }
                add = true;
                if (!tmp.Reset()) return "";
            }
            else tmp += chr;
        }
        return ret;
    }
}