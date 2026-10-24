#include "Tensor.hpp"
#include "TitForTat.hpp"
#include "TriggerStrategy.hpp"
#include "ConstantStrategy.hpp"
#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Interfaces/FunctionT.hpp>
#include <SingleTypePair.hpp>
#include <Logger.hpp>
#include <iostream>

Tensor<ssize_t> PrisonersDilemmaPayoffMatrix(const ssize_t& cooperationPayoff, const ssize_t& punishmentPayoff, const ssize_t& temptationPayoff, const ssize_t& suckersPayoff) {
    Tensor<ssize_t> ret = Tensor<ssize_t>(MathLib::VariadicSequence<size_t, 2, 2, 2>());
    ret.At(MathLib::VariadicSequence<size_t, 0, 0, 0>()) = ret.At(MathLib::VariadicSequence<size_t, 0, 0, 1>()) = cooperationPayoff;
    ret.At(MathLib::VariadicSequence<size_t, 1, 0, 0>()) = ret.At(MathLib::VariadicSequence<size_t, 0, 1, 1>()) = temptationPayoff;
    ret.At(MathLib::VariadicSequence<size_t, 1, 0, 1>()) = ret.At(MathLib::VariadicSequence<size_t, 0, 1, 0>()) = suckersPayoff;
    ret.At(MathLib::VariadicSequence<size_t, 1, 1, 0>()) = ret.At(MathLib::VariadicSequence<size_t, 1, 1, 1>()) = punishmentPayoff;
    return ret;
}
struct Human : Player {
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) override {
        if (prevEnemyMove != SIZE_MAX) {
            LogString("Previous enemy move: ");
            LogString(MathLib::ToString(prevEnemyMove, 10));
            LogChar('\n');
        }
        LogString("Your score: ");
        LogString(MathLib::ToString(score, 10));
        LogString("\n> ");
        ssize_t ret;
        std::cin >> ret;
        return ret < 0 ? SIZE_MAX : ret;
    }
};
void Main(int, char**, MathLib::FileSystem&) {
    const size_t turns = 5;
    const Tensor<ssize_t> payoffs = PrisonersDilemmaPayoffMatrix(1, 0, 2, -1);
    TitForTat player1 = TitForTat(0);

    ConstantStrategy constant0 = ConstantStrategy(0);
    ConstantStrategy constant1 = ConstantStrategy(1);
    const auto switchCondition = MathLib::MakeFunctionT<bool, bool, ssize_t, size_t>([](bool strategy, ssize_t, size_t prevEnemyMove) -> bool {
        return strategy || prevEnemyMove == 1;
    });
    TriggerStrategy player2 = TriggerStrategy(false, constant0, constant1, switchCondition);

    MathLib::Array<size_t> moves = MathLib::MakeArray<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX);
    LogString("Payoffs: ");
    LogString(payoffs.ToString());
    LogChar('\n');
    ssize_t score1 = 0;
    ssize_t score2 = 0;
    for (size_t i = 0; i < turns; i++) {
        const size_t tmp = moves.AtUnsafe(0);
        moves.AtUnsafe(0) = player1.GetMove(score1, moves.AtUnsafe(1));
        if (moves.AtUnsafe(0) == SIZE_MAX) break;
        moves.AtUnsafe(1) = player2.GetMove(score2, tmp);
        if (moves.AtUnsafe(1) == SIZE_MAX) break;
        moves.AtUnsafe(2) = 0;
        score1 += payoffs.At(moves);
        moves.AtUnsafe(2) = 1;
        score2 += payoffs.At(moves);
    }
    LogString("Player 1 score: ");
    LogString(MathLib::ToString(score1, 10));
    LogString("\nPlayer 2 score: ");
    LogString(MathLib::ToString(score2, 10));
    LogChar('\n');
}