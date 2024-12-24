#include "Typedefs.hpp"
#include "Logger.hpp"

namespace MathLib {
    num_t eps = 1e-3;
    bool benchmark = false;
    size_t benchmarkIndex = 0;

    bool BenchmarkStart(const char* function) {
        if (!benchmark) return true;
        benchmark = false;
        bool ret = LogString(ToString(benchmarkIndex, 10)) && LogString(" [") && LogString(function) && LogString("]\n");
        benchmark = true;
        benchmarkIndex++;
        return ret;
    }
    bool BenchmarkEnd(const char* function, num_t time) {
        if (!benchmark) return true;
        benchmark = false;
        const num_t tmp = GetTime();
        benchmarkIndex--;
        bool ret = LogString(ToString(benchmarkIndex, 10)) && LogString(" [") && LogString(function) && LogString("]: ") && LogString(ToString(tmp - time)) && LogChar('\n');
        benchmark = true;
        return ret;
    }
}