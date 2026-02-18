#ifndef ConstantStrategy_H
#define ConstantStrategy_H
#include "Player.hpp"

struct ConstantStrategy : Player {
    ConstantStrategy(size_t move);
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) override;

    private:
    size_t move;
};

#endif