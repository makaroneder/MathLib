#include "Quest.hpp"

Quest::Quest(const String& name, const String& description, const Array<QuestStep>& steps) : name(name), description(description), steps(steps), step(0) {}
bool Quest::IsHidden(void) const {
    for (size_t i = step; i < steps.GetSize(); i++)
        if (steps.At(i).IsHidden()) return true;
    return false;
}
String Quest::ToString(const String& padding) const {
    return padding + name + '\n' + padding + description + '\n' + padding + (step < steps.GetSize() ? steps.At(step).GetObjective() : "Zadanie wykonane");
}