#ifndef Integer_H
#define Integer_H
#include "NaturalNumber.hpp"

struct Integer : MathLib::Orderable, MathLib::Saveable, MathLib::Printable {
    Integer(void);
    Integer(const NaturalNumber& natural);
    Integer(const NaturalNumber& natural, bool positive);
    [[nodiscard]] bool IsZero(void) const;
    [[nodiscard]] bool IsEven(void) const;
    [[nodiscard]] size_t GetSize(void) const;
    [[nodiscard]] MathLib::String ToHexString(void) const;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    [[nodiscard]] Integer DivMod(const uint8_t& other, uint8_t& mod) const;
    [[nodiscard]] Integer DivModBySubtraction(const Integer& other, NaturalNumber& mod) const;
    [[nodiscard]] Integer LongDivMod(const Integer& other, NaturalNumber& mod) const;
    [[nodiscard]] Integer GreatestCommonDivisor(const Integer& other) const;
    [[nodiscard]] Integer LeastCommonMultiple(const Integer& other) const;
    void MultiplyBy256(uint8_t last);

    Integer& operator+=(const Integer& other);
    [[nodiscard]] Integer operator+(const Integer& other) const;
    Integer& operator++(void);
    Integer operator++(int);

    [[nodiscard]] Integer operator-(void) const;
    Integer& operator-=(const Integer& other);
    [[nodiscard]] Integer operator-(const Integer& other) const;
    Integer& operator--(void);
    Integer operator--(int);

    Integer& operator*=(const Integer& other);
    [[nodiscard]] Integer operator*(const Integer& other) const;

    Integer& operator/=(const Integer& other);
    [[nodiscard]] Integer operator/(const Integer& other) const;
    Integer& operator%=(const Integer& other);
    [[nodiscard]] Integer operator%(const Integer& other) const;

    Integer& operator^=(const NaturalNumber& other);
    [[nodiscard]] Integer operator^(const NaturalNumber& other) const;

    NaturalNumber natural;
    bool positive;

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;
};

#endif