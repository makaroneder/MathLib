#ifndef TriggerStrategy_H
#define TriggerStrategy_H
#include "Player.hpp"

struct TriggerStrategy : Player {
    TriggerStrategy(bool strategy, Player& strategy1, Player& strategy2, const MathLib::Function<bool, bool, ssize_t, size_t>& switchCondition);
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) override;

    private:
    Player& strategy1;
    Player& strategy2;
    const MathLib::Function<bool, bool, ssize_t, size_t>& switchCondition;
    bool strategy;
};

#endif