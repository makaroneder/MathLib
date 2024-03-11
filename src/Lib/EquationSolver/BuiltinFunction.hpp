#ifndef BuiltinFunction_H
#define BuiltinFunction_H
#include "Node.hpp"
#include "../SequencesOfConstants.hpp"

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
/// @brief Default functions for equation solver
static const BuiltinFunction defaultBuiltinFuncs[] = {
    #define CreateBuiltinFunction(name, func, argc, ...)                                                            \
    BuiltinFunction(name, [](std::vector<const Node*> args) {                                                       \
        return args.size() == argc ? new Node(Node::Type::Constant, std::to_string(func(__VA_ARGS__))) : nullptr;   \
    })
    CreateBuiltinFunction("log", Log<num_t>, 2, args[0]->ToNumber().at(0), args[1]->ToNumber().at(0)),
    CreateBuiltinFunction("ln", Ln<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("floor", std::floor, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("ceil", std::ceil, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("sin", std::sin, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("cos", std::cos, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("tan", std::tan, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("cot", Cot<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("sec", Sec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("csc", Csc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("crd", Crd<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("exsec", Exsec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("excsc", Excsc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("versin", Versin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("coversin", Coversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("vercos", Vercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("covercos", Covercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("haversin", Haversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("hacoversin", Hacoversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("havercos", Havercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("hacovercos", Hacovercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("sinh", std::sinh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("cosh", std::cosh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("tanh", std::tanh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("coth", HyperbolicCot<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("sech", HyperbolicSec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("csch", HyperbolicCsc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcsin", std::asin, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccos", std::acos, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arctan", std::atan, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccot", InversedCot<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcsec", InversedSec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccsc", InversedCsc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccrd", InversedCrd<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcexsec", InversedExsec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcexcsc", InversedExcsc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcversin", InversedVersin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccoversin", InversedCoversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcvercos", InversedVercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccovercos", InversedCovercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("archaversin", InversedHaversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("archacoversin", InversedHacoversin<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("archavercos", InversedHavercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("archacovercos", InversedHacovercos<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcsinh", std::asinh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccosh", std::acosh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arctanh", std::atanh, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccoth", InversedHyperbolicCot<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arcsech", InversedHyperbolicSec<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("arccsch", InversedHyperbolicCsc<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("harmonicNumber", HarmonicNumber<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("bernoulliNumber", BernoulliNumber<num_t>, 1, args[0]->ToNumber().at(0), false),
    CreateBuiltinFunction("negativeBernoulliNumber", BernoulliNumber<num_t>, 1, args[0]->ToNumber().at(0), true),
    CreateBuiltinFunction("favardConstants", FavardConstants<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("champernowneConstants", ChampernowneConstants<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("berahaConstants", BerahaConstants<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("hyperHarmonicNumber", HyperHarmonicNumber<num_t>, 2, args[0]->ToNumber().at(0), args[1]->ToNumber().at(0)),
    CreateBuiltinFunction("gregoryNumber", GregoryNumber<num_t>, 1, args[0]->ToNumber().at(0)),
    CreateBuiltinFunction("metallicMean", MetallicMean<num_t>, 1, args[0]->ToNumber().at(0)),
    #undef CreateBuiltinFunction
};

#endif