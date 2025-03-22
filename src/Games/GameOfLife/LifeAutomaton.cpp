#include "LifeAutomaton.hpp"

LifeAutomaton::LifeAutomaton(size_t width, size_t height, const MathLib::Sequence<char>& rule) : data(width, height) {
    size_t i = 0;
    if (i >= rule.GetSize()) MathLib::Panic("Failed to parse life automaton rule");
    bool swap = false;
    if (rule.At(i) == 'S') swap = true;
    else if (rule.At(i++) != 'B') MathLib::Panic("Failed to parse life automaton rule");
    while (i < rule.GetSize() && MathLib::IsDigit(rule.At(i)))
        if (!birth.Add(rule.At(i++) - '0')) MathLib::Panic("Failed to parse life automaton rule");
    if (i >= rule.GetSize() || rule.At(i++) != '/' || rule.At(i++) != (swap ? 'B' : 'S')) MathLib::Panic("Failed to parse life automaton rule");
    while (i < rule.GetSize() && MathLib::IsDigit(rule.At(i)))
        if (!survival.Add(rule.At(i++) - '0')) MathLib::Panic("Failed to parse life automaton rule");
}
size_t LifeAutomaton::GetWidth(void) const {
    return data.GetWidth();
}
size_t LifeAutomaton::GetHeight(void) const {
    return data.GetHeight();
}
void LifeAutomaton::Set(size_t x, size_t y, bool value) {
    data.At(x, y) = value;
}
void LifeAutomaton::Set(const Pattern& pattern) {
    const size_t cx = GetWidth() / 2;
    const size_t cy = GetHeight() / 2;
    for (size_t y = 0; y < pattern.GetHeight(); y++) {
        const MathLib::Matrix<ssize_t> position = pattern.GetPosition();
        const size_t wy = cy + y + GetY(position);
        for (size_t x = 0; x < pattern.GetWidth(); x++) Set(cx + x + GetX(position), wy, pattern.At(x, y));
    }
}
uint8_t LifeAutomaton::GetNeighbours(size_t x, size_t y) const {
    uint8_t ret = 0;
    for (ssize_t dy = -1; dy <= 1; dy++) {
        for (ssize_t dx = -1; dx <= 1; dx++) {
            if (dx || dy) {
                ssize_t wx = (ssize_t)x + dx;
                ssize_t wy = (ssize_t)y + dy;
                if (wx < 0) wx += data.GetWidth();
                if (wy < 0) wy += data.GetHeight();
                if ((size_t)wx >= data.GetWidth()) wx %= data.GetWidth();
                if ((size_t)wy >= data.GetHeight()) wy %= data.GetHeight();
                ret += data.At((size_t)wx, (size_t)wy);
            }
        }
    }
    return ret;
}
LifeAutomaton LifeAutomaton::Update(void) const {
    LifeAutomaton ret = *this;
    for (size_t y = 0; y < data.GetHeight(); y++) {
        for (size_t x = 0; x < data.GetWidth(); x++) {
            const size_t neighbours = GetNeighbours(x, y);
            ret.data.At(x, y) = data.At(x, y) ? survival.Contains(neighbours) : birth.Contains(neighbours);
        }
    }
    return ret;
}
MathLib::Image LifeAutomaton::ToImage(uint32_t deadColor, uint32_t aliveColor) const {
    MathLib::Image image = MathLib::Image(data.GetWidth(), data.GetHeight());
    for (size_t y = 0; y < data.GetHeight(); y++)
        for (size_t x = 0; x < data.GetWidth(); x++)
            image.At(x, y) = data.At(x, y) ? aliveColor : deadColor;
    return image;
}
MathLib::String LifeAutomaton::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + 'B';
    for (const uint8_t& x : birth) ret += MathLib::ToString(x, 10);
    ret += "/S";
    for (const uint8_t& x : survival) ret += MathLib::ToString(x, 10);
    return ret;
}