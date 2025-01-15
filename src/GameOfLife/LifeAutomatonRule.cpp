#include "LifeAutomatonRule.hpp"
#include <String.hpp>
#include <Host.hpp>

LifeAutomatonRule::LifeAutomatonRule(const MathLib::Array<uint8_t>& birth, const MathLib::Array<uint8_t>& survival) : birth(birth), survival(survival) {}
LifeAutomatonRule::LifeAutomatonRule(const MathLib::String& str) {
    size_t i = 0;
    if (i >= str.GetSize() || str.At(i++) != 'B') MathLib::Panic("Failed to parse life automaton rule");
    while (i < str.GetSize() && MathLib::IsDigit(str.At(i)))
        if (!birth.Add(str.At(i++) - '0')) MathLib::Panic("Failed to parse life automaton rule");
    if (i >= str.GetSize() || str.At(i++) != '/' || str.At(i++) != 'S') MathLib::Panic("Failed to parse life automaton rule");
    while (i < str.GetSize() && MathLib::IsDigit(str.At(i)))
        if (!survival.Add(str.At(i++) - '0')) MathLib::Panic("Failed to parse life automaton rule");
}
bool LifeAutomatonRule::Apply(bool prev, uint8_t neighbours) const {
    return prev ? survival.Contains(neighbours) : birth.Contains(neighbours);
}
MathLib::String LifeAutomatonRule::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + 'B';
    for (const uint8_t& x : birth) ret += MathLib::ToString(x, 10);
    ret += "/S";
    for (const uint8_t& x : survival) ret += MathLib::ToString(x, 10);
    return ret;
}