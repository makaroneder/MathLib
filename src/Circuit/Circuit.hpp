#ifndef Circuit_H
#define Circuit_H
#include "Gate/Gate.hpp"
#include "CircuitConnection.hpp"
#include <Interfaces/Dictionary.hpp>
#include <Graph/Network.hpp>
#include <Bitmap.hpp>

struct Circuit : Gate, MathLib::Printable {
    Circuit(void);
    virtual ~Circuit(void) override;
    [[nodiscard]] bool SetOutput(size_t out);
    [[nodiscard]] size_t Add(Gate* element);
    [[nodiscard]] bool Add(const MathLib::Edge& edge, size_t bit);
    [[nodiscard]] size_t Add(Gate* element, const MathLib::Sequence<CircuitConnection>& inputs);
    [[nodiscard]] virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& inputData) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    [[nodiscard]] MathLib::Bitmap Update(size_t element, MathLib::Dictionary<size_t, MathLib::Bitmap>& cache);

    MathLib::Network<Gate*, size_t> network;
    size_t output;
};

#endif