#include "Utils.hpp"
#include "Math/Trigonometry.hpp"

namespace MathLib {
    void Panic(String str) {
        Panic(str.GetValue());
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
    void SkipWhiteSpace(const String& str, size_t& i) {
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