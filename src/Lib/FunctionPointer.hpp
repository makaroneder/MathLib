#ifndef FunctionPointer_H
#define FunctionPointer_H
#include "Function.hpp"

template <typename Ret, typename... Args>
struct FunctionPointer : Function<Ret, Args...> {
    using RawFunction = Ret (*)(const void*, Args...);
    FunctionPointer(void) {}
    FunctionPointer(void* data, RawFunction function) : Function<Ret, Args...>(data), function(function) {}
    RawFunction GetFunction(void) const {
        return function;
    }
    virtual Ret Invoke(const void* data, Args... args) const override {
        return function(data, args...);
    }

    private:
    RawFunction function;
};

#endif