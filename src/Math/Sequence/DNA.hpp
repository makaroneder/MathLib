#ifndef DNA_H
#define DNA_H
#include "ArraySequence.hpp"
#include "NucleicAcid.hpp"
#include <Host.hpp>

struct DNA : ArraySequence<NucleicAcid>, MathLib::Printable {
    DNA(const MathLib::Array<NucleicAcid>& data);
    template <typename T>
    T GetDifference(const DNA& other) const {
        const size_t size = GetSize();
        if (size != other.GetSize()) return MathLib::nan;
        T ret = T();
        for (size_t i = 0; i < size; i++) ret += (At(i) != other.At(i));
        return ret / size;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const;
};

#endif