#include "TitForTat.hpp"

TitForTat::TitForTat(size_t firstMove) : firstMove(firstMove) {}
size_t TitForTat::GetMove(ssize_t, size_t prevEnemyMove) {
    return prevEnemyMove == SIZE_MAX ? firstMove : prevEnemyMove;
}