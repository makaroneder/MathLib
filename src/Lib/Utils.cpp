#include "Utils.hpp"
#include "Math/Trigonometry.hpp"

namespace MathLib {
    void Panic(const Sequence<char>& str) {
        const size_t size = str.GetSize();
        char buff[size + 1];
        for (size_t i = 0; i < size; i++) buff[i] = str.At(i);
        buff[size] = '\0';
        Panic(buff);
    }
    char ToUpper(char chr) {
        return IsLower(chr) ? (chr + 'A' - 'a') : chr;
    }
    bool IsUpper(char chr) {
        return IsBetween(chr, 'A', 'Z');
    }
    bool IsLower(char chr) {
        return IsBetween(chr, 'a', 'z');
    }
    bool IsAlpha(char chr) {
        return IsUpper(chr) || IsLower(chr);
    }
    bool IsDigit(char chr) {
        return IsBetween(chr, '0', '9');
    }
    bool IsWhiteSpace(char chr) {
        return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
    }
    void SkipWhiteSpace(const Sequence<char>& str, size_t& i) {
        const size_t size = str.GetSize();
        while (i < size && IsWhiteSpace(str.AtUnsafe(i))) i++;
    }
    bool IsHexDigit(char chr) {
        return IsDigit(chr) || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F');
    }
    bool IsAlphaDigit(char chr) {
        return IsAlpha(chr) || IsDigit(chr);
    }
    size_t BitwiseAnd(size_t x, size_t y) {
        return x & y;
    }
    size_t BitwiseOr(size_t x, size_t y) {
        return x | y;
    }
    size_t BitwiseXor(size_t x, size_t y) {
        return x ^ y;
    }
    size_t BitwiseNot(size_t x) {
        return ~x;
    }
    size_t BitwiseNand(size_t x, size_t y) {
        return ~(x & y);
    }
    size_t BitwiseNor(size_t x, size_t y) {
        return ~(x | y);
    }
    size_t BitwiseXnor(size_t x, size_t y) {
        return ~(x ^ y);
    }
    uint16_t SwapEndian16(uint16_t x) {
        Swap<uint8_t>(((uint8_t*)&x)[0], ((uint8_t*)&x)[1]);
        return x;
    }
    uint32_t SwapEndian32(uint32_t x) {
        return ((x >> 24) & UINT8_MAX <<  0) | ((x >>  8) & UINT8_MAX <<  8) | ((x <<  8) & UINT8_MAX << 16) | ((x << 24) & UINT8_MAX << 24);
    }
    num_t Abs(num_t x) {
        return x < 0 ? -x : x;
    }
    num_t Pow(num_t x, num_t y) {
        return Pow(complex_t(x, 0), complex_t(y, 0)).ToReal();
    }
    complex_t Sqrt(complex_t x) {
        return Pow(x, 0.5);
    }
    num_t Sqrt(num_t x) {
        return Sqrt(complex_t(x, 0)).ToReal();
    }
    num_t Cbrt(num_t x) {
        return Pow(x, 1 / 3);
    }
    num_t NaturalLog(num_t x) {
        return NaturalLog(complex_t(x, 0)).ToReal();
    }
    bool IsNaN(complex_t x) {
        return IsNaN(x.GetReal()) || IsNaN(x.GetImaginary());
    }
    bool IsInf(complex_t x) {
        return IsInf(x.GetReal()) || IsInf(x.GetImaginary());
    }
    complex_t Exp(complex_t x) {
        return (complex_t(0, Sin(x.GetImaginary())) + Cos<num_t>(x.GetImaginary())) * Exp(x.GetReal());
    }
    num_t Ceil(num_t x) {
        return Floor(x) + 1;
    }
    num_t Sin(num_t x) {
        return Sin(complex_t(x, 0)).ToReal();
    }
    num_t InversedSin(num_t x) {
        return InversedSin(complex_t(x, 0)).ToReal();
    }
}