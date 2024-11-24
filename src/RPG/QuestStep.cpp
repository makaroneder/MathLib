#include "QuestStep.hpp"

QuestStep::QuestStep(const MathLib::String& objective, const AbsoluteDialogIndex& dialog, bool hidden) : objective(objective), dialog(dialog), hidden(hidden) {}
bool QuestStep::IsHidden(void) const {
    return hidden;
}
MathLib::String QuestStep::GetObjective(void) const {
    return objective;
}