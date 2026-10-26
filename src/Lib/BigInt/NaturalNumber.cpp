#include "NaturalNumber.hpp"
#include "../Interfaces/Sequence/SubSequence.hpp"
#include "../Interfaces/Sequence/ExtendedSequence.hpp"

namespace MathLib {
    Array<SingleTypePair<NaturalNumber>> GetNextCoprimePair(const SingleTypePair<NaturalNumber>& pair) {
        return MakeArray<SingleTypePair<NaturalNumber>>(
            SingleTypePair<NaturalNumber>((pair.first + pair.first - pair.second).Get(), pair.first),
            SingleTypePair<NaturalNumber>(pair.first + pair.first + pair.second, pair.first),
            SingleTypePair<NaturalNumber>(pair.first + pair.second + pair.second, pair.second)
        );
    }
    [[nodiscard]] size_t GetDegree(const Sequence<uint8_t>& data) {
        const size_t size = data.GetSize();
        for (size_t i = size; i; i--)
            if (data.AtUnsafe(i - 1)) return i - 1;
        return 0;
    }
    [[nodiscard]] NaturalNumber MultSequence(const Sequence<uint8_t>& a, const Sequence<uint8_t>& b) {
        const size_t sizeA = a.GetSize();
        const size_t sizeB = b.GetSize();
        if (!sizeA || !sizeB) return NaturalNumber();
        size_t n = Max<size_t>(GetDegree(a), GetDegree(b)) + 1;
        if (n == 1) {
            const uint16_t tmp = a.AtUnsafe(0) * b.AtUnsafe(0);
            return tmp <= UINT8_MAX ? NaturalNumber::FromT<uint8_t>(tmp) : NaturalNumber::FromT<uint16_t>(tmp);
        }
        n += n % 2;
        const size_t m = n / 2;
        const ExtendedSequence<uint8_t> seqA = ExtendedSequence<uint8_t>(a, 0, n - sizeA);
        const ExtendedSequence<uint8_t> seqB = ExtendedSequence<uint8_t>(b, 0, n - sizeB);
        const SubSequence<uint8_t> x0 = SubSequence<uint8_t>(seqA, 0, m);
        const SubSequence<uint8_t> x1 = SubSequence<uint8_t>(seqA, m, n);
        const SubSequence<uint8_t> y0 = SubSequence<uint8_t>(seqB, 0, m);
        const SubSequence<uint8_t> y1 = SubSequence<uint8_t>(seqB, m, n);
        const NaturalNumber z3 = (NaturalNumber(x1) + x0).Mul(NaturalNumber(y1) + y0);
        NaturalNumber z2 = MultSequence(x1, y1);
        const NaturalNumber z0 = MultSequence(x0, y0);
        NaturalNumber z1 = ((z3 - z2).Get() - z0).Get();
        z1.MultiplyBy256ToThePowerOf(m);
        z2.MultiplyBy256ToThePowerOf(m * 2);
        return z2 + z1 + z0;
    }
    NaturalNumber::NaturalNumber(void) : data() {}
    NaturalNumber::NaturalNumber(size_t size) : data(size) {}
    NaturalNumber::NaturalNumber(const Sequence<uint8_t>& data) : data(CollectionToArray<uint8_t>(data)) {}
    NaturalNumber::NaturalNumber(const Sequence<char>& str) : data() {
        const size_t size = str.GetSize();
        for (size_t i = 0; i < size; i++) {
            const char chr = str.At(i);
            if (!IsDigit(chr)) Panic("Non digit character detected");
            *this *= NaturalNumber::FromT<uint8_t>(10);
            *this += NaturalNumber::FromT<uint8_t>(chr - '0');
        }
    }
    bool NaturalNumber::IsZero(void) const {
        for (const uint8_t& x : data)
            if (x) return false;
        return true;
    }
    bool NaturalNumber::IsEven(void) const {
        return data.IsEmpty() || !(data.At(0) & 1);
    }
    size_t NaturalNumber::GetDegree(void) const {
        return MathLib::GetDegree(data);
    }
    size_t NaturalNumber::GetSize(void) const {
        return data.GetSize();
    }
    bool NaturalNumber::GetBit(size_t n) const {
        return At(n / 8) & (1 << (n % 8));
    }
    bool NaturalNumber::Save(Writable& file) const {
        const size_t size = data.GetSize();
        return file.Write<size_t>(size) && file.WriteBuffer(data.GetValue(), size);
    }
    bool NaturalNumber::Load(Readable& file) {
        size_t size;
        if (!file.Read<size_t>(size)) return false;
        data = Array<uint8_t>(size);
        return file.ReadBuffer(data.GetValue(), size);
    }
    String NaturalNumber::ToHexString(void) const {
        const size_t size = data.GetSize();
        if (IsZero()) return "0x00";
        size_t start = size;
        while (!data.AtUnsafe(start - 1)) start--;
        String ret = "0x";
        for (size_t i = start; i; i--) ret += MathLib::ToString(data.AtUnsafe(i - 1), 16, 2);
        return ret;
    }
    String NaturalNumber::ToString(const Sequence<char>& padding) const {
        NaturalNumber self = *this;
        String ret;
        do {
            uint8_t mod = 0;
            self = self.DivMod(10, mod);
            ret += mod + '0';
        } while (!self.IsZero());
        return ret.Reverse(0, ret.GetSize()) ? CollectionToString(padding) + ret : "";
    }
    NaturalNumber NaturalNumber::RandomLessThanEqual(void) const {
        bool restrictedSize = true;
        const size_t size = GetSize();
        Array<uint8_t> ret = size;
        size_t lastZero = SIZE_MAX;
        for (size_t i = 0; i < size; i++) {
            ret.AtUnsafe(i) = RandomNumber<uint8_t>(0, restrictedSize ? data.AtUnsafe(i) : UINT8_MAX);
            restrictedSize &= ret.AtUnsafe(i) == data.AtUnsafe(i);
            if (ret.AtUnsafe(i)) lastZero = SIZE_MAX;
            else if (lastZero == SIZE_MAX) lastZero = i;
        }
        return lastZero == SIZE_MAX ? NaturalNumber(ret) : NaturalNumber(SubSequence<uint8_t>(ret, 0, lastZero));
    }
    NaturalNumber NaturalNumber::Add(const Sequence<uint8_t>& other, uint8_t carry) const {
        const size_t otherSize = other.GetSize();
        const size_t size = Max<size_t>(data.GetSize(), otherSize);
        NaturalNumber ret = size;
        for (size_t i = 0; i < size; i++) {
            const uint16_t tmp = At(i) + (i < otherSize ? other.At(i) : 0) + carry;
            carry = tmp / (UINT8_MAX + 1);
            ret.data.At(i) = tmp % (UINT8_MAX + 1);
        }
        if (carry && !ret.data.Add(carry)) return NaturalNumber();
        return ret;
    }
    NaturalNumber NaturalNumber::DivMod(const uint8_t& other, uint8_t& mod) const {
        if (!other) return NaturalNumber();
        Array<uint8_t> div;
        const size_t size = data.GetSize();
        size_t i = size;
        while (i) {
            uint16_t a = data.At(--i) + (UINT8_MAX + 1) * mod;
            const bool over = a < other && i;
            if (over) a = a * (UINT8_MAX + 1) + data.At(--i);
            const uint16_t tmp = a / other;
            if ((over && !div.Add(tmp >> 16)) || !div.Add(tmp)) return NaturalNumber();
            mod = a % other;
        }
        return div.Reverse(0, div.GetSize()) ? div : NaturalNumber();
    }
    NaturalNumber NaturalNumber::DivMod(const NaturalNumber& other, NaturalNumber& mod) const {
        const size_t degree = other.GetDegree();
        if (!degree) {
            uint8_t mod8 = 0;
            const NaturalNumber ret = DivMod(other.data.AtUnsafe(0), mod8);
            mod = FromT<uint8_t>(mod8);
            return ret;
        }
        return LongDivMod(other, mod);
    }
    NaturalNumber NaturalNumber::Mul(const NaturalNumber& other) const {
        return MultSequence(SubSequence<uint8_t>(data, 0, GetDegree() + 1), SubSequence<uint8_t>(other.data, 0, other.GetDegree() + 1));
    }
    NaturalNumber NaturalNumber::DivModBySubtraction(const NaturalNumber& other, NaturalNumber& mod) const {
        if (other.IsZero()) return NaturalNumber();
        if (LessThanEqual(other)) {
            if (other.LessThanEqual(*this)) {
                mod = NaturalNumber();
                return NaturalNumber::FromT<uint8_t>(1);
            }
            mod = *this;
            return NaturalNumber();
        }
        mod = *this;
        NaturalNumber ret = NaturalNumber();
        while (true) {
            const Expected<NaturalNumber> tmp = mod - other;
            if (!tmp.HasValue()) return ret;
            mod = tmp.Get();
            ++ret;
        }
    }
    NaturalNumber NaturalNumber::LongDivMod(const NaturalNumber& other, NaturalNumber& mod) const {
        if (other.IsZero()) return NaturalNumber();
        if (LessThanEqual(other)) {
            if (other.LessThanEqual(*this)) {
                mod = NaturalNumber();
                return NaturalNumber::FromT<uint8_t>(1);
            }
            mod = *this;
            return NaturalNumber();
        }
        NaturalNumber ret;
        mod = NaturalNumber();
        const size_t degree = GetDegree() + 1;
        for (size_t i = degree; i; i--) {
            mod.MultiplyBy256(data.AtUnsafe(i - 1));
            NaturalNumber tmp;
            const NaturalNumber div = mod.DivModBySubtraction(other, tmp);
            mod = tmp;
            if (div.GetDegree()) return NaturalNumber();
            ret.MultiplyBy256(div.IsZero() ? 0 : div.data.AtUnsafe(0));
        }
        return ret;
    }
    NaturalNumber NaturalNumber::Factorial(const NaturalNumber& level) const {
        if (level.IsZero()) return NaturalNumber();
        NaturalNumber ret = FromT<uint8_t>(1);
        NaturalNumber tmp = *this;
        while (!tmp.IsZero()) {
            ret *= tmp;
            const Expected<NaturalNumber> x = tmp - level;
            if (!x.HasValue()) break;
            tmp = x.Get();
        }
        return ret;
    }
    NaturalNumber NaturalNumber::GreatestCommonDivisor(const NaturalNumber& other) const {
        if (IsZero()) return other;
        if (other.IsZero()) return *this;
        return other.GreatestCommonDivisor(*this % other);
    }
    NaturalNumber NaturalNumber::LeastCommonMultiple(const NaturalNumber& other) const {
        return (*this * other) / GreatestCommonDivisor(other);
    }
    NaturalNumber NaturalNumber::GenerateCoprime(void) const {
        if (*this == NaturalNumber::FromT<uint8_t>(2) || *this == NaturalNumber::FromT<uint8_t>(3)) return NaturalNumber::FromT<uint8_t>(1);
        Array<SingleTypePair<NaturalNumber>> current = MakeArray<SingleTypePair<NaturalNumber>>(SingleTypePair<NaturalNumber>(NaturalNumber::FromT<uint8_t>(2), NaturalNumber::FromT<uint8_t>(1)));
        if (!IsEven() && !current.Add(SingleTypePair<NaturalNumber>(NaturalNumber::FromT<uint8_t>(3), NaturalNumber::FromT<uint8_t>(1)))) return NaturalNumber();
        Array<SingleTypePair<NaturalNumber>> next;
        while (true) {
            for (const SingleTypePair<NaturalNumber>& x : current) {
                const Array<SingleTypePair<NaturalNumber>> tmp = GetNextCoprimePair(x);
                for (const SingleTypePair<NaturalNumber>& y : tmp) {
                    if (!y.first.LessThanEqual(*this)) continue;
                    if (LessThanEqual(y.first)) return y.second;
                    if (!next.Add(y)) return NaturalNumber();
                }
            }
            current = next;
            if (!next.Reset()) return NaturalNumber();
        }
    }
    NaturalNumber NaturalNumber::GetModularInverse(const NaturalNumber& mod) const {
        if (mod <= NaturalNumber::FromT<uint8_t>(1)) return NaturalNumber();
        NaturalNumber ret;
        NaturalNumber curr;
        while (true) {
            if (curr % mod == NaturalNumber::FromT<uint8_t>(1)) return ret;
            ++ret;
            curr += *this;
        }
    }
    NaturalNumber NaturalNumber::GetModularInverseByFermatsLittleTheorem(const NaturalNumber& mod) const {
        const Expected<NaturalNumber> exp = mod - NaturalNumber::FromT<uint8_t>(2);
        return exp.HasValue() ? ModularExponentiation(exp.Get(), mod) : NaturalNumber();
    }
    NaturalNumber NaturalNumber::ModularExponentiation(const NaturalNumber& exp, const NaturalNumber& mod) const {
        if (IsZero()) return exp.IsZero() ? NaturalNumber::FromT<uint8_t>(1) : NaturalNumber();
        if (exp.IsZero()) return NaturalNumber::FromT<uint8_t>(1);
        NaturalNumber ret = NaturalNumber::FromT<uint8_t>(1);
        NaturalNumber base = *this % mod;
        const size_t degree = exp.GetDegree();
        size_t bits = (degree + 1) * 8;
        const uint8_t tmp = exp.data.AtUnsafe(degree);
        for (uint8_t i = 8; i; i--) {
            if (tmp & (1 << (i - 1))) break;
            bits--;
        }
        for (size_t i = 0; i < bits; i++) {
            if (i) base = (base * base) % mod;
            if (exp.GetBit(i)) ret = (ret * base) % mod;
        }
        return Array<uint8_t>(ret.data.GetValue(), ret.GetDegree() + 1);
    }
    void NaturalNumber::MultiplyBy256(uint8_t last) {
        const size_t size = data.GetSize();
        Array<uint8_t> arr = Array<uint8_t>(size + 1);
        arr.AtUnsafe(0) = last;
        MemoryCopy(data.GetValue(), arr.GetValue() + 1, size);
        data = arr;
    }
    void NaturalNumber::MultiplyBy256ToThePowerOf(size_t n) {
        const size_t size = data.GetSize();
        Array<uint8_t> arr = Array<uint8_t>(size + n);
        MemorySet8(arr.GetValue(), n, 0);
        MemoryCopy(data.GetValue(), arr.GetValue() + n, size);
        data = arr;
    }
    NaturalNumber& NaturalNumber::operator+=(const Sequence<uint8_t>& other) {
        return *this = *this + other;
    }
    NaturalNumber NaturalNumber::operator+(const Sequence<uint8_t>& other) const {
        return Add(other, 0);
    }
    NaturalNumber& NaturalNumber::operator+=(const NaturalNumber& other) {
        return *this = *this + other.data;
    }
    NaturalNumber NaturalNumber::operator+(const NaturalNumber& other) const {
        return Add(other.data, 0);
    }
    NaturalNumber& NaturalNumber::operator++(void) {
        return *this = Add(EmptySequence<uint8_t>(), 1);
    }
    NaturalNumber NaturalNumber::operator++(int) {
        NaturalNumber ret = *this;
        *this = Add(EmptySequence<uint8_t>(), 1);
        return ret;
    }
    Expected<NaturalNumber> NaturalNumber::operator-(const NaturalNumber& other) const {
        if (*this < other) return Expected<NaturalNumber>();
        NaturalNumber ret = *this;
        for (size_t i = 0; i < other.data.GetSize(); i++) {
            uint8_t sub = other.data.At(i);
            for (size_t j = i; j < data.GetSize(); j++) {
                if (sub <= ret.data.At(j)) {
                    ret.data.At(j) -= sub;
                    break;
                }
                ret.data.At(j) += UINT8_MAX - sub + 1;
                sub = 1;
            }
        }
        return ret;
    }
    NaturalNumber& NaturalNumber::operator-=(const NaturalNumber& other) {
        return *this = (*this - other).Get("Failed to subtract 2 natural numbers");
    }
    NaturalNumber& NaturalNumber::operator--(void) {
        return *this -= FromT<uint8_t>(1);
    }
    NaturalNumber NaturalNumber::operator--(int) {
        NaturalNumber ret = *this;
        *this -= FromT<uint8_t>(1);
        return ret;
    }
    NaturalNumber& NaturalNumber::operator*=(const NaturalNumber& other) {
        return *this = *this * other;
    }
    NaturalNumber NaturalNumber::operator*(const NaturalNumber& other) const {
        return Mul(other);
    }
    NaturalNumber& NaturalNumber::operator/=(const NaturalNumber& other) {
        NaturalNumber mod;
        return *this = DivMod(other, mod);
    }
    NaturalNumber NaturalNumber::operator/(const NaturalNumber& other) const {
        NaturalNumber mod;
        return DivMod(other, mod);
    }
    NaturalNumber& NaturalNumber::operator%=(const NaturalNumber& other) {
        NaturalNumber mod;
        (void)DivMod(other, mod);
        return *this = mod;
    }
    NaturalNumber NaturalNumber::operator%(const NaturalNumber& other) const {
        NaturalNumber mod;
        (void)DivMod(other, mod);
        return mod;
    }
    NaturalNumber& NaturalNumber::operator^=(const NaturalNumber& other) {
        return *this = *this ^ other;
    }
    NaturalNumber NaturalNumber::operator^(const NaturalNumber& other) const {
        if (other.IsZero()) return FromT<uint8_t>(1);
        uint8_t mod = 0;
        const NaturalNumber n = *this ^ other.DivMod(2, mod);
        if (mod) return n * n * *this;
        return n * n;
    }
    bool NaturalNumber::LessThanEqual(const NaturalNumber& other) const {
        const size_t size = Max<size_t>(data.GetSize(), other.data.GetSize());
        for (size_t i = size; i; i--) {
            if (At(i - 1) < other.At(i - 1)) return true;
            if (At(i - 1) > other.At(i - 1)) return false;
        }
        return true;
    }
    uint8_t NaturalNumber::At(size_t i) const {
        return i < data.GetSize() ? data.AtUnsafe(i) : 0;
    }
}