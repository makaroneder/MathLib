#ifndef MathLib_Math_ByteFiniteField_H
#define MathLib_Math_ByteFiniteField_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct ByteFiniteField;
    struct ByteFiniteField : Comparable<ByteFiniteField>, Printable {
        uint8_t value;

        ByteFiniteField(void);
        ByteFiniteField(uint8_t value);
        [[nodiscard]] ByteFiniteField XTimes(void) const;
        [[nodiscard]] ByteFiniteField operator+(const ByteFiniteField& other) const;
        ByteFiniteField& operator+=(const ByteFiniteField& other);
        [[nodiscard]] ByteFiniteField operator*(const ByteFiniteField& other) const;
        ByteFiniteField& operator*=(const ByteFiniteField& other);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        protected:
        [[nodiscard]] virtual bool Equals(const ByteFiniteField& other) const override;
    };
}

#endif