#include "Integer.hpp"

Integer::Integer(void) : natural(), positive(true) {}
Integer::Integer(const NaturalNumber& natural) : natural(natural), positive(true) {}
Integer::Integer(const NaturalNumber& natural, bool positive) : natural(natural), positive(positive) {}
bool Integer::IsZero(void) const {
    return natural.IsZero();
}
bool Integer::IsEven(void) const {
    return natural.IsEven();
}
size_t Integer::GetSize(void) const {
    return natural.GetSize();
}
MathLib::String Integer::ToHexString(void) const {
    const MathLib::String tmp = natural.ToHexString();
    return positive ? tmp : '-'_M + tmp;
}
bool Integer::Save(MathLib::Writable& file) const {
    return natural.Save(file) && file.Write<bool>(positive);
}
bool Integer::Load(MathLib::Readable& file) {
    return natural.Load(file) && file.Read<bool>(positive);
}
MathLib::String Integer::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding);
    if (!positive) ret += '-';
    return ret + natural.ToString();
}
Integer Integer::DivMod(const uint8_t& other, uint8_t& mod) const {
    return Integer(natural.DivMod(other, mod), positive);
}
Integer Integer::DivModBySubtraction(const Integer& other, NaturalNumber& mod) const {
    return Integer(natural.DivModBySubtraction(other.natural, mod), positive == other.positive);
}
Integer Integer::LongDivMod(const Integer& other, NaturalNumber& mod) const {
    return Integer(natural.LongDivMod(other.natural, mod), positive == other.positive);
}
Integer Integer::GreatestCommonDivisor(const Integer& other) const {
    return natural.GreatestCommonDivisor(other.natural);
}
Integer Integer::LeastCommonMultiple(const Integer& other) const {
    return natural.LeastCommonMultiple(other.natural);
}
void Integer::MultiplyBy256(uint8_t last) {
    natural.MultiplyBy256(last);
}
Integer Integer::operator+(const Integer& other) const {
    if (positive == other.positive) return Integer(natural + other.natural, positive);
    if (natural > other.natural) return Integer((natural - other.natural).Get(), positive);
    return Integer((other.natural - natural).Get(), !positive);
}
Integer& Integer::operator+=(const Integer& other) {
    return *this = *this + other;
}
Integer& Integer::operator++(void) {
    if (natural.IsZero()) positive = true;
    if (positive) {
        ++natural;
        return *this;
    }
    --natural;
    return *this;
}
Integer Integer::operator++(int) {
    Integer ret = *this;
    ++*this;
    return ret;
}
Integer Integer::operator-(void) const {
    return Integer(natural, !positive);
}
Integer& Integer::operator-=(const Integer& other) {
    return *this += -other;
}
Integer Integer::operator-(const Integer& other) const {
    return *this + -other;
}
Integer& Integer::operator--(void) {
    if (natural.IsZero()) positive = false;
    if (positive) {
        --natural;
        return *this;
    }
    ++natural;
    return *this;
}
Integer Integer::operator--(int) {
    Integer ret = *this;
    --*this;
    return ret;
}
Integer& Integer::operator*=(const Integer& other) {
    natural *= other.natural;
    positive = positive == other.positive;
    return *this;
}
Integer Integer::operator*(const Integer& other) const {
    return Integer(natural * other.natural, positive == other.positive);
}
Integer& Integer::operator/=(const Integer& other) {
    NaturalNumber mod;
    return *this = LongDivMod(other, mod);
}
Integer Integer::operator/(const Integer& other) const {
    NaturalNumber mod;
    return LongDivMod(other, mod);
}
Integer& Integer::operator%=(const Integer& other) {
    NaturalNumber mod;
    (void)natural.LongDivMod(other.natural, mod);
    return *this = mod;
}
Integer Integer::operator%(const Integer& other) const {
    NaturalNumber mod;
    (void)natural.LongDivMod(other.natural, mod);
    return mod;
}
Integer& Integer::operator^=(const NaturalNumber& other) {
    natural ^= other;
    positive |= other.IsEven();
    return *this;
}
Integer Integer::operator^(const NaturalNumber& other) const {
    return Integer(natural ^ other, positive || other.IsEven());
}
bool Integer::LessThanEqual(const MathLib::Orderable& other_) const {
    const Integer& other = (const Integer&)other_;
    if ((IsZero() && other.IsZero()) || (!positive && other.positive)) return true;
    if (positive && other.positive) return natural <= other.natural;
    if (!positive && !other.positive) return other.natural <= natural;
    return false;
}