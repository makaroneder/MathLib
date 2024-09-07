#ifndef Host_H
#define Host_H
#include "Complex.hpp"

#ifndef Freestanding
#include <ostream>
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
num_t StringToNumber(String str);
String ToString(num_t x);
num_t GetTime(void);
size_t GetThreadCount(void);
Thread* AllocThread(void);
void DeallocThread(Thread* thread);
num_t MakeNaN(void);
num_t MakeInf(void);
char ToUpper(char chr);
bool IsUpper(char chr);
bool IsLower(char chr);
bool IsAlpha(char chr);
bool IsDigit(char chr);
/// @brief Checks if character is white space
/// @param chr Character to check
/// @return Is character white space
bool IsWhiteSpace(const char& chr);
bool IsHexDigit(char chr);
bool IsAlphaDigit(char chr);
size_t BitwiseAnd(size_t x, size_t y);
size_t BitwiseOr(size_t x, size_t y);
size_t BitwiseXor(size_t x, size_t y);
size_t BitwiseNot(size_t x);
size_t BitwiseNand(size_t x, size_t y);
size_t BitwiseNor(size_t x, size_t y);
size_t BitwiseXnor(size_t x, size_t y);
num_t RandomFloat(void);
num_t Abs(num_t x);
num_t Abs(complex_t x);
num_t Max(num_t x, num_t y);
num_t Min(num_t x, num_t y);
complex_t Pow(complex_t x, complex_t y);
num_t Pow(num_t x, num_t y);
complex_t Sqrt(complex_t x);
num_t Sqrt(num_t x);
num_t Cbrt(num_t x);
num_t NaturalLog(num_t x);
complex_t NaturalLog(complex_t x);
bool IsNaN(num_t x);
bool IsNaN(complex_t x);
bool IsInf(num_t x);
bool IsInf(complex_t x);
num_t Exp(num_t x);
complex_t Exp(complex_t x);
num_t Round(num_t x);
num_t Floor(num_t x);
num_t Ceil(num_t x);
num_t Sin(num_t x);
complex_t Sin(complex_t x);
num_t InversedSin(num_t x);
complex_t InversedSin(complex_t x);
num_t InversedTan2(num_t y, num_t x);

template <typename T>
constexpr void Swap(T& a, T& b) {
    const T tmp = a;
    a = b;
    b = tmp;
}

#endif