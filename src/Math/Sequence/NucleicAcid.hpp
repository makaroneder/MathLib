#ifndef NucleicAcid_H
#define NucleicAcid_H
#include <Interfaces/Printable.hpp>

struct NucleicAcid : MathLib::Printable {
    NucleicAcid(void);
    NucleicAcid(bool adenine, bool cytosine, bool guanine, bool thymine);
    bool operator==(const NucleicAcid& other) const;
    bool operator!=(const NucleicAcid& other) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    bool adenine : 1;
    bool cytosine : 1;
    bool guanine : 1;
    bool thymine : 1;
    uint8_t reserved : 4;
};
extern const NucleicAcid adenine;
extern const NucleicAcid cytosine;
extern const NucleicAcid guanine;
extern const NucleicAcid thymine;

#endif