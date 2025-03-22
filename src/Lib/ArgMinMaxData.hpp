#ifndef MathLib_ArgMinMaxData_H
#define MathLib_ArgMinMaxData_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    struct ArgMinMaxData {
        Ret ret;
        Array<Arg> args;

        ArgMinMaxData(void) : ret(), args() {
            EmptyBenchmark
        }
        ArgMinMaxData(const Ret& ret, const Sequence<Arg>& args) : ret(ret), args(CollectionToArray<Arg>(args)) {
            EmptyBenchmark
        }
    };
}

#endif