#ifndef MathLib_Utils_H
#define MathLib_Utils_H
#include "Math/Complex.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const Sequence<char>& str);
    [[nodiscard]] char ToUpper(char chr);
    [[nodiscard]] bool IsUpper(char chr);
    [[nodiscard]] bool IsLower(char chr);
    [[nodiscard]] bool IsAlpha(char chr);
    [[nodiscard]] bool IsDigit(char chr);
    /// @brief Checks if character is white space
    /// @param chr Character to check
    /// @return Is character white space
    [[nodiscard]] bool IsWhiteSpace(char chr);
    /// @brief Skips whitespace characters
    /// @param str String to skip
    /// @param i Current position in the string
    void SkipWhiteSpace(const Sequence<char>& str, size_t& i);
    [[nodiscard]] bool IsHexDigit(char chr);
    [[nodiscard]] bool IsAlphaDigit(char chr);
    [[nodiscard]] size_t BitwiseAnd(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseOr(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseXor(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseNot(size_t x);
    [[nodiscard]] size_t BitwiseNand(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseNor(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseXnor(size_t x, size_t y);
    [[nodiscard]] uint16_t SwapEndian16(uint16_t x);
    [[nodiscard]] uint32_t SwapEndian32(uint32_t x);
    [[nodiscard]] num_t Abs(num_t x);
    [[nodiscard]] num_t Pow(num_t x, num_t y);
    [[nodiscard]] complex_t Sqrt(complex_t x);
    [[nodiscard]] num_t Sqrt(num_t x);
    [[nodiscard]] num_t Cbrt(num_t x);
    [[nodiscard]] num_t NaturalLog(num_t x);
    [[nodiscard]] bool IsNaN(complex_t x);
    [[nodiscard]] bool IsInf(complex_t x);
    [[nodiscard]] complex_t Exp(complex_t x);
    [[nodiscard]] num_t Ceil(num_t x);
    [[nodiscard]] num_t Sin(num_t x);
    [[nodiscard]] num_t InversedSin(num_t x);
}

#endif