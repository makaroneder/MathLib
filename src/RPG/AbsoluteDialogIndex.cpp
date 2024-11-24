#include "AbsoluteDialogIndex.hpp"

AbsoluteDialogIndex::AbsoluteDialogIndex(void) : npc(SIZE_MAX), dialog(SIZE_MAX) {}
AbsoluteDialogIndex::AbsoluteDialogIndex(size_t npc, size_t dialog) : npc(npc), dialog(dialog) {}
bool AbsoluteDialogIndex::Save(MathLib::Writeable& file) const {
    return file.Write<size_t>(npc) && file.Write<size_t>(dialog);
}
bool AbsoluteDialogIndex::Load(MathLib::Readable& file) {
    return file.Read<size_t>(npc) && file.Read<size_t>(dialog);
}
MathLib::String AbsoluteDialogIndex::ToString(const MathLib::String& padding) const {
    return padding + MathLib::ToString(npc) + ": " + MathLib::ToString(dialog);
}
bool AbsoluteDialogIndex::operator==(const AbsoluteDialogIndex& other) const {
    return npc == other.npc && dialog == other.dialog;
}