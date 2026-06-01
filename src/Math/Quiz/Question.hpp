#ifndef Question_H
#define Question_H
#include <Interfaces/Printable.hpp>

struct Question : MathLib::Printable {
    Question(const MathLib::Sequence<char>& equation);
    [[nodiscard]] bool IsSolution(MathLib::num_t sol, MathLib::num_t eps) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    MathLib::String equation;
    MathLib::num_t solution;
};

#endif