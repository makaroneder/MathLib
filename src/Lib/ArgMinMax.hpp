#ifndef MathLib_ArgMinMax_H
#define MathLib_ArgMinMax_H
#include "ArgMinMaxData.hpp"

namespace MathLib {
    template <typename Ret, typename Arg>
    ArgMinMaxData<Ret, Arg> ArgMinMax(const Sequence<Arg>& sequence, const Function<Ret, Arg>& func, bool max) {
        Ret ret = max ? -infinity : infinity;
        Array<Arg> args;
        const size_t size = sequence.GetSize();
        for (size_t i = 0; i < size; i++) {
            const Arg arg = sequence.At(i);
            const Ret tmp = func(arg);
            if (FloatsEqual<Ret>(ret, tmp)) {
                if (!args.Add(arg)) return (ArgMinMaxData<Ret, Arg>());
            }
            else if ((ret < tmp) == max) {
                ret = tmp;
                args = MakeArray<Arg>(arg);
            }
        }
        return (ArgMinMaxData<Ret, Arg>(ret, args));
    }
}

#endif