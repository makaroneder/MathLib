#include "ConstantStrategy.hpp"

ConstantStrategy::ConstantStrategy(size_t move) : move(move) {}
size_t ConstantStrategy::GetMove(size_t) {
    return move;
}