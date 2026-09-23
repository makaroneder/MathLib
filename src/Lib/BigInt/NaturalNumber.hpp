#ifndef MathLib_BigInt_NaturalNumber_H
#define MathLib_BigInt_NaturalNumber_H
#include "../Expected.hpp"
#include "../SingleTypePair.hpp"
#include "../Interfaces/Saveable.hpp"
#include "../Interfaces/Printable.hpp"
#include "../Interfaces/Orderable.hpp"

namespace MathLib {
    struct NaturalNumber;
    struct NaturalNumber : Orderable<NaturalNumber>, Saveable, Printable {
        Array<uint8_t> data;

        NaturalNumber(void);
        NaturalNumber(size_t size);
        NaturalNumber(const Sequence<uint8_t>& data);
        NaturalNumber(const Sequence<char>& str);
        template <typename T>
        static NaturalNumber FromT(const T& number) {
            NaturalNumber ret;
            ret.data = Array<uint8_t>((const uint8_t*)&number, sizeof(T));
            return ret;
        }
        [[nodiscard]] bool IsZero(void) const;
        [[nodiscard]] bool IsEven(void) const;
        [[nodiscard]] size_t GetDegree(void) const;
        [[nodiscard]] size_t GetSize(void) const;
        [[nodiscard]] String ToHexString(void) const;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] NaturalNumber Add(const Sequence<uint8_t>& other, uint8_t carry) const;
        [[nodiscard]] NaturalNumber DivMod(const uint8_t& other, uint8_t& mod) const;
        [[nodiscard]] NaturalNumber DivModBySubtraction(const NaturalNumber& other, NaturalNumber& mod) const;
        [[nodiscard]] NaturalNumber LongDivMod(const NaturalNumber& other, NaturalNumber& mod) const;
        [[nodiscard]] NaturalNumber Factorial(const NaturalNumber& level) const;
        [[nodiscard]] NaturalNumber GreatestCommonDivisor(const NaturalNumber& other) const;
        [[nodiscard]] NaturalNumber LeastCommonMultiple(const NaturalNumber& other) const;
        [[nodiscard]] NaturalNumber GreatestCommonDivisorBySubtraction(const NaturalNumber& other) const;
        [[nodiscard]] NaturalNumber LeastCommonMultipleBySubtraction(const NaturalNumber& other) const;
        [[nodiscard]] NaturalNumber GenerateCoprime(void) const;
        [[nodiscard]] NaturalNumber GetModularInverse(const NaturalNumber& mod) const;
        void MultiplyBy256(uint8_t last);

        NaturalNumber& operator+=(const Sequence<uint8_t>& other);
        [[nodiscard]] NaturalNumber operator+(const Sequence<uint8_t>& other) const;
        NaturalNumber& operator+=(const NaturalNumber& other);
        [[nodiscard]] NaturalNumber operator+(const NaturalNumber& other) const;
        NaturalNumber& operator++(void);
        NaturalNumber operator++(int);

        [[nodiscard]] Expected<NaturalNumber> operator-(const NaturalNumber& other) const;
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
        [[nodiscard]] virtual bool LessThanEqual(const NaturalNumber& other) const override;

        private:
        [[nodiscard]] uint8_t At(size_t i) const;
    };
}

#endif