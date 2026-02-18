#ifndef MathLib_Cryptography_Compressor_BytePairEncoding_Substitute_H
#define MathLib_Cryptography_Compressor_BytePairEncoding_Substitute_H
#include "../../../Interfaces/Saveable.hpp"

namespace MathLib {
    struct BytePairEncodingSubstitute : Saveable, Printable {
        size_t name;
        size_t a;
        size_t b;

        BytePairEncodingSubstitute(void);
        BytePairEncodingSubstitute(size_t name, size_t a, size_t b);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const BytePairEncodingSubstitute& other) const;
        [[nodiscard]] bool operator!=(const BytePairEncodingSubstitute& other) const;
    };
}

#endif