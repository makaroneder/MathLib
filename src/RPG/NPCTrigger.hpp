#ifndef NPCTrigger_H
#define NPCTrigger_H
#include <Interfaces/Allocatable.hpp>
#include <stdint.h>

struct NPCTrigger : MathLib::Allocatable {
    enum class Type {
        None,
        Fight,
        Ending,
    } type;
    size_t dialog;
    size_t arg;
    NPCTrigger(Type type, size_t dialog, size_t arg = SIZE_MAX);
};

#endif