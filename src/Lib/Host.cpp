#include "Host.hpp"
#include "Math/Trigonometry.hpp"

namespace MathLib {
    void Panic(String str) {
        StartBenchmark
        Panic(str.GetValue());
        EndBenchmark
    }
    char ToUpper(char chr) {
        StartBenchmark
        ReturnFromBenchmark(IsLower(chr) ? (chr + 'A' - 'a') : chr);
    }
    bool IsUpper(char chr) {
        StartBenchmark
        ReturnFromBenchmark(chr >= 'A' && chr <= 'Z');
    }
    bool IsLower(char chr) {
        StartBenchmark
        ReturnFromBenchmark(chr >= 'a' && chr <= 'z');
    }
    bool IsAlpha(char chr) {
        StartBenchmark
        ReturnFromBenchmark(IsUpper(chr) || IsLower(chr));
    }
    bool IsDigit(char chr) {
        StartBenchmark
        ReturnFromBenchmark(chr >= '0' && chr <= '9');
    }
    bool IsWhiteSpace(char chr) {
        StartBenchmark
        ReturnFromBenchmark(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r');
    }
    bool IsHexDigit(char chr) {
        StartBenchmark
        ReturnFromBenchmark(IsDigit(chr) || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F'));
    }
    bool IsAlphaDigit(char chr) {
        StartBenchmark
        ReturnFromBenchmark(IsAlpha(chr) || IsDigit(chr));
    }
    size_t BitwiseAnd(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(x & y);
    }
    size_t BitwiseOr(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(x | y);
    }
    size_t BitwiseXor(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(x ^ y);
    }
    size_t BitwiseNot(size_t x) {
        StartBenchmark
        ReturnFromBenchmark(~x);
    }
    size_t BitwiseNand(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(~(x & y));
    }
    size_t BitwiseNor(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(~(x | y));
    }
    size_t BitwiseXnor(size_t x, size_t y) {
        StartBenchmark
        ReturnFromBenchmark(~(x ^ y));
    }
    num_t Abs(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(x < 0 ? -x : x);
    }
    num_t Pow(num_t x, num_t y) {
        StartBenchmark
        ReturnFromBenchmark(Pow(complex_t(x, 0), complex_t(y, 0)).ToReal());
    }
    complex_t Sqrt(complex_t x) {
        StartBenchmark
        ReturnFromBenchmark(Pow(x, 0.5));
    }
    num_t Sqrt(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(Sqrt(complex_t(x, 0)).ToReal());
    }
    #ifdef Freestanding
    num_t Cbrt(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(Pow(x, 1 / 3));
    }
    #endif
    num_t NaturalLog(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(NaturalLog(complex_t(x, 0)).ToReal());
    }
    bool IsNaN(complex_t x) {
        StartBenchmark
        ReturnFromBenchmark(IsNaN(x.GetReal()) || IsNaN(x.GetImaginary()));
    }
    bool IsInf(complex_t x) {
        StartBenchmark
        ReturnFromBenchmark(IsInf(x.GetReal()) || IsInf(x.GetImaginary()));
    }
    complex_t Exp(complex_t x) {
        StartBenchmark
        ReturnFromBenchmark((complex_t(0, Sin(x.GetImaginary())) + Cos<num_t>(x.GetImaginary())) * Exp(x.GetReal()));
    }
    num_t Ceil(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(Floor(x) + 1);
    }
    num_t Sin(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(Sin(complex_t(x, 0)).ToReal());
    }
    num_t InversedSin(num_t x) {
        StartBenchmark
        ReturnFromBenchmark(InversedSin(complex_t(x, 0)).ToReal());
    }
}