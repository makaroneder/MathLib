#include "ApproxEBySequence.hpp"
#include "../Thread.hpp"
#include <FunctionPointer.hpp>

void ApproxEBySequenceUnit(NaturalNumber start, NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret) {
    for (NaturalNumber i = start; i < end;) {
        ret->ret += RationalNumber(NaturalNumber::FromT<uint8_t>(1), ret->factorial);
        ret->factorial *= ++i;
    }
}
void ApproxEBySequenceMultiUnit(NaturalNumber start, NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret) {
    const size_t threadCount = std::thread::hardware_concurrency();
    const NaturalNumber dt = (end - start).Get("End is greater than start") / NaturalNumber::FromT<size_t>(threadCount);
    Thread<NaturalNumber, NaturalNumber, ApproxEBySequenceUnitReturnValue*> threads[threadCount];
    ApproxEBySequenceUnitReturnValue values[threadCount];
    NaturalNumber i = start;
    const MathLib::FunctionPointer<void, NaturalNumber, NaturalNumber, ApproxEBySequenceUnitReturnValue*> func = MathLib::FunctionPointer<void, NaturalNumber, NaturalNumber, ApproxEBySequenceUnitReturnValue*>(&ApproxEBySequenceUnit);
    for (size_t thread = 0; thread < threadCount; thread++) {
        const NaturalNumber tmp = i;
        i += dt;
        threads[thread].Run(func, tmp, i, &values[thread]);
    }
    for (size_t thread = 0; thread < threadCount; thread++) {
        threads[thread].Join();
        ret->ret += values[thread].ret * RationalNumber(NaturalNumber::FromT<uint8_t>(1), ret->factorial);
        ret->factorial *= values[thread].factorial;
    }
}
RationalNumber ApproxEBySequence(NaturalNumber limit) {
    ApproxEBySequenceUnitReturnValue ret;
    ApproxEBySequenceMultiUnit(NaturalNumber(), limit, &ret);
    return ret.ret;
}