#include "RSA.hpp"
#include "RSAPKCS1v1v5.hpp"

namespace MathLib {
    Array<uint8_t> RSAPKCS1v1v5::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        const size_t k = key.children.At(0).data.GetSize();
        const size_t size = data.GetSize();
        if (k < size + 11) return Array<uint8_t>();
        Array<uint8_t> message = k;
        message.AtUnsafe(0) = 0;
        message.AtUnsafe(1) = 2;
        for (size_t i = 2; i < k - size - 1; i++) {
            message.AtUnsafe(i) = RandomNumber<uint8_t>(1, UINT8_MAX);
        }
        message.AtUnsafe(k - size - 1) = 0;
        for (size_t i = 0; i < size; i++) message.AtUnsafe(i + k - size) = data.AtUnsafe(i);
        MathLib::Array<uint8_t> ret = RSA().Encrypt(message, key);
        if (ret.GetSize() > k) return Array<uint8_t>();
        while (ret.GetSize() < k)
            if (!ret.Add(0)) return Array<uint8_t>();
        return ret;
    }
    Array<uint8_t> RSAPKCS1v1v5::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.CheckChildrenSize(2)) return Array<uint8_t>();
        const size_t k = key.children.At(0).data.GetSize();
        if (k < 11) return Array<uint8_t>();
        const size_t size = data.GetSize();
        if (size != k) return Array<uint8_t>();
        MathLib::Array<uint8_t> message = RSA().Decrypt(data, key);
        if (message.GetSize() > k) return Array<uint8_t>();
        while (message.GetSize() < k)
            if (!message.Add(0)) return Array<uint8_t>();
        if (message.AtUnsafe(0) || message.AtUnsafe(1) != 2) return Array<uint8_t>();
        const size_t retStart = message.Find(0, 2);
        if (retStart == SIZE_MAX || retStart < 10) return Array<uint8_t>();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(k - retStart + 1, range.GetMax());
        return start < end ? Array<uint8_t>(message.GetValue() + start + retStart + 1, end - start) : Array<uint8_t>();
    }
}