#ifndef MathLib_BigInt_RationalNumber_H
#define MathLib_BigInt_RationalNumber_H
#include "Integer.hpp"

namespace MathLib {
    struct RationalNumber;
    struct RationalNumber : Orderable<RationalNumber>, Saveable, Printable {
        RationalNumber(void);
        RationalNumber(Integer nat);
        RationalNumber(Integer a, NaturalNumber b);
        [[nodiscard]] static RationalNumber FromFloatingPointString(const Sequence<char>& str);
        [[nodiscard]] size_t GetSize(void) const;
        [[nodiscard]] RationalNumber Invert(void) const;
        [[nodiscard]] RationalNumber Simplify(void) const;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
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
}

#endif