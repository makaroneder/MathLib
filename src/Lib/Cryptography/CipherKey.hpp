#ifndef MathLib_Cryptography_CipherKey_H
#define MathLib_Cryptography_CipherKey_H
#include "../Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    struct CipherKey;
    struct CipherKey : Comparable<CipherKey>, Printable {
        enum class Type : uint8_t {
            Normal,
            MultiKey,
        };
        ByteArray data;
        Array<CipherKey> children;
        Type type;

        CipherKey(void);
        CipherKey(const Sequence<uint8_t>& data);
        CipherKey(ByteArray& data);
        CipherKey(const Sequence<CipherKey>& children);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool IsEmpty(void) const;

        protected:
        [[nodiscard]] virtual bool Equals(const CipherKey& other) const override;
    };
}

#endif