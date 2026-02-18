#ifndef Player_H
#define Player_H
#include <Typedefs.hpp>

struct Player {
    [[nodiscard]] virtual size_t GetMove(ssize_t score, size_t prevEnemyMove) = 0;
};

#endif