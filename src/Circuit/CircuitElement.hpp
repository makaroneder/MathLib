#ifndef CircuitElement_H
#define CircuitElement_H
#include "CircuitElementConnection.hpp"
#include <Bitmap.hpp>

struct CircuitElement : MathLib::Printable {
    CircuitElement(void);
    CircuitElement(const MathLib::Sequence<CircuitElementConnection>& children);
    [[nodiscard]] virtual MathLib::Expected<MathLib::Bitmap> Evaluate(void) const = 0;

    protected:
    MathLib::Array<CircuitElementConnection> children;
};
extern MathLib::Array<CircuitElement*> elements;

#endif