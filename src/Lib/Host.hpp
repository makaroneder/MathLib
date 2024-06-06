#ifndef Host_H
#define Host_H
#include "Printable.hpp"
#include <ostream>

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

void Panic(String str);
num_t StringToNumber(String str);
String ToString(num_t x);
bool IsUpper(char chr);
bool IsLower(char chr);
bool IsAlpha(char chr);
bool IsDigit(char chr);
bool IsHexDigit(char chr);
bool IsAlphaDigit(char chr);
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
num_t Gamma(num_t x);
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

// 2 All:
// std::complex, std::function

// 1 ComplexPosition.hpp: std::complex
// 1 Factorial.hpp: std::complex
// 1 Typedefs.hpp: std::complex
// 1 EquationSolver/Optimizer.cpp: std::complex
// 1 EquationSolver/Node.hpp: std::complex
// 1 EquationSolver/Node.cpp: std::complex
// 1 EquationSolver/BuiltinFunction.cpp: std::function
// 2 Renderer.hpp: std::function, std::complex
// 2 EquationSolver/BuiltinFunction.hpp: std::function, std::complex

#endif