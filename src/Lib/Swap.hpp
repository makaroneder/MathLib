#ifndef MathLib_Swap_H
#define MathLib_Swap_H

namespace MathLib {
    template <typename T>
    void Swap(T& a, T& b) {
        const T tmp = a;
        a = b;
        b = tmp;
    }
}

#endif
