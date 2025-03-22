#include "BNF.hpp"
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        BNF bnf = BNF(MathLib::MakeArray<BNFVariable>(
            BNFVariable("expr"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue("add"_M, false),
                BNFValue("constant"_M, false)
            ), false)),
            BNFVariable("add"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue("expr"_M, false),
                BNFValue('+'_M, true),
                BNFValue("expr"_M, false)
            ), true)),
            BNFVariable("constant"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue("nonZeroConstant"_M, false),
                BNFValue('0'_M, true)
            ), false)),
            BNFVariable("nonZeroConstant"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue(MathLib::MakeArray<BNFValue>(
                    BNFValue('1'_M, true),
                    BNFValue('2'_M, true),
                    BNFValue('3'_M, true),
                    BNFValue('4'_M, true),
                    BNFValue('5'_M, true),
                    BNFValue('6'_M, true),
                    BNFValue('7'_M, true),
                    BNFValue('8'_M, true),
                    BNFValue('9'_M, true)
                ), false),
                BNFValue(MathLib::MakeArray<BNFValue>(
                    BNFValue("constant"_M, false),
                    BNFValue('.'_M, true)
                ), false)
            ), true))
        ));
        std::cout << bnf << std::endl;
        std::cout << bnf.Match('1'_M, "nonZeroConstant"_M) << std::endl;
        // TODO: Check if string can be constructed from this grammar
        // TODO: Compress strings using byte pair encoding (which would return string and BNF)
        // TODO: Train LLN on byte pair encoded data
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}