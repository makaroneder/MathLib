#include "BNF.hpp"
#include <String.hpp>

[[nodiscard]] MathLib::String LoadSimpleValue(const MathLib::Sequence<char>& str, size_t& i, char start, char end) {
    if (str.At(i++) != start) return "";
    const size_t next = str.Find(end, i);
    if (next == SIZE_MAX) return "";
    const MathLib::String ret = MathLib::SubString(str, i, next - i);
    i = next + 1;
    return ret;
}
[[nodiscard]] BNFValue LoadBNFValue(const MathLib::Sequence<char>& str, size_t& i) {
    BNFValue ret;
    MathLib::SkipWhiteSpace(str, i);
    if (str.At(i) == '"' || str.At(i) == '\'') {
        const MathLib::String tmp = LoadSimpleValue(str, i, str.At(i), str.At(i));
        const size_t size = tmp.GetSize();
        MathLib::String value;
        for (size_t i = 0; i < size; i++) {
            if (tmp.At(i) == '\\') {
                switch (tmp.At(++i)) {
                    case '\\': {
                        value += '\\';
                        break;
                    }
                    case 't': {
                        value += '\t';
                        break;
                    }
                    case 'n': {
                        value += '\n';
                        break;
                    }
                    case 'r': {
                        value += '\r';
                        break;
                    }
                    default: return BNFValue();
                }
            }
            else value += tmp.At(i);
        }
        ret = BNFValue(value, true);
    }
    else if (str.At(i) == '<') ret = BNFValue(LoadSimpleValue(str, i, '<', '>'), false);
    else if (str.At(i) == '(') {
        i++;
        bool array = true;
        MathLib::Array<BNFValue> values;
        while (true) {
            if (!values.Add(LoadBNFValue(str, i))) return BNFValue();
            if (str.At(i) == ')') break;
            if (str.At(i) == '|') {
                i++;
                array = false;
            }
            else if (!array) return BNFValue();
        }
        i++;
        ret = BNFValue(values, array);
    }
    else if (str.At(i) == '[') {
        i++;
        ret = BNFValue(BNFValue::Type::Optional, LoadBNFValue(str, i));
        if (str.At(i++) != ']') return BNFValue();
    }
    MathLib::SkipWhiteSpace(str, i);
    if (i + 3 < str.GetSize() && str.At(i) == '.' && str.At(i + 1) == '.' && str.At(i + 2) == '.') {
        if (!ret.IsConstant()) return BNFValue();
        const MathLib::String value = ret.GetValue();
        if (value.GetSize() != 1) return BNFValue();
        i += 3;
        MathLib::SkipWhiteSpace(str, i);
        ret = BNFValue(MathLib::Interval<char>(value.At(0), str.At(i++)));
    }
    if (i < str.GetSize() && (str.At(i) == '*' || str.At(i) == '+')) {
        ret = BNFValue(str.At(i++) == '*' ? BNFValue::Type::KleeneStar : BNFValue::Type::KleenePlus, ret);
        MathLib::SkipWhiteSpace(str, i);
    }
    return ret;
}
BNF::BNF(void) {}
BNF::BNF(const MathLib::Array<BNFVariable>& variables) : variables(variables) {}
bool BNF::Add(const BNFVariable& variable) {
    return variables.Add(variable);
}
bool BNF::Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const {
    for (const BNFVariable& var : variables)
        if (var.CheckName(variable)) return var.Match(str, variables);
    return false;
}
MathLib::Tree<MathLib::String> BNF::BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const {
    for (const BNFVariable& var : variables)
        if (var.CheckName(variable)) return var.BuildAST(str, variables);
    return MathLib::Tree<MathLib::String>();
}
size_t BNF::GetDependenceCount(const MathLib::Sequence<char>& variable, bool checkVariables) const {
    size_t ret = 0;
    const size_t size = variables.GetSize();
    for (size_t i = 0; i < size; i++) ret += variables.At(i).GetDependenceCount(variable, checkVariables ? variables : MathLib::Array<BNFVariable>());
    return ret;
}
BNF BNF::Simplify(const MathLib::Sequence<char>& variable) const {
    BNF ret;
    const size_t size = variables.GetSize();
    bool done = true;
    for (size_t i = 0; i < size; i++) {
        const BNFVariable var = variables.At(i);
        if (var.CheckName(variable) || GetDependenceCount(var.GetName(), false) > 1) continue;
        done = false;
        for (size_t j = 0; j < size; j++)
            if (i != j && !ret.Add(variables.At(j).Substitute(var).Simplify())) return BNF();
        break;
    }
    return done ? *this : ret.Simplify(variable);
}
BNFValue BNF::Substitute(const MathLib::Sequence<char>& variable) const {
    const size_t size = variables.GetSize();
    for (size_t i = 0; i < size; i++) {
        BNFVariable var = variables.At(i);
        if (!var.CheckName(variable)) continue;
        return var.Substitute(variables).GetValue().Simplify();
    }
    return BNFValue();
}
size_t BNF::GetDataSize(const MathLib::Sequence<char>& variable, size_t variableSize) const {
    for (const BNFVariable& var : variables)
        if (var.CheckName(variable)) return var.GetDataSize(variableSize);
    return 0;
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