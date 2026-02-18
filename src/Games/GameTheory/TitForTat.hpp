#ifndef TitForTat_H
#define TitForTat_H
#include "Player.hpp"

struct TitForTat : Player {
    TitForTat(size_t firstMove);
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) override;

    private:
    size_t firstMove;
};

#endif