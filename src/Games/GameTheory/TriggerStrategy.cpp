#include "TriggerStrategy.hpp"

TriggerStrategy::TriggerStrategy(bool strategy, Player& strategy1, Player& strategy2, const MathLib::Function<bool, bool, ssize_t, size_t>& switchCondition) : strategy1(strategy1), strategy2(strategy2), switchCondition(switchCondition), strategy(strategy) {}
size_t TriggerStrategy::GetMove(ssize_t score, size_t prevEnemyMove) {
    strategy = switchCondition(strategy, score, prevEnemyMove);
    return (strategy ? strategy2 : strategy1).GetMove(score, prevEnemyMove);
}