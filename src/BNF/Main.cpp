#include "BNF.hpp"
#include <Cryptography/Compressor/BytePairEncoding/BytePairEncoding.hpp>
#include <iostream>

template <typename T>
T Evaluate(const MathLib::Tree<MathLib::String>& tree) {
    if (tree.name == "expr") {
        if (tree.data == "add") return Evaluate<T>(tree.children.At(1)) + Evaluate<T>(tree.children.At(2));
        if (tree.data == "sub") return Evaluate<T>(tree.children.At(1)) - Evaluate<T>(tree.children.At(2));
        if (tree.data == "mul") return Evaluate<T>(tree.children.At(1)) * Evaluate<T>(tree.children.At(2));
        if (tree.data == "div") return Evaluate<T>(tree.children.At(1)) / Evaluate<T>(tree.children.At(2));
        if (tree.data == "sqrt") return MathLib::Sqrt(Evaluate<T>(tree.children.At(1)));
        if (tree.data.IsEmpty()) {
            if (tree.children.At(1).name == "constant") {
                MathLib::String tmp;
                for (uint8_t i = 0; i < tree.children.At(1).children.GetSize(); i++)
                    tmp += tree.children.At(1).children.At(i).data;
                return MathLib::StringToNumber(tmp);
            }
        }
    }
    return MathLib::nan;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const BNF bnf = BNF(MathLib::MakeArray<BNFVariable>(
            BNFVariable("expr"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue("whitespace"_M, false),
                BNFValue(MathLib::MakeArray<BNFValue>(
                    BNFValue("constant"_M, false),
                    BNFValue(MathLib::MakeArray<BNFValue>(
                        BNFValue(MathLib::MakeArray<BNFValue>(
                            BNFValue("sqrt"_M, true)
                        ), false),
                        BNFValue("expr"_M, false)
                    ), true),
                    BNFValue(MathLib::MakeArray<BNFValue>(
                        BNFValue(MathLib::MakeArray<BNFValue>(
                            BNFValue("add"_M, true),
                            BNFValue("sub"_M, true),
                            BNFValue("mul"_M, true),
                            BNFValue("div"_M, true)
                        ), false),
                        BNFValue("expr"_M, false),
                        BNFValue("expr"_M, false)
                    ), true)
                ), false)
            ), true)),
            BNFVariable("constant"_M, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue("digit"_M, false),
                BNFValue(BNFValue::Type::Optional, BNFValue(MathLib::MakeArray<BNFValue>(
                    BNFValue('.'_M, true),
                    BNFValue("digit"_M, false)
                ), true))
            ), true)),
            BNFVariable("digit"_M, BNFValue(BNFValue::Type::KleenePlus, BNFValue(MathLib::Interval<char>('0', '9')))),
            BNFVariable("whitespace"_M, BNFValue(BNFValue::Type::KleeneStar, BNFValue(MathLib::MakeArray<BNFValue>(
                BNFValue(' '_M, true),
                BNFValue('\t'_M, true),
                BNFValue('\n'_M, true),
                BNFValue('\r'_M, true)
            ), false)))
        ));
        std::cout << bnf << std::endl;
        const MathLib::String expr = "add 1 sqrt 2.0";
        const MathLib::Tree<MathLib::String> ast = bnf.BuildAST(expr, "expr"_M);
        std::cout << ast << std::endl;
        std::cout << expr << std::endl;
        std::cout << Evaluate<MathLib::num_t>(ast) << std::endl;

        const MathLib::String data = "The original BPE algorithm operates by iteratively replacing the most common contiguous sequences of characters in a target text with unused 'placeholder' bytes. The iteration ends when no sequences can be found, leaving the target text effectively compressed. Decompression can be performed by reversing this process, querying known placeholder terms against their corresponding denoted sequence, using a lookup table. In the original paper, this lookup table is encoded and stored alongside the compressed text.";
        MathLib::num_t start = MathLib::GetTime();
        MathLib::BytePairEncoding bpe;
        std::cout << bpe.CheckEncryption<char>(data, MathLib::Array<uint64_t>()) << std::endl;
        std::cout << bpe.TestEncryption<char>(data, MathLib::Array<uint64_t>()) << std::endl;
        std::cout << MathLib::GetTime() - start << std::endl;
        // TODO: Train LLN on byte pair encoded data
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}