#include "Circuit.hpp"
#include <Interfaces/Sequence/FunctionSequence.hpp>
#include <FunctionT.hpp>
#include <String.hpp>

Circuit::Circuit(void) {}
Circuit::~Circuit(void) {
    if (!network.ResetNodes(MathLib::MakeFunctionT<void, Gate*&>([](Gate*& element) -> void {
        delete element;
    }))) MathLib::Panic("Failed to destroy circuit gates");
}
bool Circuit::SetOutput(size_t out) {
    if (out >= network.GetNodeCount()) return false;
    output = out;
    return true;
}
size_t Circuit::Add(Gate* element) {
    return element ? network.Add(element) : SIZE_MAX;
}
bool Circuit::Add(const MathLib::Edge& edge, size_t bit) {
    const size_t size = network.GetNodeCount();
    return (edge.from < size || edge.from == SIZE_MAX) && edge.to < size && network.Add(edge, bit);
}
size_t Circuit::Add(Gate* element, const MathLib::Sequence<CircuitConnection>& inputs) {
    const size_t ret = Add(element);
    if (ret == SIZE_MAX) return ret;
    const size_t size = inputs.GetSize();
    for (size_t i = 0; i < size; i++) {
        const CircuitConnection connection = inputs.At(i);
        if (!Add(MathLib::Edge(connection.element, ret), connection.bit)) return SIZE_MAX;
    }
    return ret;
}
MathLib::Bitmap Circuit::Update(const MathLib::Sequence<bool>& inputData) {
    MathLib::Dictionary<size_t, MathLib::Bitmap> cache = MathLib::Dictionary<size_t, MathLib::Bitmap>(
        MathLib::MakeArray<MathLib::DictionaryElement<size_t, MathLib::Bitmap>>(
            MathLib::DictionaryElement<size_t, MathLib::Bitmap>(SIZE_MAX, MathLib::Bitmap(inputData))
        )
    );
    return Update(output, cache);
}
MathLib::Bitmap Circuit::Update(size_t element, MathLib::Dictionary<size_t, MathLib::Bitmap>& cache) {
    const MathLib::Expected<MathLib::Bitmap> ret = cache.Get(element);
    if (ret.HasValue()) return ret.Get();
    const MathLib::Array<MathLib::NetworkElement> inputElements = network.GetConnectionsAttributes(element, true);
    const MathLib::Bitmap result = network.GetNode(element)->Update(MathLib::FunctionSequence<bool>(MathLib::MakeFunctionT<bool, size_t>([this, &inputElements, &cache](size_t index) -> bool {
        const MathLib::NetworkElement connection = inputElements.At(index);
        return Update(connection.node, cache).At(network.GetLink(connection.link));
    }), inputElements.GetSize()));
    return cache.Add(MathLib::DictionaryElement<size_t, MathLib::Bitmap>(element, result)) ? result : MathLib::Bitmap();
}
MathLib::String Circuit::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = '\t'_M + padding;
    return "{\n"_M + network.ToString(padd) + '\n' + padd + "Output: " + MathLib::ToString(output, 10) + '\n' + padding + '}';
}