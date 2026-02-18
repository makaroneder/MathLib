#ifndef MathLib_Cryptography_Compressor_BytePairEncoding_Grammar_H
#define MathLib_Cryptography_Compressor_BytePairEncoding_Grammar_H
#include "BytePairEncodingSubstitute.hpp"

namespace MathLib {
    struct BytePairEncodingGrammar : Saveable, Printable {
        Array<BytePairEncodingSubstitute> substitutes;
        Array<size_t> string;

        BytePairEncodingGrammar(void);
        BytePairEncodingGrammar(const Sequence<uint8_t>& data);
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] Array<size_t> GetContinuations(size_t x) const;
    };
}

#endif