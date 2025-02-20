#include "String.hpp"
#include "Graph/Tree.hpp"

namespace MathLib {
    num_t eps = 1e-3;
    bool benchmark = false;
    Tree<num_t> traceData;
    Array<Tree<num_t>*> traces;
    size_t currentTrace;

    void StartBenchmarking(void) {
        traceData = Tree<num_t>("Root", GetTime());
        traces = MakeArray<Tree<num_t>*>(&traceData);
        currentTrace = 0;
        benchmark = true;
    }
    bool BenchmarkStart(const char* function) {
        if (!benchmark) return true;
        benchmark = false;
        if (!traces.At(currentTrace)->Add(Tree<num_t>(function, GetTime())) || !traces.Add(&traces.At(currentTrace)->children.At(traces.At(currentTrace)->children.GetSize() - 1))) {
            benchmark = true;
            return false;
        }
        currentTrace++;
        benchmark = true;
        return true;
    }
    bool BenchmarkEnd(void) {
        if (!benchmark) return true;
        benchmark = false;
        traces.At(currentTrace)->data = GetTime() - traces.At(currentTrace)->data;
        currentTrace--;
        traces = Array<Tree<num_t>*>(traces.GetValue(), traces.GetSize() - 1);
        benchmark = true;
        return true;
    }
    Tree<num_t> StopBenchmarking(void) {
        benchmark = false;
        traceData.data = GetTime() - traceData.data;
        return traceData;
    }
}
MathLib::String operator""_M(const char* str, size_t size) {
    (void)size;
    return MathLib::String(str);
}
MathLib::String operator""_M(char chr) {
    return MathLib::String(chr);
}