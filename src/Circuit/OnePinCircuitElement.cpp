#include "OnePinCircuitElement.hpp"

OnePinCircuitElement::OnePinCircuitElement(void) {}
OnePinCircuitElement::OnePinCircuitElement(const MathLib::Array<CircuitElementConnection>& children) : CircuitElement(children) {}
MathLib::Expected<MathLib::Bitmap> OnePinCircuitElement::Evaluate(void) const {
    MathLib::Bitmap bitmap = MathLib::Bitmap(1);
    const MathLib::Expected<bool> tmp = EvaluateInternal();
    if (!tmp.HasValue() || !bitmap.Set(0, tmp.Get())) return MathLib::Expected<MathLib::Bitmap>();
    return MathLib::Expected<MathLib::Bitmap>(bitmap);
}