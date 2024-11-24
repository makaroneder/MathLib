#include <Host.hpp>

struct AtExitFunction : MathLib::Allocatable {
    using Function = void (*)(void*);
    Function function;
    void* arg;
    void* dso;

    AtExitFunction(Function function = nullptr, void* arg = nullptr, void* dso = nullptr);
    bool operator==(void* func) const;
    void operator()(void);
};
AtExitFunction atExitFunctions[128];
size_t atExitFunctionCount = 0;

AtExitFunction::AtExitFunction(Function function, void* arg, void* dso) : function(function), arg(arg), dso(dso) {}
bool AtExitFunction::operator==(void* func) const {
    return function == func || !func;
}
void AtExitFunction::operator()(void) {
    if (function) function(arg);
    function = nullptr;
}
extern "C" int __cxa_atexit(AtExitFunction::Function function, void* arg, void* dso) {
    if (atExitFunctionCount >= SizeOfArray(atExitFunctions)) {return -1;};
    atExitFunctions[atExitFunctionCount++] = AtExitFunction(function, arg, dso);
    return 0;
}
extern "C" void __cxa_finalize(void* function) {
    size_t i = atExitFunctionCount;
    while (i--)
        if (atExitFunctions[i] == function) atExitFunctions[i]();
}
uintptr_t __stack_chk_guard = 0x2137699642012345;
extern "C" [[noreturn]] void __stack_chk_fail(void) {
	MathLib::Panic("Stack smashing detected");
}
extern "C" void __cxa_pure_virtual(void) {
    MathLib::Panic("Pure virtual function called");
}