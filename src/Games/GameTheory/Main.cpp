#include "Tensor.hpp"
#include "TitForTat.hpp"
#include "ConstantStrategy.hpp"
#include <Interfaces/Sequence/VariadicSequence.hpp>
#include <SingleTypePair.hpp>
#include <FunctionT.hpp>
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
        if (prevEnemyMove != SIZE_MAX)
            std::cout << "Previous enemy move: " << prevEnemyMove << std::endl;
        std::cout << "Your score: " << score << std::endl;
        std::cout << "> ";
        ssize_t ret;
        std::cin >> ret;
        return ret < 0 ? SIZE_MAX : ret;
    }
};
struct TriggerStrategy : Player {
    TriggerStrategy(bool strategy, Player& strategy1, Player& strategy2, const MathLib::Function<bool, bool, ssize_t, size_t>& switchCondition) : strategy1(strategy1), strategy2(strategy2), switchCondition(switchCondition), strategy(strategy) {}
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) override {
        strategy = switchCondition(strategy, score, prevEnemyMove);
        return (strategy ? strategy2 : strategy1).GetMove(score, prevEnemyMove);
    }

    private:
    Player& strategy1;
    Player& strategy2;
    const MathLib::Function<bool, bool, ssize_t, size_t>& switchCondition;
    bool strategy;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const size_t turns = 5;
        const Tensor<ssize_t> payoffs = PrisonersDilemmaPayoffMatrix(1, 0, 2, -1);
        TitForTat p1 = TitForTat(0);

        ConstantStrategy constant0 = ConstantStrategy(0);
        ConstantStrategy constant1 = ConstantStrategy(1);
        const auto switchCond = MathLib::MakeFunctionT<bool, bool, ssize_t, size_t>([](bool strategy, ssize_t, size_t prevEnemyMove) -> bool {
            return strategy || prevEnemyMove == 1;
        });
        TriggerStrategy p2 = TriggerStrategy(false, constant0, constant1, switchCond);

        MathLib::Array<size_t> moves = MathLib::MakeArray<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX);
        std::cout << "Payoffs: " << payoffs << std::endl;
        ssize_t score1 = 0;
        ssize_t score2 = 0;
        for (size_t i = 0; i < turns; i++) {
            const size_t tmp = moves.At(0);
            moves.At(0) = p1.GetMove(score1, moves.At(1));
            if (moves.At(0) == SIZE_MAX) break;
            moves.At(1) = p2.GetMove(score2, tmp);
            if (moves.At(1) == SIZE_MAX) break;
            moves.At(2) = 0;
            score1 += payoffs.At(moves);
            moves.At(2) = 1;
            score2 += payoffs.At(moves);
        }
        std::cout << "Player 1 score: " << score1 << std::endl;
        std::cout << "Player 2 score: " << score2 << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}