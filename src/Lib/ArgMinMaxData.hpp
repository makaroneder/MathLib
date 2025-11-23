#ifndef MathLib_ArgMinMaxData_H
#define MathLib_ArgMinMaxData_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct ArgMinMaxData {
        Array<Arg> args;
        Ret ret;

        ArgMinMaxData(void) : args(), ret() {}
        ArgMinMaxData(const Ret& ret, const Sequence<Arg>& args) : args(CollectionToArray<Arg>(args)), ret(ret) {}
    };
}

#endif