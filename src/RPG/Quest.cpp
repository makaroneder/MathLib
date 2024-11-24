#include "Quest.hpp"

Quest::Quest(const MathLib::String& name, const MathLib::String& description, const MathLib::Array<QuestStep>& steps) : name(name), description(description), steps(steps), step(0) {}
bool Quest::IsHidden(void) const {
    for (size_t i = step; i < steps.GetSize(); i++)
        if (steps.At(i).IsHidden()) return true;
    return false;
}
MathLib::String Quest::ToString(const MathLib::String& padding) const {
    return padding + name + '\n' + padding + description + '\n' + padding + (step < steps.GetSize() ? steps.At(step).GetObjective() : "Zadanie wykonane");
}