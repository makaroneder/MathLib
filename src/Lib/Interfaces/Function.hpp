#ifndef Interfaces_Function_H
#define Interfaces_Function_H
#include "Allocatable.hpp"

template <typename Ret, typename... Args>
struct Function : Allocatable {
    Function(void) {}
    Function(void* data) : data(data) {}
    const void* GetData(void) const {
        return data;
    }
    Ret operator()(Args... args) const {
        return Invoke(data, args...);
    }
    virtual Ret Invoke(const void* data, Args... args) const = 0;

    private:
    void* data;
};

#endif