#include "ApproxEBySequence.hpp"
#include "../Thread.hpp"
#include <FunctionPointer.hpp>

void ApproxEBySequenceUnit(MathLib::NaturalNumber start, MathLib::NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret) {
    for (MathLib::NaturalNumber i = start; i < end;) {
        ret->ret += MathLib::RationalNumber(MathLib::NaturalNumber::FromT<uint8_t>(1), ret->factorial);
        ret->factorial *= ++i;
    }
}
void ApproxEBySequenceMultiUnit(MathLib::NaturalNumber start, MathLib::NaturalNumber end, ApproxEBySequenceUnitReturnValue* ret) {
    const size_t threadCount = std::thread::hardware_concurrency();
    const MathLib::NaturalNumber dt = (end - start).Get("End is greater than start") / MathLib::NaturalNumber::FromT<size_t>(threadCount);
    Thread<MathLib::NaturalNumber, MathLib::NaturalNumber, ApproxEBySequenceUnitReturnValue*> threads[threadCount];
    ApproxEBySequenceUnitReturnValue values[threadCount];
    MathLib::NaturalNumber i = start;
    const MathLib::FunctionPointer<void, MathLib::NaturalNumber, MathLib::NaturalNumber, ApproxEBySequenceUnitReturnValue*> func = MathLib::FunctionPointer<void, MathLib::NaturalNumber, MathLib::NaturalNumber, ApproxEBySequenceUnitReturnValue*>(&ApproxEBySequenceUnit);
    for (size_t thread = 0; thread < threadCount; thread++) {
        const MathLib::NaturalNumber tmp = i;
        i += dt;
        threads[thread].Run(func, tmp, i, &values[thread]);
    }
    for (size_t thread = 0; thread < threadCount; thread++) {
        threads[thread].Join();
        ret->ret += values[thread].ret * MathLib::RationalNumber(MathLib::NaturalNumber::FromT<uint8_t>(1), ret->factorial);
        ret->factorial *= values[thread].factorial;
    }
}
MathLib::RationalNumber ApproxEBySequence(MathLib::NaturalNumber limit) {
    ApproxEBySequenceUnitReturnValue ret;
    ApproxEBySequenceMultiUnit(MathLib::NaturalNumber(), limit, &ret);
    return ret.ret;
}