#ifndef RationalNumber_H
#define RationalNumber_H
#include "NaturalNumber.hpp"

struct RationalNumber : MathLib::Orderable, MathLib::Saveable, MathLib::Printable {
    RationalNumber(void);
    RationalNumber(NaturalNumber nat);
    RationalNumber(NaturalNumber a, NaturalNumber b, bool positive);
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
    RationalNumber& operator^=(const NaturalNumber& other);
    [[nodiscard]] RationalNumber operator^(const NaturalNumber& other) const;

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;

    private:
    NaturalNumber a;
    NaturalNumber b;
    bool positive;
};

#endif