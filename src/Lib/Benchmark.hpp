#ifndef MathLib_Benchmark_H
#define MathLib_Benchmark_H
#define StartBenchmark if (!MathLib::IsCompileTimeEvaluated() && !MathLib::BenchmarkStart(__PRETTY_FUNCTION__)) MathLib::Panic("Failed to start benchmark");
#define EndBenchmark if (!MathLib::IsCompileTimeEvaluated() && !MathLib::BenchmarkEnd()) MathLib::Panic("Failed to end benchmark");
#define ReturnFromBenchmark(ret) {  \
    EndBenchmark                    \
    return ret;                     \
}
#define StartAndReturnFromBenchmark(ret)    \
    StartBenchmark                          \
    ReturnFromBenchmark(ret)
#define EmptyBenchmark  \
    StartBenchmark      \
    EndBenchmark

namespace MathLib {
    [[noreturn]] void Panic(const char* str);
    [[nodiscard]] constexpr bool IsCompileTimeEvaluated(void) {
        #ifndef __MINGW32__
        return __builtin_is_constant_evaluated();
        #endif
        return true;
    }
    [[nodiscard]] bool BenchmarkStart(const char* function);
    [[nodiscard]] bool BenchmarkEnd(void);
}

#endif