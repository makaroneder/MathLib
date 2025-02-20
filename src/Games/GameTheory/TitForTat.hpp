#ifndef TitForTat_H
#define TitForTat_H
#include "GameStrategy.hpp"

struct TitForTat : GameStrategy {
    TitForTat(size_t firstMove);
    [[nodiscard]] virtual size_t GetMove(size_t prevEnemyMove) override;

    private:
    size_t firstMove;
};

#endif