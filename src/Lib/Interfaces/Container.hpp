#ifndef MathLib_Interfaces_Container_H
#define MathLib_Interfaces_Container_H

namespace MathLib {
    template <typename T>
    struct Container {
        [[nodiscard]] virtual bool Contains(const T& value) const = 0;
    };
}

#endif