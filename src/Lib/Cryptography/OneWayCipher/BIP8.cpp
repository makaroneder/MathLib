#include "BIP8.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> BIP8::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(bool)) return Array<uint8_t>();
        const bool ret = Bitmap(data).GetCountOf(true) % 2;
        return MakeArray<uint8_t>(key.data.AsT<bool>().Get() ? ret : !ret);
    }
}