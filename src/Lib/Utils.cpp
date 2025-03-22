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
        StartAndReturnFromBenchmark(IsLower(chr) ? (chr + 'A' - 'a') : chr);
    }
    bool IsUpper(char chr) {
        StartAndReturnFromBenchmark(chr >= 'A' && chr <= 'Z');
    }
    bool IsLower(char chr) {
        StartAndReturnFromBenchmark(chr >= 'a' && chr <= 'z');
    }
    bool IsAlpha(char chr) {
        StartAndReturnFromBenchmark(IsUpper(chr) || IsLower(chr));
    }
    bool IsDigit(char chr) {
        StartAndReturnFromBenchmark(chr >= '0' && chr <= '9');
    }
    bool IsWhiteSpace(char chr) {
        StartAndReturnFromBenchmark(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r');
    }
    void SkipWhiteSpace(const Sequence<char>& str, size_t& i) {
        StartBenchmark
        while (i < str.GetSize() && IsWhiteSpace(str.At(i))) i++;
        EndBenchmark
    }
    bool IsHexDigit(char chr) {
        StartAndReturnFromBenchmark(IsDigit(chr) || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F'));
    }
    bool IsAlphaDigit(char chr) {
        StartAndReturnFromBenchmark(IsAlpha(chr) || IsDigit(chr));
    }
    size_t BitwiseAnd(size_t x, size_t y) {
        StartAndReturnFromBenchmark(x & y);
    }
    size_t BitwiseOr(size_t x, size_t y) {
        StartAndReturnFromBenchmark(x | y);
    }
    size_t BitwiseXor(size_t x, size_t y) {
        StartAndReturnFromBenchmark(x ^ y);
    }
    size_t BitwiseNot(size_t x) {
        StartAndReturnFromBenchmark(~x);
    }
    size_t BitwiseNand(size_t x, size_t y) {
        StartAndReturnFromBenchmark(~(x & y));
    }
    size_t BitwiseNor(size_t x, size_t y) {
        StartAndReturnFromBenchmark(~(x | y));
    }
    size_t BitwiseXnor(size_t x, size_t y) {
        StartAndReturnFromBenchmark(~(x ^ y));
    }
    uint16_t SwapEndian16(uint16_t x) {
        Swap<uint8_t>(((uint8_t*)&x)[0], ((uint8_t*)&x)[1]);
        return x;
    }
    uint32_t SwapEndian32(uint32_t x) {
        StartAndReturnFromBenchmark(
            ((x >> 24) & UINT8_MAX <<  0) |
            ((x >>  8) & UINT8_MAX <<  8) |
            ((x <<  8) & UINT8_MAX << 16) |
            ((x << 24) & UINT8_MAX << 24)
        );
    }
    num_t Abs(num_t x) {
        StartAndReturnFromBenchmark(x < 0 ? -x : x);
    }
    num_t Pow(num_t x, num_t y) {
        StartAndReturnFromBenchmark(Pow(complex_t(x, 0), complex_t(y, 0)).ToReal());
    }
    complex_t Sqrt(complex_t x) {
        StartAndReturnFromBenchmark(Pow(x, 0.5));
    }
    num_t Sqrt(num_t x) {
        StartAndReturnFromBenchmark(Sqrt(complex_t(x, 0)).ToReal());
    }
    num_t Cbrt(num_t x) {
        StartAndReturnFromBenchmark(Pow(x, 1 / 3));
    }
    num_t NaturalLog(num_t x) {
        StartAndReturnFromBenchmark(NaturalLog(complex_t(x, 0)).ToReal());
    }
    bool IsNaN(complex_t x) {
        StartAndReturnFromBenchmark(IsNaN(x.GetReal()) || IsNaN(x.GetImaginary()));
    }
    bool IsInf(complex_t x) {
        StartAndReturnFromBenchmark(IsInf(x.GetReal()) || IsInf(x.GetImaginary()));
    }
    complex_t Exp(complex_t x) {
        StartAndReturnFromBenchmark((complex_t(0, Sin(x.GetImaginary())) + Cos<num_t>(x.GetImaginary())) * Exp(x.GetReal()));
    }
    num_t Ceil(num_t x) {
        StartAndReturnFromBenchmark(Floor(x) + 1);
    }
    num_t Sin(num_t x) {
        StartAndReturnFromBenchmark(Sin(complex_t(x, 0)).ToReal());
    }
    num_t InversedSin(num_t x) {
        StartAndReturnFromBenchmark(InversedSin(complex_t(x, 0)).ToReal());
    }
}