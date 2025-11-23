#ifndef NaturalNumber_H
#define NaturalNumber_H
#include <Expected.hpp>
#include <Interfaces/Saveable.hpp>
#include <Interfaces/Printable.hpp>
#include <Interfaces/Orderable.hpp>

struct NaturalNumber : MathLib::Orderable, MathLib::Saveable, MathLib::Printable {
    NaturalNumber(void);
    NaturalNumber(size_t size);
    NaturalNumber(const MathLib::Sequence<uint8_t>& data);
    NaturalNumber(const MathLib::Sequence<char>& str);
    template <typename T>
    static NaturalNumber FromT(const T& number) {
        NaturalNumber ret;
        ret.data = MathLib::Array<uint8_t>((const uint8_t*)&number, sizeof(T));
        return ret;
    }
    [[nodiscard]] bool IsZero(void) const;
    [[nodiscard]] bool IsEven(void) const;
    [[nodiscard]] size_t GetSize(void) const;
    [[nodiscard]] MathLib::String ToHexString(void) const;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] NaturalNumber Add(const MathLib::Sequence<uint8_t>& other, uint8_t carry) const;
    [[nodiscard]] NaturalNumber DivMod(const uint8_t& other, uint8_t& mod) const;
    [[nodiscard]] NaturalNumber DivModBySubtraction(const NaturalNumber& other, NaturalNumber& mod) const;
    [[nodiscard]] NaturalNumber LongDivMod(const NaturalNumber& other, NaturalNumber& mod) const;
    [[nodiscard]] NaturalNumber Factorial(const NaturalNumber& level) const;
    [[nodiscard]] NaturalNumber GreatestCommonDivisor(const NaturalNumber& other) const;
    [[nodiscard]] NaturalNumber LeastCommonMultiple(const NaturalNumber& other) const;
    void MultiplyBy256(uint8_t last);

    NaturalNumber& operator+=(const MathLib::Sequence<uint8_t>& other);
    [[nodiscard]] NaturalNumber operator+(const MathLib::Sequence<uint8_t>& other) const;
    NaturalNumber& operator+=(const NaturalNumber& other);
    [[nodiscard]] NaturalNumber operator+(const NaturalNumber& other) const;
    NaturalNumber& operator++(void);
    NaturalNumber operator++(int);

    [[nodiscard]] MathLib::Expected<NaturalNumber> operator-(const NaturalNumber& other) const;
    NaturalNumber& operator-=(const NaturalNumber& other);
    NaturalNumber& operator--(void);
    NaturalNumber operator--(int);

    NaturalNumber& operator*=(const NaturalNumber& other);
    [[nodiscard]] NaturalNumber operator*(const NaturalNumber& other) const;

    NaturalNumber& operator/=(const NaturalNumber& other);
    [[nodiscard]] NaturalNumber operator/(const NaturalNumber& other) const;
    NaturalNumber& operator%=(const NaturalNumber& other);
    [[nodiscard]] NaturalNumber operator%(const NaturalNumber& other) const;

    NaturalNumber& operator^=(const NaturalNumber& other);
    [[nodiscard]] NaturalNumber operator^(const NaturalNumber& other) const;

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;

    private:
    [[nodiscard]] uint8_t At(size_t i) const;

    MathLib::Array<uint8_t> data;
};

#endif