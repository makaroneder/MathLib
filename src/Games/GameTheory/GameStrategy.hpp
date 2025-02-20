#ifndef GameStrategy_H
#define GameStrategy_H
#include <Typedefs.hpp>

struct GameStrategy : MathLib::Allocatable {
    [[nodiscard]] virtual size_t GetMove(size_t prevEnemyMove) = 0;
};

#endif