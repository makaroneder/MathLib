#include "ConstantStrategy.hpp"

ConstantStrategy::ConstantStrategy(size_t move) : move(move) {}
size_t ConstantStrategy::GetMove(ssize_t, size_t) {
    return move;
}