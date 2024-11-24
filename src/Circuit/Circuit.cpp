#include "Circuit.hpp"

Circuit::Circuit(const MathLib::Array<CircuitElementConnection>& children) : CircuitElement(children) {}
MathLib::Expected<MathLib::Bitmap> Circuit::Evaluate(void) const {
    MathLib::Bitmap ret = MathLib::Bitmap(children.GetSize());
    for (size_t i = 0; i < children.GetSize(); i++) {
        const MathLib::Expected<bool> tmp = children.At(i).Evaluate();
        if (!tmp.HasValue() || !ret.Set(i, tmp.Get())) return MathLib::Expected<MathLib::Bitmap>();
    }
    return MathLib::Expected<MathLib::Bitmap>(ret);
}
MathLib::String Circuit::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "Circuit: {\n";
    for (const CircuitElementConnection& child : children) ret += child.ToString(padding + '\t') + '\n';
    return ret + padding + '}';
}