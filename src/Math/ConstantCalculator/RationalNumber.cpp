#include "RationalNumber.hpp"

NaturalNumber Add(const NaturalNumber& a, const NaturalNumber& b, bool& positive1, bool positive2) {
    if (positive1 == positive2) return a + b;
    if (a > b) return (a - b).Get();
    positive1 = !positive1;
    return (b - a).Get();
}
RationalNumber::RationalNumber(void) : a(), b(NaturalNumber::FromT<uint8_t>(1)) {}
RationalNumber::RationalNumber(Integer nat) : a(nat), b(NaturalNumber::FromT<uint8_t>(1)) {}
RationalNumber::RationalNumber(Integer a, NaturalNumber b) : a(a), b(b) {}
RationalNumber RationalNumber::FromFloatingPointString(const MathLib::Sequence<char>& str) {
    const size_t size = str.GetSize();
    MathLib::String parsed;
    NaturalNumber div;
    bool start = false;
    bool positive = true;
    const char chr = str.At(start);
    if (chr == '-' || chr == '+') {
        start = true;
        positive = chr == '+';
    }
    for (size_t i = start; i < size; i++) {
        const char chr = str.At(i);
        if (chr == '.') {
            if (!div.IsZero()) return RationalNumber();
            div = NaturalNumber::FromT<uint8_t>(1);
            continue;
        }
        if (!MathLib::IsDigit(chr)) return RationalNumber();
        parsed += chr;
        div *= NaturalNumber::FromT<uint8_t>(10);
    }
    return RationalNumber(Integer(NaturalNumber(parsed), positive), div.IsZero() ? NaturalNumber::FromT<uint8_t>(1) : div).Simplify();
}
size_t RationalNumber::GetSize(void) const {
    return a.GetSize() + b.GetSize();
}
RationalNumber RationalNumber::Invert(void) const {
    return RationalNumber(Integer(b, a.positive), a.natural);
}
RationalNumber RationalNumber::Simplify(void) const {
    if (a.IsZero()) return RationalNumber();
    const uint8_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
        89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
        181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
    };
    RationalNumber ret = *this;
    for (uint8_t i = 0; i < SizeOfArray(primes); i++) {
        uint8_t mod = 0;
        while (true) {
            const Integer tmp1 = ret.a.DivMod(primes[i], mod);
            if (mod) break;
            const NaturalNumber tmp2 = ret.b.DivMod(primes[i], mod);
            if (mod) break;
            ret.a = tmp1;
            ret.b = tmp2;
        }
    }
    return ret;
}
bool RationalNumber::Save(MathLib::Writable& file) const {
    return a.Save(file) && b.Save(file);
}
bool RationalNumber::Load(MathLib::Readable& file) {
    return a.Load(file) && b.Load(file);
}
MathLib::String RationalNumber::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    if (b == NaturalNumber::FromT<uint8_t>(1)) return padd + a.ToString();
    return padd + a.ToString() + '/' + b.ToString();
}
RationalNumber& RationalNumber::operator+=(const RationalNumber& other) {
    return *this = (*this + other).Simplify();
}
RationalNumber RationalNumber::operator+(const RationalNumber& other) const {
    return RationalNumber(a * other.b + other.a * b, b * other.b).Simplify();
}
RationalNumber RationalNumber::operator-(void) const {
    return RationalNumber(-a, b);
}
RationalNumber& RationalNumber::operator-=(const RationalNumber& other) {
    return *this += -other;
}
RationalNumber RationalNumber::operator-(const RationalNumber& other) const {
    return *this + -other;
}
RationalNumber& RationalNumber::operator*=(const RationalNumber& other) {
    a *= other.a;
    b *= other.b;
    *this = Simplify();
    return *this;
}
RationalNumber RationalNumber::operator*(const RationalNumber& other) const {
    RationalNumber tmp = *this;
    tmp *= other;
    return tmp;
}
RationalNumber& RationalNumber::operator/=(const RationalNumber& other) {
    return *this *= other.Invert();
}
RationalNumber RationalNumber::operator/(const RationalNumber& other) const {
    return *this * other.Invert();
}
RationalNumber& RationalNumber::operator^=(const NaturalNumber& other) {
    a ^= other;
    b ^= other;
    return *this = Simplify();
}
RationalNumber RationalNumber::operator^(const NaturalNumber& other) const {
    RationalNumber tmp = *this;
    tmp ^= other;
    return tmp;
}
bool RationalNumber::LessThanEqual(const MathLib::Orderable& other_) const {
    const RationalNumber& other = (const RationalNumber&)other_;
    return a * other.b <= other.a * b;
}