#ifndef DNA_H
#define DNA_H
#include "NucleicAcid.hpp"
#include <Host.hpp>

struct DNA : MathLib::Sequence<NucleicAcid>, MathLib::Printable {
    DNA(const MathLib::Sequence<NucleicAcid>& data);
    template <typename T>
    T GetDifference(const DNA& other) const {
        const size_t size = GetSize();
        if (size != other.GetSize()) return MathLib::nan;
        T ret = T();
        for (size_t i = 0; i < size; i++) ret += (At(i) != other.At(i));
        return ret / size;
    }
    [[nodiscard]] virtual size_t GetSize(void) const override;
    [[nodiscard]] virtual NucleicAcid At(size_t index) const override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const;

    private:
    MathLib::Array<NucleicAcid> data;
};

#endif