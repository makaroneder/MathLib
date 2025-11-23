#include "BytePairEncodingGrammar.hpp"
#include "../../../String.hpp"

namespace MathLib {
    BytePairEncodingGrammar::BytePairEncodingGrammar(void) : substitutes(), string() {}
    BytePairEncodingGrammar::BytePairEncodingGrammar(const Sequence<uint8_t>& data) : substitutes(), string(data.GetSize()) {
        const size_t size = string.GetSize();
        for (size_t i = 0; i < size; i++) string.At(i) = data.At(i);
    }
    bool BytePairEncodingGrammar::Save(Writable& file) const {
        size_t size = substitutes.GetSize();
        if (!file.Write<size_t>(size)) return false;
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Save(file)) return false;
        size = string.GetSize();
        if (!file.Write<size_t>(size)) return false;
        for (size_t i = 0; i < size; i++)
            if (!file.Write<size_t>(string.At(i))) return false;
        return true;
    }
    bool BytePairEncodingGrammar::Load(Readable& file) {
        size_t size;
        if (!file.Read<size_t>(size)) return false;
        substitutes = Array<BytePairEncodingSubstitute>(size);
        for (size_t i = 0; i < size; i++)
            if (!substitutes.At(i).Load(file)) return false;
        if (!file.Read<size_t>(size)) return false;
        string = Array<size_t>(size);
        for (size_t i = 0; i < size; i++)
            if (!file.Read<size_t>(string.At(i))) return false;
        return true;
    }
    String BytePairEncodingGrammar::ToString(const Sequence<char>& padding) const {
        String ret = "{\n";
        const String padd = '\t'_M + padding;
        for (const BytePairEncodingSubstitute& substitute : substitutes) ret += substitute.ToString(padd) + '\n';
        ret += padd + "String = (";
        const size_t size = string.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (i) ret += ", ";
            ret += Formatter<size_t>::ToString(string.At(i));
        }
        return ret + ")\n" + padding + '}';
    }
}