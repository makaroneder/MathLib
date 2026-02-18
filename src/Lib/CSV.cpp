#include "CSV.hpp"

namespace MathLib {
    CSV::CSV(void) : data() {}
    CSV::CSV(const Sequence<char>& str) : data() {
        const size_t size = str.GetSize();
        String tmp;
        Array<String> curr;
        size_t i = 0;
        while (i < size) {
            const char chr = str.AtUnsafe(i++);
            if (chr == ',') {
                if (!curr.Add(tmp)) Panic("Failed to allocate memory for CSV data");
                if (!tmp.Reset()) Panic("Failed to reset temporary CSV buffer");
            }
            else if (chr != '\n') tmp += chr;
            if (chr == '\n' || i == size) {
                if (!curr.Add(tmp)) Panic("Failed to allocate memory for CSV data");
                if (!tmp.Reset()) Panic("Failed to reset temporary CSV buffer");
                if (!data.Add(curr)) Panic("Failed to allocate memory for CSV data");
                if (!curr.Reset()) Panic("Failed to reset temporary CSV buffer");
            }
        }
    }
    String CSV::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const size_t height = data.GetSize();
        String ret = padd;
        for (size_t y = 0; y < height; y++) {
            const Array<String> tmp = data.AtUnsafe(y);
            const size_t width = tmp.GetSize();
            for (size_t x = 0; x < width; x++) {
                ret += tmp.AtUnsafe(x);
                if (x + 1 != width) ret += ',';
            }
            if (y + 1 != height) ret += '\n'_M + padd;
        }
        return ret;
    }
    size_t CSV::GetHeight(void) const {
        return data.GetSize();
    }
    size_t CSV::GetWidth(size_t y) const {
        return y < data.GetSize() ? data.AtUnsafe(y).GetSize() : 0;
    }
    size_t CSV::GetMaxWidth(void) const {
        const size_t height = data.GetSize();
        size_t ret = 0;
        for (size_t y = 0; y < height; y++) {
            const size_t tmp = data.AtUnsafe(y).GetSize();
            if (ret < tmp) ret = tmp;
        }
        return ret;
    }
    size_t CSV::GetKeyIndex(const Sequence<char>& key) const {
        if (data.IsEmpty()) return SIZE_MAX;
        const Array<String> baseFields = data.AtUnsafe(0);
        const size_t width = baseFields.GetSize();
        for (size_t i = 0; i < width; i++)
            if (baseFields.AtUnsafe(i) == key) return i;
        return SIZE_MAX;
    }
    CSV CSV::Reduce(const Sequence<String>& fields) const {
        const size_t size = fields.GetSize();
        if (!size) return CSV();
        const Array<String> baseFields = data.AtUnsafe(0);
        const size_t width = baseFields.GetSize();
        Array<size_t> index = size;
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < width; j++) {
                if (fields.AtUnsafe(i) != baseFields.AtUnsafe(j)) continue;
                index.AtUnsafe(i) = j;
                break;
            }
        }
        CSV ret;
        const size_t height = data.GetSize();
        for (size_t y = 0; y < height; y++) {
            Array<String> tmp;
            for (size_t x = 0; x < size; x++)
                if (!tmp.Add(At(index.AtUnsafe(x), y))) return CSV();
            if (!ret.data.Add(tmp)) return CSV();
        }
        return ret;
    }
    String CSV::At(size_t x, size_t y) const {
        return data.At(y).At(x);
    }
    String& CSV::At(size_t x, size_t y) {
        return data.At(y).At(x);
    }
}