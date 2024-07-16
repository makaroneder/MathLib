#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include "../Sigmoid.hpp"
#include "../Trigonometry.hpp"
#include "../FunctionPointer.hpp"

using BuiltinFunctionPointer = FunctionPointer<Node*, const Array<const Node*>&>;
/// @brief Builtin function
struct BuiltinFunction : Allocatable {
    /// @brief Name of the builtin function
    String name;
    /// @brief Code of the builtin function
    BuiltinFunctionPointer function;

    /// @brief Creates empty builtin function
    BuiltinFunction(void);
    /// @brief Creates new builtin function
    /// @param n Name of the builtin function
    /// @param func Code of the builtin function
    BuiltinFunction(const String& n, const BuiltinFunctionPointer& func);
};

#define CreateBuiltinFunction(name, func, argc, ...)                                                                \
static Node* EquationSolverBuiltin##name(const void*, const Array<const Node*>& args) {                             \
    return args.GetSize() == argc ? new Node(Node::Type::Constant, ::ToString(func(__VA_ARGS__))) : nullptr;        \
}
#define CreateComplexBuiltinFunction(name, func, argc, ...)                                                         \
static Node* EquationSolverBuiltin##name(const void*, const Array<const Node*>& args) {                             \
    return args.GetSize() == argc ? new Node(Node::Type::Constant, func(__VA_ARGS__).ToString()) : nullptr;         \
}
CreateBuiltinFunction(Argument, InversedTan2, 1, args[0]->ToNumber().At(0).GetImaginary(), args[0]->ToNumber().At(0).GetReal())
CreateBuiltinFunction(Real, args[0]->ToNumber().At(0).GetReal, 1, )
CreateBuiltinFunction(Imaginary, args[0]->ToNumber().At(0).GetImaginary, 1, )
CreateComplexBuiltinFunction(Sigmoid, Sigmoid<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Log, Log<complex_t>, 2, args[0]->ToNumber().At(0), args[1]->ToNumber().At(0))
CreateComplexBuiltinFunction(NaturalLog, NaturalLog, 1, args[0]->ToNumber().At(0))
CreateBuiltinFunction(Sign, Sign, 1, args[0]->ToNumber().At(0).GetReal())
CreateBuiltinFunction(Floor, Floor, 1, args[0]->ToNumber().At(0).GetReal())
CreateBuiltinFunction(Ceil, Ceil, 1, args[0]->ToNumber().At(0).GetReal())
CreateBuiltinFunction(InversedTan2, InversedTan2, 2, args[0]->ToNumber().At(0).GetReal(), args[1]->ToNumber().At(0).GetReal())
CreateComplexBuiltinFunction(Sin, Sin, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Cos, Cos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Tan, Tan<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Cot, Cot<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Sec, Sec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Csc, Csc<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Crd, Crd<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Exsec, Exsec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Excsc, Excsc<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Versin, Versin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Coversin, Coversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Vercos, Vercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Covercos, Covercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Haversin, Haversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Hacoversin, Hacoversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Havercos, Havercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(Hacovercos, Hacovercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicSin, HyperbolicSin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicCos, HyperbolicCos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicTan, HyperbolicTan<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicCot, HyperbolicCot<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicSec, HyperbolicSec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(HyperbolicCsc, HyperbolicCsc<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedSin, InversedSin, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCos, InversedCos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedTan, InversedTan<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCot, InversedCot<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedSec, InversedSec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCsc, InversedCsc<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCrd, InversedCrd<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedExsec, InversedExsec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedExcsc, InversedExcsc<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedVersin, InversedVersin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCoversin, InversedCoversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedVercos, InversedVercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedCovercos, InversedCovercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHaversin, InversedHaversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHacoversin, InversedHacoversin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHavercos, InversedHavercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHacovercos, InversedHacovercos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicSin, InversedHyperbolicSin<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicCos, InversedHyperbolicCos<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicTan, InversedHyperbolicTan<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicCot, InversedHyperbolicCot<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicSec, InversedHyperbolicSec<complex_t>, 1, args[0]->ToNumber().At(0))
CreateComplexBuiltinFunction(InversedHyperbolicCsc, InversedHyperbolicCsc<complex_t>, 1, args[0]->ToNumber().At(0))

#undef CreateComplexBuiltinFunction
#undef CreateBuiltinFunction
#define CreateBuiltinFunction(name, func) BuiltinFunction(name, BuiltinFunctionPointer(nullptr, EquationSolverBuiltin##func))

/// @brief Default functions for equation solver
static const BuiltinFunction defaultBuiltinFuncs[] = {
    CreateBuiltinFunction("arg", Argument),
    CreateBuiltinFunction("re", Real),
    CreateBuiltinFunction("im", Imaginary),
    CreateBuiltinFunction("sigmoid", Sigmoid),
    CreateBuiltinFunction("log", Log),
    CreateBuiltinFunction("ln", NaturalLog),
    CreateBuiltinFunction("sign", Sign),
    CreateBuiltinFunction("floor", Floor),
    CreateBuiltinFunction("ceil", Ceil),
    CreateBuiltinFunction("atan2", InversedTan2),
    CreateBuiltinFunction("sin", Sin),
    CreateBuiltinFunction("cos", Cos),
    CreateBuiltinFunction("tan", Tan),
    CreateBuiltinFunction("cot", Cot),
    CreateBuiltinFunction("sec", Sec),
    CreateBuiltinFunction("csc", Csc),
    CreateBuiltinFunction("crd", Crd),
    CreateBuiltinFunction("exsec", Exsec),
    CreateBuiltinFunction("excsc", Excsc),
    CreateBuiltinFunction("versin", Versin),
    CreateBuiltinFunction("coversin", Coversin),
    CreateBuiltinFunction("vercos", Vercos),
    CreateBuiltinFunction("covercos", Covercos),
    CreateBuiltinFunction("haversin", Haversin),
    CreateBuiltinFunction("hacoversin", Hacoversin),
    CreateBuiltinFunction("havercos", Havercos),
    CreateBuiltinFunction("hacovercos", Hacovercos),
    CreateBuiltinFunction("sinh", HyperbolicSin),
    CreateBuiltinFunction("cosh", HyperbolicCos),
    CreateBuiltinFunction("tanh", HyperbolicTan),
    CreateBuiltinFunction("coth", HyperbolicCot),
    CreateBuiltinFunction("sech", HyperbolicSec),
    CreateBuiltinFunction("csch", HyperbolicCsc),
    CreateBuiltinFunction("arcsin", InversedSin),
    CreateBuiltinFunction("arccos", InversedCos),
    CreateBuiltinFunction("arctan", InversedTan),
    CreateBuiltinFunction("arccot", InversedCot),
    CreateBuiltinFunction("arcsec", InversedSec),
    CreateBuiltinFunction("arccsc", InversedCsc),
    CreateBuiltinFunction("arccrd", InversedCrd),
    CreateBuiltinFunction("arcexsec", InversedExsec),
    CreateBuiltinFunction("arcexcsc", InversedExcsc),
    CreateBuiltinFunction("arcversin", InversedVersin),
    CreateBuiltinFunction("arccoversin", InversedCoversin),
    CreateBuiltinFunction("arcvercos", InversedVercos),
    CreateBuiltinFunction("arccovercos", InversedCovercos),
    CreateBuiltinFunction("archaversin", InversedHaversin),
    CreateBuiltinFunction("archacoversin", InversedHacoversin),
    CreateBuiltinFunction("archavercos", InversedHavercos),
    CreateBuiltinFunction("archacovercos", InversedHacovercos),
    CreateBuiltinFunction("arcsinh", InversedHyperbolicSin),
    CreateBuiltinFunction("arccosh", InversedHyperbolicCos),
    CreateBuiltinFunction("arctanh", InversedHyperbolicTan),
    CreateBuiltinFunction("arccoth", InversedHyperbolicCot),
    CreateBuiltinFunction("arcsech", InversedHyperbolicSec),
    CreateBuiltinFunction("arccsch", InversedHyperbolicCsc),
};

#undef CreateBuiltinFunction
#endif