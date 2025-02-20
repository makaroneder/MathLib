#include "DNA.hpp"

DNA::DNA(const MathLib::Array<NucleicAcid>& data) : ArraySequence(data) {}
MathLib::String DNA::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding;
    const size_t size = GetSize();
    for (size_t i = 0; i < size; i++) ret += At(i).ToString();
    return ret;
}