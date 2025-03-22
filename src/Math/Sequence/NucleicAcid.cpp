#include "NucleicAcid.hpp"

const NucleicAcid adenine = NucleicAcid(true, false, false, false);
const NucleicAcid cytosine = NucleicAcid(false, true, false, false);
const NucleicAcid guanine = NucleicAcid(false, false, true, false);
const NucleicAcid thymine = NucleicAcid(false, false, false, true);

NucleicAcid::NucleicAcid(void) {}
NucleicAcid::NucleicAcid(bool adenine, bool cytosine, bool guanine, bool thymine) : adenine(adenine), cytosine(cytosine), guanine(guanine), thymine(thymine) {}
bool NucleicAcid::operator==(const NucleicAcid& other) const {
    return adenine == other.adenine && cytosine == other.cytosine && guanine == other.guanine && thymine == other.thymine;
}
bool NucleicAcid::operator!=(const NucleicAcid& other) const {
    return !(*this == other);
}
MathLib::String NucleicAcid::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret;
    if (adenine) ret += 'A';
    if (cytosine) ret += ret.IsEmpty() ? "C" : "/C";
    if (guanine) ret += ret.IsEmpty() ? "G" : "/G";
    if (thymine) ret += ret.IsEmpty() ? "T" : "/T";
    return MathLib::CollectionToString(padding) + (ret.IsEmpty() ? "0" : ret);
}