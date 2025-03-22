#include "Operation.hpp"

Operation::Operation(const MathLib::Sequence<char>& name, Func* func) : name(MathLib::CollectionToString(name)), func(func) {}
Operation::~Operation(void) {
    delete func;
}