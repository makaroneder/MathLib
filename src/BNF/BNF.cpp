#include "BNF.hpp"
#include <String.hpp>

[[nodiscard]] MathLib::String LoadSimpleValue(const MathLib::Sequence<char>& str, size_t& i, char start, char end) {
    if (str.At(i++) != start) return "";
    const size_t next = str.Find(end, i);
    if (next == SIZE_MAX) return "";
    const MathLib::String name = MathLib::SubString(str, i, next - i);
    i = next + 1;
    return name;
}
[[nodiscard]] BNFValue LoadBNFValue(const MathLib::Sequence<char>& str, size_t& i) {
    BNFValue ret;
    MathLib::SkipWhiteSpace(str, i);
    if (str.At(i) == '"') ret = BNFValue(LoadSimpleValue(str, i, '"', '"'), true);
    else if (str.At(i) == '<') ret = BNFValue(LoadSimpleValue(str, i, '<', '>'), false);
    else if (str.At(i) == '(') {
        i++;
        bool array = true;
        while (true) {
            if (!ret.children.Add(LoadBNFValue(str, i))) return BNFValue();
            if (str.At(i) == ')') break;
            if (str.At(i) == '|') {
                i++;
                array = false;
            }
            else if (!array) return BNFValue();
        }
        i++;
        ret.type = array ? BNFValue::Type::Array : BNFValue::Type::Or;
    }
    MathLib::SkipWhiteSpace(str, i);
    return ret;
}
BNF::BNF(void) {}
BNF::BNF(const MathLib::Array<BNFVariable>& variables) : variables(variables) {}
bool BNF::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const {
    for (const BNFVariable& var : variables)
        if (var.name == variable) return var.Match(str, variables);
    return false;
}
MathLib::String BNF::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret;
    const size_t size = variables.GetSize();
    for (size_t i = 0; i < size; i++) {
        if (i) ret += '\n';
        ret += variables.At(i).ToString(padding);
    }
    return ret;
}
bool BNF::Save(MathLib::Writable& file) const {
    return file.Puts(ToString());
}
bool BNF::Load(MathLib::Readable& file) {
    variables = MathLib::Array<BNFVariable>();
    const MathLib::String str = file.ReadUntil('\0');
    const size_t size = str.GetSize();
    size_t i = 0;
    while (i < size) {
        MathLib::SkipWhiteSpace(str, i);
        const MathLib::String name = LoadSimpleValue(str, i, '<', '>');
        if (name.IsEmpty()) return false;
        MathLib::SkipWhiteSpace(str, i);
        for (uint8_t j = 0; j < 2; j++)
            if (str.At(i++) != ':') return false;
        if (str.At(i++) != '=') return false;
        if (!variables.Add(BNFVariable(name, LoadBNFValue(str, i)))) return false;
    }
    return true;
}