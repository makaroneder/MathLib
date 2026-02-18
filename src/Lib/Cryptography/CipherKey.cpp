#include "CipherKey.hpp"

namespace MathLib {
    CipherKey::CipherKey(void) : data(), children(), type(Type::Normal) {}
    CipherKey::CipherKey(const Sequence<uint8_t>& data) : data(data), children(), type(Type::Normal) {}
    CipherKey::CipherKey(ByteArray& data) : data(data), children(), type(Type::Normal) {}
    CipherKey::CipherKey(const Sequence<CipherKey>& children) : data(), children(CollectionToArray<CipherKey>(children)), type(Type::MultiKey) {}
    bool CipherKey::IsEmpty(void) const {
        switch (type) {
            case Type::Normal: return data.IsEmpty();
            case Type::MultiKey: return children.IsEmpty();
            default: return false;
        }
    }
    bool CipherKey::Equals(const CipherKey& other) const {
        return type == other.type && data == other.data && children == other.children;
    }
}