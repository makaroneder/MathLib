#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include "../Trigonometry.hpp"

/// @brief Builtin function
struct BuiltinFunction {
    /// @brief Name of the function
    std::string name;
    /// @brief Code of the function
    std::function<Node*(std::vector<const Node*>)> function;

    /// @brief Creates new function
    /// @param n Name of the function
    /// @param func Code of the function
    BuiltinFunction(std::string n, std::function<Node*(std::vector<const Node*>)> func);
};
static std::string ComplexToString(complex_t num) {
    if (num.imag() == 0) return std::to_string(num.real());
    else return '(' + std::to_string(num.real()) + ", " + std::to_string(num.imag()) + ')';
}
/// @brief Default functions for equation solver
static const BuiltinFunction defaultBuiltinFuncs[] = {
    #define CreateBuiltinFunction(name, func, argc, ...)                                                                    \
    BuiltinFunction(name, [](std::vector<const Node*> args) {                                                               \
        return args.size() == argc ? new Node(Node::Type::Constant, std::to_string(func(__VA_ARGS__))) : nullptr;           \
    })
    #define CreateComplexBuiltinFunction(name, func, argc, ...)                                                             \
    BuiltinFunction(name, [](std::vector<const Node*> args) {                                                               \
        return args.size() == argc ? new Node(Node::Type::ComplexConstant, ComplexToString(func(__VA_ARGS__))) : nullptr;   \
    })
    CreateBuiltinFunction("arg", std::atan2, 1, args[0]->ToNumber().at(0).imag(), args[0]->ToNumber().at(0).real()),
    CreateBuiltinFunction("re", args[0]->ToNumber().at(0).real, 1, ),
    CreateBuiltinFunction("im", args[0]->ToNumber().at(0).imag, 1, ),
    CreateComplexBuiltinFunction("sigmoid", Sigmoid<complex_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("log", Log<num_t>, 2, args[0]->ToNumber().at(0).real(), args[1]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("ln", std::log, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("floor", std::floor, 1, args[0]->ToNumber().at(0).real()),
    CreateBuiltinFunction("ceil", std::ceil, 1, args[0]->ToNumber().at(0).real()),
    CreateBuiltinFunction("atan2", std::atan2, 2, args[0]->ToNumber().at(0).real(), args[1]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("sin", std::sin, 1, args[0]->ToNumber().at(0)),
    CreateComplexBuiltinFunction("cos", std::cos, 1, args[0]->ToNumber().at(0)),
    CreateComplexBuiltinFunction("tan", std::tan, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("cot", Cot<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("sec", Sec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("csc", Csc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("crd", Crd<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("exsec", Exsec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("excsc", Excsc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("versin", Versin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("coversin", Coversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("vercos", Vercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("covercos", Covercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("haversin", Haversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("hacoversin", Hacoversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("havercos", Havercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("hacovercos", Hacovercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("sinh", std::sinh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("cosh", std::cosh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("tanh", std::tanh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("coth", HyperbolicCot<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("sech", HyperbolicSec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("csch", HyperbolicCsc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcsin", std::asin, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccos", std::acos, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arctan", std::atan, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccot", InversedCot<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcsec", InversedSec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccsc", InversedCsc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccrd", InversedCrd<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcexsec", InversedExsec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcexcsc", InversedExcsc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcversin", InversedVersin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccoversin", InversedCoversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcvercos", InversedVercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccovercos", InversedCovercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("archaversin", InversedHaversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("archacoversin", InversedHacoversin<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("archavercos", InversedHavercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("archacovercos", InversedHacovercos<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcsinh", std::asinh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccosh", std::acosh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arctanh", std::atanh, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccoth", InversedHyperbolicCot<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arcsech", InversedHyperbolicSec<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    CreateComplexBuiltinFunction("arccsch", InversedHyperbolicCsc<complex_t>, 1, args[0]->ToNumber().at(0).real()),
    #undef CreateComplexBuiltinFunction
    #undef CreateBuiltinFunction
};

#endif