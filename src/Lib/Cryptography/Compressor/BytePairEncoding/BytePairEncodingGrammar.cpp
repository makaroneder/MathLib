#include "BytePairEncodingGrammar.hpp"
#include "../../../String.hpp"

namespace MathLib {
    BytePairEncodingGrammar::BytePairEncodingGrammar(void) : substitutes(), string() {}
    BytePairEncodingGrammar::BytePairEncodingGrammar(const Sequence<uint8_t>& data) : substitutes(), string(data.GetSize()) {
        const size_t size = string.GetSize();
        for (size_t i = 0; i < size; i++) string.AtUnsafe(i) = data.AtUnsafe(i);
    }
    bool BytePairEncodingGrammar::Save(Writable& file) const {
        if (!file.Write<size_t>(substitutes.GetSize())) return false;
        for (const BytePairEncodingSubstitute& substitute : substitutes)
            if (!substitute.Save(file)) return false;
        if (!file.Write<size_t>(string.GetSize())) return false;
        for (const size_t& x : string)
            if (!file.Write<size_t>(x)) return false;
        return true;
    }
    bool BytePairEncodingGrammar::Load(Readable& file) {
        size_t size;
        if (!file.Read<size_t>(size)) return false;
        substitutes = Array<BytePairEncodingSubstitute>(size);
        for (BytePairEncodingSubstitute& substitute : substitutes)
            if (!substitute.Load(file)) return false;
        if (!file.Read<size_t>(size)) return false;
        string = Array<size_t>(size);
        for (size_t& x : string)
            if (!file.Read<size_t>(x)) return false;
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
    Array<size_t> BytePairEncodingGrammar::GetContinuations(size_t x) const {
        Array<size_t> ret;
        for (const BytePairEncodingSubstitute& substitute : substitutes)
            if (substitute.a == x && !ret.Add(substitute.b)) return Array<size_t>();
        if (x <= UINT8_MAX) {
            const size_t size = string.GetSize();
            for (size_t i = 1; i < size; i++)
                if (string.AtUnsafe(i - 1) == x && !ret.Add(string.AtUnsafe(i))) return Array<size_t>();
        }
        return ret;
    }
}