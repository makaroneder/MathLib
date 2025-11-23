#ifndef MathLib_Interfaces_Function_H
#define MathLib_Interfaces_Function_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename Ret, typename... Args>
    struct Function : Allocatable {
        [[nodiscard]] Ret operator()(Args... args) const {
            return Invoke(args...);
        }
        [[nodiscard]] virtual Ret Invoke(Args... args) const = 0;
    };
}

#endif