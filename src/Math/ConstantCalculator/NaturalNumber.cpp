#include "NaturalNumber.hpp"

NaturalNumber::NaturalNumber(void) : data() {}
NaturalNumber::NaturalNumber(size_t size) : data(size) {}
NaturalNumber::NaturalNumber(const MathLib::Sequence<uint8_t>& data) : data(MathLib::CollectionToArray<uint8_t>(data)) {}
NaturalNumber::NaturalNumber(const MathLib::Sequence<char>& str) : data() {
    const size_t size = str.GetSize();
    for (size_t i = 0; i < size; i++) {
        const char chr = str.At(i);
        if (!MathLib::IsDigit(chr)) MathLib::Panic("Non digit character detected");
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
size_t NaturalNumber::GetSize(void) const {
    return data.GetSize();
}
bool NaturalNumber::Save(MathLib::Writable& file) const {
    const size_t size = data.GetSize();
    return file.Write<size_t>(size) && file.WriteBuffer(data.GetValue(), size);
}
bool NaturalNumber::Load(MathLib::Readable& file) {
    size_t size;
    if (!file.Read<size_t>(size)) return false;
    data = MathLib::Array<uint8_t>(size);
    return file.ReadBuffer(data.GetValue(), size);
}
MathLib::String NaturalNumber::ToHexString(void) const {
    const size_t size = data.GetSize();
    MathLib::String ret = "0x";
    if (IsZero()) return ret + "0";
    size_t start = size;
    while (!data.At(start - 1)) start--;
    ret += MathLib::ToString(data.At(start - 1), 16);
    for (size_t i = start - 1; i; i--) ret += MathLib::ToString(data.At(i - 1), 16, 2);
    return ret;
}
MathLib::String NaturalNumber::ToString(const MathLib::Sequence<char>& padding) const {
    NaturalNumber self = *this;
    MathLib::String ret;
    do {
        uint8_t mod = 0;
        self = self.DivMod(10, mod);
        ret += mod + '0';
    } while (!self.IsZero());
    return ret.Reverse(0, ret.GetSize()) ? MathLib::CollectionToString(padding) + ret : "";
}
NaturalNumber NaturalNumber::Add(const MathLib::Sequence<uint8_t>& other, uint8_t carry) const {
    const size_t otherSize = other.GetSize();
    const size_t size = MathLib::Max<size_t>(data.GetSize(), otherSize);
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
    MathLib::Array<uint8_t> div;
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
NaturalNumber NaturalNumber::DivModBySubtraction(const NaturalNumber& other, NaturalNumber& mod) const {
    if (other.IsZero()) return NaturalNumber();
    mod = *this;
    NaturalNumber ret = NaturalNumber();
    while (true) {
        const MathLib::Expected<NaturalNumber> tmp = mod - other;
        if (!tmp.HasValue()) return ret;
        mod = tmp.Get();
        ++ret;
    }
}
NaturalNumber NaturalNumber::LongDivMod(const NaturalNumber& other, NaturalNumber& mod) const {
    if (other.IsZero()) return NaturalNumber();
    if (*this < other) {
        mod = *this;
        return NaturalNumber();
    }
    size_t i = data.GetSize();
    while (!data.At(i - 1)) i--;
    if (i == 1) {
        mod = NaturalNumber::FromT<uint8_t>(data.At(--i) % other.data.At(0));
        return NaturalNumber::FromT<uint8_t>(data.At(i) / other.data.At(0));
    }
    NaturalNumber curr;
    while (curr < other) curr.MultiplyBy256(data.At(--i));
    if (!i) return DivModBySubtraction(other, mod);
    NaturalNumber ret;
    while (true) {
        const NaturalNumber tmp = curr.LongDivMod(other, mod);
        ret.MultiplyBy256(0);
        ret += tmp;
        curr = mod;
        if (!i) return ret;
        curr.MultiplyBy256(data.At(--i));
    }
}
NaturalNumber NaturalNumber::Factorial(const NaturalNumber& level) const {
    if (level.IsZero()) return NaturalNumber();
    NaturalNumber ret = FromT<uint8_t>(1);
    NaturalNumber tmp = *this;
    while (!tmp.IsZero()) {
        ret *= tmp;
        const MathLib::Expected<NaturalNumber> x = tmp - level;
        if (!x.HasValue()) break;
        tmp = x.Get();
    }
    return ret;
}
NaturalNumber NaturalNumber::GreatestCommonDivisor(const NaturalNumber& other) const {
    if (IsZero()) return other;
    if (other.IsZero()) return *this;
    NaturalNumber mod;
    (void)LongDivMod(other, mod);
    return other.GreatestCommonDivisor(mod);
}
NaturalNumber NaturalNumber::LeastCommonMultiple(const NaturalNumber& other) const {
    NaturalNumber mod;
    return (*this * other).LongDivMod(GreatestCommonDivisor(other), mod);
}
void NaturalNumber::MultiplyBy256(uint8_t last) {
    const size_t size = data.GetSize();
    MathLib::Array<uint8_t> arr = MathLib::Array<uint8_t>(size + 1);
    arr.At(0) = last;
    MathLib::MemoryCopy(data.GetValue(), arr.GetValue() + 1, size);
    data = arr;
}
NaturalNumber& NaturalNumber::operator+=(const MathLib::Sequence<uint8_t>& other) {
    return *this = *this + other;
}
NaturalNumber NaturalNumber::operator+(const MathLib::Sequence<uint8_t>& other) const {
    return Add(other, 0);
}
NaturalNumber& NaturalNumber::operator+=(const NaturalNumber& other) {
    return *this = *this + other.data;
}
NaturalNumber NaturalNumber::operator+(const NaturalNumber& other) const {
    return Add(other.data, 0);
}
NaturalNumber& NaturalNumber::operator++(void) {
    return *this = Add(MathLib::EmptySequence<uint8_t>(), 1);
}
NaturalNumber NaturalNumber::operator++(int) {
    NaturalNumber ret = *this;
    *this = Add(MathLib::EmptySequence<uint8_t>(), 1);
    return ret;
}
MathLib::Expected<NaturalNumber> NaturalNumber::operator-(const NaturalNumber& other) const {
    if (*this < other) return MathLib::Expected<NaturalNumber>();
    NaturalNumber ret = *this;
    for (size_t i = 0; i < other.data.GetSize(); i++) {
        uint8_t sub = other.data.At(i);
        for (size_t j = i; j < data.GetSize(); j++) {
            if (sub <= data.At(j)) {
                ret.data.At(j) -= sub;
                break;
            }
            else {
                ret.data.At(j) += UINT8_MAX - sub + 1;
                sub = 1;
            }
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
    if (other.IsZero()) return NaturalNumber();
    uint8_t mod = 0;
    const NaturalNumber n = *this * other.DivMod(2, mod);
    if (mod) return n + n + *this;
    return n + n;
}
NaturalNumber& NaturalNumber::operator/=(const NaturalNumber& other) {
    NaturalNumber mod;
    return *this = LongDivMod(other, mod);
}
NaturalNumber NaturalNumber::operator/(const NaturalNumber& other) const {
    NaturalNumber mod;
    return LongDivMod(other, mod);
}
NaturalNumber& NaturalNumber::operator%=(const NaturalNumber& other) {
    NaturalNumber mod;
    (void)LongDivMod(other, mod);
    return *this = mod;
}
NaturalNumber NaturalNumber::operator%(const NaturalNumber& other) const {
    NaturalNumber mod;
    (void)LongDivMod(other, mod);
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
bool NaturalNumber::LessThanEqual(const MathLib::Orderable& other_) const {
    const NaturalNumber& other = (const NaturalNumber&)other_;
    const size_t size = MathLib::Max<size_t>(data.GetSize(), other.data.GetSize());
    for (size_t i = size; i; i--) {
        if (At(i - 1) < other.At(i - 1)) return true;
        if (At(i - 1) > other.At(i - 1)) return false;
    }
    return true;
}
uint8_t NaturalNumber::At(size_t i) const {
    return i < data.GetSize() ? data.At(i) : 0;
}