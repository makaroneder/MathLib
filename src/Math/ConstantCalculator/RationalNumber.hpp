#ifndef RationalNumber_H
#define RationalNumber_H
#include "Integer.hpp"

struct RationalNumber;
struct RationalNumber : MathLib::Orderable<RationalNumber>, MathLib::Saveable, MathLib::Printable {
    RationalNumber(void);
    RationalNumber(Integer nat);
    RationalNumber(Integer a, NaturalNumber b);
    [[nodiscard]] static RationalNumber FromFloatingPointString(const MathLib::Sequence<char>& str);
    [[nodiscard]] size_t GetSize(void) const;
    [[nodiscard]] RationalNumber Invert(void) const;
    [[nodiscard]] RationalNumber Simplify(void) const;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    RationalNumber& operator+=(const RationalNumber& other);
    [[nodiscard]] RationalNumber operator+(const RationalNumber& other) const;
    [[nodiscard]] RationalNumber operator-(void) const;
    RationalNumber& operator-=(const RationalNumber& other);
    [[nodiscard]] RationalNumber operator-(const RationalNumber& other) const;
    RationalNumber& operator*=(const RationalNumber& other);
    [[nodiscard]] RationalNumber operator*(const RationalNumber& other) const;
    RationalNumber& operator/=(const RationalNumber& other);
    [[nodiscard]] RationalNumber operator/(const RationalNumber& other) const;
    RationalNumber& operator^=(const Integer& other);
    [[nodiscard]] RationalNumber operator^(const Integer& other) const;
    [[nodiscard]] virtual bool LessThanEqual(const RationalNumber& other) const override;

    private:
    Integer a;
    NaturalNumber b;
};

#endif