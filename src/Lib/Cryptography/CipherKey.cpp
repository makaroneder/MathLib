#include "CipherKey.hpp"

namespace MathLib {
    CipherKey::CipherKey(void) : data(), children(), type(Type::Normal) {}
    CipherKey::CipherKey(const Sequence<uint8_t>& data) : data(data), children(), type(Type::Normal) {}
    CipherKey::CipherKey(ByteArray& data) : data(data), children(), type(Type::Normal) {}
    CipherKey::CipherKey(const Sequence<CipherKey>& children) : data(), children(CollectionToArray<CipherKey>(children)), type(Type::MultiKey) {}
    String CipherKey::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        MathLib::String ret = "{\n";
        switch (type) {
            case Type::Normal: {
                ret += padd2 + DumpMemory<uint8_t>(data, 16);
                break;
            }
            case Type::MultiKey: {
                for (const CipherKey& child : children) ret += padd2 + child.ToString(padd2) + '\n';
                break;
            }
            default: return "";
        }
        return ret + padd + '}';
    }
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