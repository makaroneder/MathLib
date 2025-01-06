#ifndef MathLib_Host_H
#define MathLib_Host_H
#include "Math/Complex.hpp"
#ifndef Freestanding
#include <ostream>
#endif

namespace MathLib {
    #ifndef Freestanding
    /// @brief Prints a string
    /// @param stream Stream to print string into
    /// @param string String to print
    /// @return Stream
    std::ostream& operator<<(std::ostream& stream, const String& string);
    /// @brief Prints a string representation of printable structures
    /// @param stream Stream to print data into
    /// @param printable Data to print
    /// @return Stream
    std::ostream& operator<<(std::ostream& stream, const Printable& printable);
    #endif

    struct Thread;
    [[noreturn]] void Panic(String str);
    [[noreturn]] void Panic(const char* str);
    [[nodiscard]] num_t StringToNumber(String str);
    [[nodiscard]] String ToString(num_t x);
    [[nodiscard]] num_t GetTime(void);
    [[nodiscard]] size_t GetThreadCount(void);
    [[nodiscard]] Thread* AllocThread(void);
    void DeallocThread(Thread* thread);
    [[nodiscard]] num_t MakeNaN(void);
    [[nodiscard]] num_t MakeInf(void);
    [[nodiscard]] char ToUpper(char chr);
    [[nodiscard]] bool IsUpper(char chr);
    [[nodiscard]] bool IsLower(char chr);
    [[nodiscard]] bool IsAlpha(char chr);
    [[nodiscard]] bool IsDigit(char chr);
    /// @brief Checks if character is white space
    /// @param chr Character to check
    /// @return Is character white space
    [[nodiscard]] bool IsWhiteSpace(char chr);
    [[nodiscard]] bool IsHexDigit(char chr);
    [[nodiscard]] bool IsAlphaDigit(char chr);
    [[nodiscard]] size_t BitwiseAnd(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseOr(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseXor(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseNot(size_t x);
    [[nodiscard]] size_t BitwiseNand(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseNor(size_t x, size_t y);
    [[nodiscard]] size_t BitwiseXnor(size_t x, size_t y);
    [[nodiscard]] num_t RandomFloat(void);
    [[nodiscard]] num_t Abs(num_t x);
    [[nodiscard]] num_t Abs(complex_t x);
    [[nodiscard]] complex_t Pow(complex_t x, complex_t y);
    [[nodiscard]] num_t Pow(num_t x, num_t y);
    [[nodiscard]] complex_t Sqrt(complex_t x);
    [[nodiscard]] num_t Sqrt(num_t x);
    [[nodiscard]] num_t Cbrt(num_t x);
    [[nodiscard]] num_t NaturalLog(num_t x);
    [[nodiscard]] complex_t NaturalLog(complex_t x);
    [[nodiscard]] bool IsNaN(num_t x);
    [[nodiscard]] bool IsNaN(complex_t x);
    [[nodiscard]] bool IsInf(num_t x);
    [[nodiscard]] bool IsInf(complex_t x);
    [[nodiscard]] num_t Exp(num_t x);
    [[nodiscard]] complex_t Exp(complex_t x);
    [[nodiscard]] num_t Round(num_t x);
    [[nodiscard]] num_t Floor(num_t x);
    [[nodiscard]] num_t Ceil(num_t x);
    [[nodiscard]] num_t Sin(num_t x);
    [[nodiscard]] complex_t Sin(complex_t x);
    [[nodiscard]] num_t InversedSin(num_t x);
    [[nodiscard]] complex_t InversedSin(complex_t x);
    [[nodiscard]] num_t InversedTan2(num_t y, num_t x);
}

#endif