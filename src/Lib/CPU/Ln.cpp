#include "Ln.hpp"
#include "FPU/Ln.hpp"
#include "FPU/FPU.hpp"
#include "../Host.hpp"
#include "../Math/Constants.hpp"

namespace MathLib {
    LnFunction lnFunction = &DefaultLn;
    num_t DefaultLn(num_t x) {
        #ifdef __x86_64__
        if (waitForFPU) return GenericLn(x);
        CheckFPU();
        lnFunction = fpu ? &FPULn : &GenericLn;
        return lnFunction(x);
        #endif
        lnFunction = &GenericLn;
        return lnFunction(x);
    }
    num_t GenericLn(num_t x) {
        if (IsNaN(x) || x <= 0) return nan;
        if (IsInf(x)) return infinity;
        num_t ret = 0;
        while (x >= 2) {
            x /= 2;
            ret += ln2;
        }
        int8_t sign = 1;
        num_t mult = x - 1;
        for (size_t i = 1; true; i++) {
            const num_t tmp = sign * mult / i;
            ret += tmp;
            if (FloatsEqual<num_t>(tmp, 0)) break;
            sign = -sign;
            mult *= x - 1;
        }
        return ret;
    }
}