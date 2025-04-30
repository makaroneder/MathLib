#ifndef MathLib_Cryptography_Compressor_Deflate_H
#define MathLib_Cryptography_Compressor_Deflate_H
#include "../ReadableCipher.hpp"
#include "../../Bitmap.hpp"
#include "DeflateTree.hpp"

namespace MathLib {
    struct Deflate : ReadableCipher {
        Deflate(void);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;
        [[nodiscard]] virtual Array<uint8_t> DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const override;

        private:
        void GenerateTree(DeflateTree& tree, const uint8_t* lengths, uint16_t size) const;
        [[nodiscard]] uint16_t Decode(const DeflateTree& tree, Readable& readable, Bitmap& bitmap, size_t& i) const;
        [[nodiscard]] uint64_t Read(Readable& readable, Bitmap& bitmap, size_t& i, uint8_t bits) const;
        DeflateTree fixedDataTree;
        DeflateTree fixedDistTree;
    };
}

#endif