#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include "../SequencesOfConstants.hpp"

/// @brief Builtin function
struct BuiltinFunction {
    /// @brief Name of the function
    std::string name;
    /// @brief Code of the function
    std::function<Node*(std::vector<Node*>)> function;

    /// @brief Creates new function
    /// @param n Name of the function
    /// @param func Code of the function
    BuiltinFunction(std::string n, std::function<Node*(std::vector<Node*>)> func);
};
/// @brief Default functions for equation solver
static const BuiltinFunction defaultBuiltinFuncs[] = {
    #define CreateBuiltinFunction(name, func, argc, ...)                                                            \
    BuiltinFunction(name, [](std::vector<Node*> args) {                                                             \
        return args.size() == argc ? new Node(Node::Type::Constant, std::to_string(func(__VA_ARGS__))) : nullptr;   \
    })
    CreateBuiltinFunction("log", Log<num_t>, 2, std::stold(args[0]->value), std::stold(args[1]->value)),
    CreateBuiltinFunction("ln", Ln<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("sin", std::sin, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("cos", std::cos, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("tan", std::tan, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("cot", Cot<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("sec", Sec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("csc", Csc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("crd", Crd<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("exsec", Exsec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("excsc", Excsc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("versin", Versin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("coversin", Coversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("vercos", Vercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("covercos", Covercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("haversin", Haversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("hacoversin", Hacoversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("havercos", Havercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("hacovercos", Hacovercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("sinh", std::sinh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("cosh", std::cosh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("tanh", std::tanh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("coth", HyperbolicCot<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("sech", HyperbolicSec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("csch", HyperbolicCsc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcsin", std::asin, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccos", std::acos, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arctan", std::atan, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccot", InversedCot<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcsec", InversedSec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccsc", InversedCsc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccrd", InversedCrd<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcexsec", InversedExsec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcexcsc", InversedExcsc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcversin", InversedVersin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccoversin", InversedCoversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcvercos", InversedVercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccovercos", InversedCovercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("archaversin", InversedHaversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("archacoversin", InversedHacoversin<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("archavercos", InversedHavercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("archacovercos", InversedHacovercos<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcsinh", std::asinh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccosh", std::acosh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arctanh", std::atanh, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccoth", InversedHyperbolicCot<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arcsech", InversedHyperbolicSec<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("arccsch", InversedHyperbolicCsc<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("harmonicNumber", HarmonicNumber<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("bernoulliNumber", BernoulliNumber<num_t>, 1, std::stold(args[0]->value), false),
    CreateBuiltinFunction("negativeBernoulliNumber", BernoulliNumber<num_t>, 1, std::stold(args[0]->value), true),
    CreateBuiltinFunction("favardConstants", FavardConstants<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("champernowneConstants", ChampernowneConstants<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("berahaConstants", BerahaConstants<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("hyperHarmonicNumber", HyperHarmonicNumber<num_t>, 2, std::stold(args[0]->value), std::stold(args[1]->value)),
    CreateBuiltinFunction("gregoryNumber", GregoryNumber<num_t>, 1, std::stold(args[0]->value)),
    CreateBuiltinFunction("metallicMean", MetallicMean<num_t>, 1, std::stold(args[0]->value)),
    #undef CreateBuiltinFunction
};

#endif