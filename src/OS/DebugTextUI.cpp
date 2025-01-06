#include "DebugTextUI.hpp"
#include <Logger.hpp>

DebugTextUI::DebugTextUI(void) : TextUI(0, 0) {}
size_t DebugTextUI::WriteSizedBuffer(const void* buffer, size_t size) {
    return MathLib::logger->WriteSizedBuffer(buffer, size);
}
size_t DebugTextUI::GetWidth(void) const {
    return 0;
}
size_t DebugTextUI::GetHeight(void) const {
    return 0;
}
void DebugTextUI::SetPosition(const MathLib::SingleTypePair<size_t>&) {}
MathLib::SingleTypePair<size_t> DebugTextUI::GetPosition(void) const {
    return MathLib::SingleTypePair<size_t>(0, 0);
}