#ifndef ConstantStrategy_H
#define ConstantStrategy_H
#include "GameStrategy.hpp"

struct ConstantStrategy : GameStrategy {
    ConstantStrategy(size_t move);
    [[nodiscard]] virtual size_t GetMove(size_t) override;

    private:
    size_t move;
};

#endif