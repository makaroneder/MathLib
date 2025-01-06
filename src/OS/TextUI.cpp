#include "TextUI.hpp"
#include <Math/Vector.hpp>

TextUI* textUI = nullptr;
TextUI::TextUI(uint32_t bgColor, uint32_t fgColor) : bgColor(bgColor), fgColor(fgColor) {}
size_t TextUI::GetSize(void) const {
    return GetWidth() * GetHeight();
}
bool TextUI::Clear(void) {
    const size_t size = GetSize();
    if (!size) return true;
    char buff[size];
    for (size_t i = 0; i < size; i++) buff[i] = ' ';
    SetPosition(MathLib::SingleTypePair<size_t>(0, 0));
    if (!WriteBuffer(buff, size)) return false;
    SetPosition(MathLib::SingleTypePair<size_t>(0, 0));
    return true;
}