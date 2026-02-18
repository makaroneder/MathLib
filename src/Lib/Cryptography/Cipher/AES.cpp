#include "AES.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    void AES::ShiftRows(Matrix<ByteFiniteField>& state) {
        const Matrix<ByteFiniteField> tmp = state;
        for (uint8_t y = 0; y < 4; y++)
            for (uint8_t x = 0; x < 4; x++)
                state.AtUnsafe(x, y) = tmp.AtUnsafe((x + y) % 4, y);
    }
    void AES::InverseShiftRows(Matrix<ByteFiniteField>& state) {
        const Matrix<ByteFiniteField> tmp = state;
        for (uint8_t y = 0; y < 4; y++)
            for (uint8_t x = 0; x < 4; x++)
                state.AtUnsafe(x, y) = tmp.AtUnsafe((x < y ? 4 - y + x : x - y) % 4, y);
    }
    Matrix<ByteFiniteField> AES::KeyToMatrix(const Sequence<uint8_t>& data, size_t& i, bool add) {
        Matrix<ByteFiniteField> ret = Matrix<ByteFiniteField>(4, 4);
        if (!add) i -= 16;
        for (uint8_t x = 0; x < 4; x++)
            for (uint8_t y = 0; y < 4; y++)
                ret.AtUnsafe(x, y) = data.AtUnsafe(i++);
        if (!add) i -= 16;
        return ret;
    }
    CipherKey AES::GenerateKey(const CipherKey& key) {
        if (key.type != CipherKey::Type::MultiKey) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(Rounds)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal) return Array<uint8_t>();
        const size_t keySize = key2.data.GetSize();
        if (!keySize || keySize % 4) return Array<uint8_t>();
        const size_t nk = keySize / 4;
        const Rounds nr = key1.data.AsT<Rounds>().Get();
        CipherKey ret;
        for (size_t i = 0; i < nk; i++)
            for (uint8_t j = 0; j < sizeof(uint32_t); j++)
                if (!ret.data.Add(key2.data.AtUnsafe(4 * i + j))) return Array<uint8_t>();
        for (size_t i = nk; i < 4 * (size_t)nr + 4; i++) {
            uint32_t tmp = ret.data.AsT<uint32_t>((i - 1) * 4).Get();
            if (!(i % nk)) {
                uint8_t* buff1 = (uint8_t*)&tmp;
                uint8_t buff2[4];
                buff2[0] = buff1[1];
                buff2[1] = buff1[2];
                buff2[2] = buff1[3];
                buff2[3] = buff1[0];
                for (uint8_t j = 0; j < sizeof(uint32_t); j++) buff2[j] = sbox[buff2[j]];
                switch (i / nk) {
                    case 1: {
                        buff2[0] ^= 0x01;
                        break;
                    }
                    case 2: {
                        buff2[0] ^= 0x02;
                        break;
                    }
                    case 3: {
                        buff2[0] ^= 0x04;
                        break;
                    }
                    case 4: {
                        buff2[0] ^= 0x08;
                        break;
                    }
                    case 5: {
                        buff2[0] ^= 0x10;
                        break;
                    }
                    case 6: {
                        buff2[0] ^= 0x20;
                        break;
                    }
                    case 7: {
                        buff2[0] ^= 0x40;
                        break;
                    }
                    case 8: {
                        buff2[0] ^= 0x80;
                        break;
                    }
                    case 9: {
                        buff2[0] ^= 0x1b;
                        break;
                    }
                    case 10: {
                        buff2[0] ^= 0x36;
                        break;
                    }
                    default: return CipherKey();
                }
                tmp = *(const uint32_t*)buff2;
            }
            else if (nk > 6 && i % nk == 4) {
                uint8_t* buff = (uint8_t*)&tmp;
                for (uint8_t j = 0; j < sizeof(uint32_t); j++) buff[j] = sbox[buff[j]];
            }
            const uint32_t prev = ret.data.AsT<uint32_t>((i - nk) * 4).Get() ^ tmp;
            const uint8_t* buff = (const uint8_t*)&prev;
            for (uint8_t j = 0; j < sizeof(uint32_t); j++)
                if (!ret.data.Add(buff[j])) return Array<uint8_t>();
        }
        return ret;
    }
    Array<uint8_t> AES::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || data.GetSize() != 16) return Array<uint8_t>();
        const size_t keySize = key.data.GetSize();
        if (keySize % 16 || keySize < 16 * 2) return Array<uint8_t>();
        const Matrix<ByteFiniteField> mul = Matrix<ByteFiniteField>(4, 4, MakeArray<ByteFiniteField>(
            0x02, 0x03, 0x01, 0x01,
            0x01, 0x02, 0x03, 0x01,
            0x01, 0x01, 0x02, 0x03,
            0x03, 0x01, 0x01, 0x02
        ));
        size_t i = 0;
        Matrix<ByteFiniteField> state = KeyToMatrix(data, i, true);
        i = 0;
        state += KeyToMatrix(key.data, i, true);
        while (i != keySize - 16) {
            for (ByteFiniteField& x : state) x.value = sbox[x.value];
            ShiftRows(state);
            for (uint8_t x = 0; x < 4; x++) {
                Matrix<ByteFiniteField> tmp = Matrix<ByteFiniteField>(1, 4);
                for (uint8_t y = 0; y < 4; y++) tmp.AtUnsafe(0, y) = state.AtUnsafe(x, y);
                tmp = (mul * tmp).Get("Failed to multiply matrices");
                for (uint8_t y = 0; y < 4; y++) state.AtUnsafe(x, y) = tmp.AtUnsafe(0, y);
            }
            state += KeyToMatrix(key.data, i, true);
        }
        for (ByteFiniteField& x : state) x.value = sbox[x.value];
        ShiftRows(state);
        state += KeyToMatrix(key.data, i, true);
        i = 0;
        Array<uint8_t> ret = 16;
        for (uint8_t x = 0; x < 4; x++)
            for (uint8_t y = 0; y < 4; y++) ret.AtUnsafe(i++) = state.AtUnsafe(x, y).value;
        return ret;
    }
    Array<uint8_t> AES::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::Normal || data.GetSize() != 16) return Array<uint8_t>();
        const size_t keySize = key.data.GetSize();
        if (keySize % 16 || keySize < 16 * 2) return Array<uint8_t>();
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max || min >= 16) return Array<uint8_t>();
        const Matrix<ByteFiniteField> mul = Matrix<ByteFiniteField>(4, 4, MakeArray<ByteFiniteField>(
            0x0e, 0x0b, 0x0d, 0x09,
            0x09, 0x0e, 0x0b, 0x0d,
            0x0d, 0x09, 0x0e, 0x0b,
            0x0b, 0x0d, 0x09, 0x0e
        ));
        size_t i = 0;
        Matrix<ByteFiniteField> state = KeyToMatrix(data, i, true);
        i = keySize;
        state += KeyToMatrix(key.data, i, false);
        while (i != 16) {
            InverseShiftRows(state);
            for (ByteFiniteField& x : state) x.value = rsbox[x.value];
            state += KeyToMatrix(key.data, i, false);
            for (uint8_t x = 0; x < 4; x++) {
                Matrix<ByteFiniteField> tmp = Matrix<ByteFiniteField>(1, 4);
                for (uint8_t y = 0; y < 4; y++) tmp.AtUnsafe(0, y) = state.AtUnsafe(x, y);
                tmp = (mul * tmp).Get("Failed to multiply matrices");
                for (uint8_t y = 0; y < 4; y++) state.AtUnsafe(x, y) = tmp.AtUnsafe(0, y);
            }
        }
        InverseShiftRows(state);
        for (ByteFiniteField& x : state) x.value = rsbox[x.value];
        state += KeyToMatrix(key.data, i, false);
        i = 0;
        Array<uint8_t> ret = 16;
        for (uint8_t x = 0; x < 4; x++)
            for (uint8_t y = 0; y < 4; y++) ret.AtUnsafe(i++) = state.AtUnsafe(x, y).value;
        return CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, 16)));
    }
}