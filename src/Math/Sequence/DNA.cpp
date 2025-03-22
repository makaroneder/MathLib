#include "DNA.hpp"

DNA::DNA(const MathLib::Sequence<NucleicAcid>& data) : data(MathLib::CollectionToArray<NucleicAcid>(data)) {}
size_t DNA::GetSize(void) const {
    return data.GetSize();
}
NucleicAcid DNA::At(size_t index) const {
    return data.At(index);
}
MathLib::String DNA::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding);
    const size_t size = GetSize();
    for (size_t i = 0; i < size; i++) ret += At(i).ToString();
    return ret;
}