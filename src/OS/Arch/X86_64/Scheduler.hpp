#ifdef __x86_64__
#ifndef X86_Scheduler_H
#define X86_Scheduler_H
#include "Registers.hpp"
#include <FunctionPointer.hpp>

struct Task {
    enum class State {
        None,
        Running,
        Dead,
    } state;
    MathLib::FunctionPointer<void, size_t> entry;
    Registers regs;

    Task(void);
    Task(const MathLib::FunctionPointer<void, size_t>& entry);
    void Handle(Registers* prev, size_t index);
};

void Schedule(const void*, Registers* regs);

#endif
#endif