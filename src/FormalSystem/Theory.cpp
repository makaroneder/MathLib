#include "Theory.hpp"

[[nodiscard]] Formula Substitute(const Formula& curr, const MathLib::Sequence<char>& name, const Formula& arg) {
    if (curr.type == Formula::Type::Variable && curr.value == name) return arg;
    Formula ret = curr;
    for (Formula& formula : ret.children) formula = Substitute(formula, name, arg);
    return ret;
}
Theory::Theory(void) : name(), variables(), data() {}
Theory::Theory(const MathLib::Sequence<char>& name, const MathLib::Sequence<Formula>& data) : name(MathLib::CollectionToString(name)), variables(), data(MathLib::CollectionToArray<Formula>(data)) {}
Theory::Theory(const MathLib::Sequence<char>& name, const MathLib::Sequence<MathLib::String>& variables, const MathLib::Sequence<Formula>& data) : name(MathLib::CollectionToString(name)), variables(MathLib::CollectionToArray<MathLib::String>(variables)), data(MathLib::CollectionToArray<Formula>(data)) {}
size_t Theory::GetFreeVariables(const Theory& args) const {
    return variables.GetSize() - args.data.GetSize();
}
Theory Theory::Substitute(const Theory& args) const {
    MathLib::Array<MathLib::String> vars;
    for (const MathLib::String& var : args.variables)
        if (!vars.Add(var)) return Theory();
    const size_t size = args.data.GetSize();
    for (size_t i = size; i < variables.GetSize(); i++)
        if (!vars.Add(variables.At(i))) return Theory();
    Theory ret = Theory(name, vars, data);
    for (size_t i = 0; i < size; i++) {
        const MathLib::String name = variables.At(i);
        const Formula arg = args.data.At(i);
        for (Formula& formula : ret.data) formula = ::Substitute(formula, name, arg);
    }
    return ret;
}
MathLib::String Theory::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = "{\n"_M + padding + "\tName: " + name + '\n' + padding + "\tVariables: ";
    const MathLib::String padd = "\t\t"_M + padding;
    if (!variables.IsEmpty()) {
        ret += "{\n";
        for (const MathLib::String& variable : variables) ret += padd + variable + '\n';
        ret += MathLib::CollectionToString(padding) + "\t}";
    }
    else ret += "None";
    ret += '\n'_M + padding + "\tData: {\n";
    for (const Formula& formula : data)
        ret += formula.ToString(padd) + '\n';
    return ret + padding + "\t}\n" + padding + '}';
}
bool Theory::operator==(const Theory& other) const {
    return name == other.name && variables == other.variables && data == other.data;
}
bool Theory::operator!=(const Theory& other) const {
    return !(*this == other);
}