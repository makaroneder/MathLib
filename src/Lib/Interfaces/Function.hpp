#ifndef MathLib_Interfaces_Function_H
#define MathLib_Interfaces_Function_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct Function : Allocatable {
        void* data;

        Function(void) : data(nullptr) {}
        Function(void* data) : data(data) {}
        [[nodiscard]] Ret operator()(Args... args) const {
            return Invoke(data, args...);
        }
        [[nodiscard]] virtual Ret Invoke(const void* data, Args... args) const = 0;
    };
}

#endif