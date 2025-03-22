#include "FormalSystem.hpp"
#include <String.hpp>
#include <iostream>

void TestProof(FormalSystem& system, const Theory& proof) {
    std::cout << "Proof: " << proof << std::endl;
    std::cout << "Proved: " << MathLib::BoolToString(system.Prove(proof)) << std::endl;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        FormalSystem system = FormalSystem(MathLib::MakeArray<Operation*>(
            new Operation("equal"_M, MathLib::AllocFunctionT<MathLib::Array<Formula>, const MathLib::Sequence<Formula>&>(nullptr, [] (const void*, const MathLib::Sequence<Formula>& args) -> MathLib::Array<Formula> {
                if (args.GetSize() != 2) return MathLib::Array<Formula>();
                Formula a = args.At(0);
                Formula b = args.At(1);
                MathLib::Array<Formula> ret = MathLib::MakeArray(Formula("equal"_M, b, a));
                if (a.type == b.type) {
                    if (a == b) {
                        if (!ret.Add(true)) return MathLib::Array<Formula>();
                    }
                    else if (a.type == Formula::Type::Constant && !ret.Add(a.value == b.value)) return MathLib::Array<Formula>();
                }
                return ret;
            })),
            new Operation("add"_M, MathLib::AllocFunctionT<MathLib::Array<Formula>, const MathLib::Sequence<Formula>&>(nullptr, [] (const void*, const MathLib::Sequence<Formula>& args) -> MathLib::Array<Formula> {
                if (args.GetSize() != 2) return MathLib::Array<Formula>();
                Formula a = args.At(0);
                Formula b = args.At(1);
                MathLib::Array<Formula> ret = MathLib::MakeArray(Formula("add"_M, b, a));
                if (a.type != Formula::Type::Constant) MathLib::Swap<Formula>(a, b);
                if (a.type != Formula::Type::Constant) return ret;
                if (a.value == "0") {
                    if (!ret.Add(b)) return MathLib::Array<Formula>();
                }
                else if (b.type == Formula::Type::Constant && !ret.Add(Formula(true, MathLib::ToString(MathLib::StringToNumber(a.value) + MathLib::StringToNumber(b.value))))) return MathLib::Array<Formula>();
                return ret;
            }))
        ), MathLib::MakeArray<Theory>(
            Theory("Reduce common"_M, MathLib::MakeArray<MathLib::String>('a'_M, 'b'_M, 'c'_M), MathLib::MakeArray<Formula>(
                Formula("equal"_M,
                    Formula("add"_M,
                        Formula(false, 'a'_M),
                        Formula(false, 'b'_M)
                    ),
                    Formula("add"_M,
                        Formula(false, 'c'_M),
                        Formula(false, 'b'_M)
                    )
                ),
                Formula("equal"_M,
                    Formula(false, 'a'_M),
                    Formula(false, 'c'_M)
                )
            ))
        ));
        TestProof(system, Theory('1'_M, MathLib::MakeArray<Formula>(
            Formula("equal"_M,
                Formula("add"_M,
                    Formula(true, '2'_M),
                    Formula(true, '2'_M)
                ),
                Formula(true, '4'_M)
            ),
            Formula("equal"_M,
                Formula(true, '4'_M),
                Formula(true, '4'_M)
            ),
            Formula(true)
        )));
        TestProof(system, Theory('2'_M, MathLib::MakeArray<MathLib::String>('a'_M, 'b'_M), MathLib::MakeArray<Formula>(
            Formula("equal"_M,
                Formula("add"_M,
                    Formula(false, 'a'_M),
                    Formula(false, 'b'_M)
                ),
                Formula("add"_M,
                    Formula(false, 'b'_M),
                    Formula(false, 'a'_M)
                )
            ),
            Formula("ApplyTheorem"_M,
                Formula("Reduce common"_M, MathLib::MakeArray<Formula>(
                    Formula('a'_M, Formula(false, 'a'_M)),
                    Formula('b'_M, Formula(false, 'b'_M)),
                    Formula('c'_M, Formula(false, 'a'_M))
                )),
                Formula("equal"_M,
                    Formula(false, 'a'_M),
                    Formula(false, 'a'_M)
                )
            ),
            Formula(true)
        )));
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}