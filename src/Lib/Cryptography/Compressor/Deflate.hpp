#ifndef MathLib_Cryptography_Compressor_Deflate_H
#define MathLib_Cryptography_Compressor_Deflate_H
#include "../../Bitmap.hpp"
#include "DeflateTree.hpp"
#include "../Cipher.hpp"

namespace MathLib {
    struct Deflate : Cipher {
        Deflate(void);
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const override;

        private:
        void GenerateTree(DeflateTree& tree, const uint8_t* lengths, uint16_t size) const;
        [[nodiscard]] uint16_t Decode(const DeflateTree& tree, const Bitmap& data, size_t& i) const;
        DeflateTree fixedDataTree;
        DeflateTree fixedDistTree;
    };
}

#endif