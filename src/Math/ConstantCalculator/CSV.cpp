#include "CSV.hpp"

CSV::CSV(void) : data() {}
CSV::CSV(const MathLib::Sequence<char>& str) : data() {
    const size_t size = str.GetSize();
    MathLib::String tmp = "";
    MathLib::Array<MathLib::String> curr;
    size_t i = 0;
    while (i < size) {
        const char chr = str.At(i++);
        if (chr == ',') {
            if (!curr.Add(tmp)) MathLib::Panic("Failed to allocate memory for CSV data");
            if (!tmp.Reset()) MathLib::Panic("Failed to reset temporary CSV buffer");
        }
        else if (chr != '\n') tmp += chr;
        if (chr == '\n' || i == size) {
            if (!curr.Add(tmp)) MathLib::Panic("Failed to allocate memory for CSV data");
            if (!tmp.Reset()) MathLib::Panic("Failed to reset temporary CSV buffer");
            if (!data.Add(curr)) MathLib::Panic("Failed to allocate memory for CSV data");
            if (!curr.Reset()) MathLib::Panic("Failed to reset temporary CSV buffer");
        }
    }
}
MathLib::String CSV::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const size_t height = data.GetSize();
    MathLib::String ret = padd;
    for (size_t y = 0; y < height; y++) {
        const size_t width = data.At(y).GetSize();
        for (size_t x = 0; x < width; x++) {
            ret += data.At(y).At(x);
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
    return y < data.GetSize() ? data.At(y).GetSize() : 0;
}
CSV CSV::Reduce(const MathLib::Sequence<MathLib::String>& fields) const {
    const size_t size = fields.GetSize();
    const size_t width = data.At(0).GetSize();
    MathLib::Array<size_t> index = size;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < width; j++) {
            if (fields.At(i) != data.At(0).At(j)) continue;
            index.At(i) = j;
            break;
        }
    }
    CSV ret;
    const size_t height = data.GetSize();
    for (size_t y = 0; y < height; y++) {
        MathLib::Array<MathLib::String> tmp;
        for (size_t x = 0; x < size; x++)
            if (!tmp.Add(At(index.At(x), y))) return CSV();
        if (!ret.data.Add(tmp)) return CSV();
    }
    return ret;
}
MathLib::String CSV::At(size_t x, size_t y) const {
    return data.At(y).At(x);
}
MathLib::String& CSV::At(size_t x, size_t y) {
    return data.At(y).At(x);
}
MathLib::String CSV::At(const MathLib::Sequence<char>& key, size_t y) const {
    size_t x = SIZE_MAX;
    for (size_t i = 0; i < data.At(0).GetSize(); i++) {
        if (data.AtUnsafe(0).AtUnsafe(i) != key) continue;
        x = i;
        break;
    }
    if (x == SIZE_MAX) MathLib::Panic("Invalid CSV key");
    return At(x, y);
}
MathLib::String& CSV::At(const MathLib::Sequence<char>& key, size_t y) {
    size_t x = SIZE_MAX;
    for (size_t i = 0; i < data.At(0).GetSize(); i++) {
        if (data.AtUnsafe(0).AtUnsafe(i) != key) continue;
        x = i;
        break;
    }
    if (x == SIZE_MAX) MathLib::Panic("Invalid CSV key");
    return At(x, y);
}