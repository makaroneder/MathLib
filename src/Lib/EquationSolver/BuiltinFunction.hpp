#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include "../Trigonometry.hpp"

/// @brief Builtin function
struct BuiltinFunction {
    /// @brief Name of the builtin function
    String name;
    /// @brief Code of the builtin function
    std::function<Node*(Array<const Node*>)> function;

    /// @brief Creates empty builtin function
    BuiltinFunction(void);
    /// @brief Creates new builtin function
    /// @param n Name of the builtin function
    /// @param func Code of the builtin function
    BuiltinFunction(String n, std::function<Node*(Array<const Node*>)> func);
};
/// @brief Default functions for equation solver
static const BuiltinFunction defaultBuiltinFuncs[] = {
    #define CreateBuiltinFunction(name, func, argc, ...)                                                                    \
    BuiltinFunction(name, [](Array<const Node*> args) {                                                             \
        return args.GetSize() == argc ? new Node(Node::Type::Constant, ::ToString(func(__VA_ARGS__))) : nullptr;    \
    })
    #define CreateComplexBuiltinFunction(name, func, argc, ...)                                                             \
    BuiltinFunction(name, [](Array<const Node*> args) {                                                               \
        return args.GetSize() == argc ? new Node(Node::Type::ComplexConstant, ComplexToString<num_t>(func(__VA_ARGS__))) : nullptr;   \
    })
    CreateBuiltinFunction("arg", InversedTan2, 1, args[0]->ToNumber().At(0).imag(), args[0]->ToNumber().At(0).real()),
    CreateBuiltinFunction("re", args[0]->ToNumber().At(0).real, 1, ),
    CreateBuiltinFunction("im", args[0]->ToNumber().At(0).imag, 1, ),
    CreateComplexBuiltinFunction("sigmoid", Sigmoid<complex_t>, 1, args[0]->ToNumber().At(0)),
    CreateBuiltinFunction("log", Log<num_t>, 2, args[0]->ToNumber().At(0).real(), args[1]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("ln", NaturalLog, 1, args[0]->ToNumber().At(0)),
    CreateBuiltinFunction("floor", Floor, 1, args[0]->ToNumber().At(0).real()),
    CreateBuiltinFunction("ceil", Ceil, 1, args[0]->ToNumber().At(0).real()),
    CreateBuiltinFunction("atan2", InversedTan2, 2, args[0]->ToNumber().At(0).real(), args[1]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("sin", Sin, 1, args[0]->ToNumber().At(0)),
    CreateComplexBuiltinFunction("cos", Cos<complex_t>, 1, args[0]->ToNumber().At(0)),
    CreateComplexBuiltinFunction("tan", Tan, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("cot", Cot<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("sec", Sec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("csc", Csc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("crd", Crd<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("exsec", Exsec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("excsc", Excsc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("versin", Versin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("coversin", Coversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("vercos", Vercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("covercos", Covercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("haversin", Haversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("hacoversin", Hacoversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("havercos", Havercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("hacovercos", Hacovercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("sinh", HyperbolicSin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("cosh", HyperbolicCos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("tanh", HyperbolicTan<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("coth", HyperbolicCot<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("sech", HyperbolicSec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("csch", HyperbolicCsc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcsin", InversedSin, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccos", InversedCos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arctan", InversedTan<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccot", InversedCot<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcsec", InversedSec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccsc", InversedCsc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccrd", InversedCrd<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcexsec", InversedExsec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcexcsc", InversedExcsc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcversin", InversedVersin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccoversin", InversedCoversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcvercos", InversedVercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccovercos", InversedCovercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("archaversin", InversedHaversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("archacoversin", InversedHacoversin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("archavercos", InversedHavercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("archacovercos", InversedHacovercos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcsinh", InversedHyperbolicSin<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccosh", InversedHyperbolicCos<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arctanh", InversedHyperbolicTan<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccoth", InversedHyperbolicCot<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arcsech", InversedHyperbolicSec<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    CreateComplexBuiltinFunction("arccsch", InversedHyperbolicCsc<complex_t>, 1, args[0]->ToNumber().At(0).real()),
    #undef CreateComplexBuiltinFunction
    #undef CreateBuiltinFunction
};

#endif