#ifndef Host_H
#define Host_H
#include "Typedefs.hpp"

bool IsUpper(char chr);
bool IsLower(char chr);
bool IsAlpha(char chr);
bool IsDigit(char chr);
bool IsHexDigit(char chr);
bool IsAlphaDigit(char chr);
num_t Abs(num_t x);
num_t Abs(complex_t x);
num_t Max(num_t x, num_t y);
complex_t Pow(complex_t x, complex_t y);
num_t Pow(num_t x, num_t y);
complex_t Sqrt(complex_t x);
num_t Sqrt(num_t x);
num_t NaturalLog(num_t x);
complex_t NaturalLog(complex_t x);
bool IsNaN(num_t x);
bool IsNaN(complex_t x);
bool IsInf(num_t x);
bool IsInf(complex_t x);

template <typename T>
constexpr void Swap(T& a, T& b) {
    const T tmp = a;
    a = b;
    b = tmp;
}

// 42 All:
// std::round, std::sin, std::tgamma, std::getline, fopen, fflush,
// fclose, std::string, std::runtime_error, std::stold, rand,
// RAND_MAX, std::exp, fwrite, fread, fprintf, fscanf, fseek, std::cbrt,
// std::atan, std::asinh, std::cosh, std::acosh, std::cos, std::acos, std::asin, std::atan2,
// std::floor, std::ceil, std::tan, std::sinh, std::tanh, std::atanh, std::to_string, std::complex,
// std::vector, std::ifstream, std::istringstream, std::ostream, FILE, std::function

// 1 Printable.cpp: std::ostream
// 1 ComplexPosition.hpp: std::complex
// 1 EquationSolver/Variable.cpp: std::string
// 1 EquationSolver/Tokenizer.hpp: std::string
// 1 EquationSolver/Preprocesor.hpp: std::string
// 1 Cryptography/ROT13.hpp: std::string
// 1 Cryptography/ROT13.cpp: std::string
// 1 Cryptography/Protocol.hpp: std::string
// 1 Cryptography/CaesarCipher.hpp: std::string
// 1 Cryptography/CaesarCipher.cpp: std::string
// 1 Cryptography/AtbashCipher.hpp: std::string
// 1 Cryptography/AtbashCipher.cpp: std::string
// 1 Cryptography/AffineCipher.hpp: std::string
// 1 Chemistry/ReactionElements.hpp: std::string
// 1 Cryptography/Protocol.cpp: std::string
// 1 Color.hpp: std::sin
// 1 Set.hpp: std::vector
// 2 EquationSolver/State.hpp: std::vector, std::string
// 2 EquationSolver/State.cpp: std::vector, std::string
// 2 EquationSolver/Variable.hpp: std::string, std::to_string
// 2 Printable.hpp: std::string, std::ostream
// 2 Saveable.hpp: FILE, std::string
// 2 Chemistry/Molecule.hpp: std::vector, std::string
// 2 EquationSolver/Function.hpp: std::string, std::vector
// 2 EquationSolver/Function.cpp: std::string, std::vector
// 2 Cryptography/AffineCipher.cpp: std::string, std::stold
// 2 Factorial.hpp: std::complex, std::tgamma
// 2 EquationSolver/Tokenizer.cpp: std::to_string, std::string
// 3 Chemistry/Reaction.hpp: std::vector, std::string, std::round
// 3 Chemistry/Elements.hpp: std::string, std::round, std::to_string
// 3 EquationSolver/BuiltinFunction.cpp: std::string, std::function, std::vector
// 3 EquationSolver/Preprocesor.cpp: std::string, std::ifstream, std::getline
// 3 Physics/SIUnits.hpp: std::string, std::to_string, std::runtime_error
// 4 EquationSolver/Node.hpp: std::string, std::vector, std::function, std::complex
// 4 EquationSolver/Optimizer.cpp: std::vector, std::complex, std::to_string, std::string
// 4 Geometry/WavefrontObject.hpp: std::vector, std::ifstream, std::string, std::getline
// 5 Saveable.cpp: FILE, std::string, fopen, fflush, fclose
// 5 Renderer.hpp: std::vector, std::string, std::function, std::complex, FILE
// 6 Typedefs.hpp: rand, RAND_MAX, std::exp, std::string, std::to_string, std::complex
// 6 NeuralNetwork.hpp: std::vector, std::string, std::to_string, FILE, fwrite, fread
// 6 Constants.hpp: std::atan, std::cbrt, std::asinh, std::cosh, std::acosh, std::exp
// 6 Quaternion.hpp: std::string, std::to_string, std::acos, std::cos, std::sin, std::exp
// 7 EquationSolver/Node.cpp: std::vector, std::function, std::string, std::complex, std::to_string, std::istringstream, std::stold
// 7 Matrix.hpp: std::vector, std::string, std::to_string, FILE, fwrite, fread, std::runtime_error
// 7 Trigonometry.hpp: std::cos, std::sin, std::exp, std::atan, std::acos, std::asin, std::acosh
// 8 Renderer.cpp: std::vector, std::function, FILE, fprintf, fwrite, fscanf, fseek, fread
// 20 EquationSolver/BuiltinFunction.hpp: std::string, std::function, std::vector, std::complex, std::to_string, std::atan2, std::floor, std::ceil, std::sin, std::cos, std::tan, std::sinh, std::cosh, std::tanh, std::asin, std::acos, std::atan, std::asinh, std::acosh, std::atanh

#endif