#include "LifeAutomaton.hpp"

LifeAutomaton::LifeAutomaton(size_t width, size_t height, const LifeAutomatonRule& rule) : data(width, height), rule(rule) {}
void LifeAutomaton::Set(size_t x, size_t y, bool value) {
    data.At(x, y) = value;
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
    LifeAutomaton ret = LifeAutomaton(data.GetWidth(), data.GetHeight(), rule);
    for (size_t y = 0; y < data.GetHeight(); y++)
        for (size_t x = 0; x < data.GetWidth(); x++)
            ret.data.At(x, y) = rule.Apply(data.At(x, y), GetNeighbours(x, y));
    return ret;
}
MathLib::Image LifeAutomaton::ToImage(uint32_t deadColor, uint32_t aliveColor) const {
    MathLib::Image image = MathLib::Image(data.GetWidth(), data.GetHeight());
    for (size_t y = 0; y < data.GetHeight(); y++)
        for (size_t x = 0; x < data.GetWidth(); x++)
            image.At(x, y) = data.At(x, y) ? aliveColor : deadColor;
    return image;
}
MathLib::String LifeAutomaton::ToString(const MathLib::String& padding) const {
    return rule.ToString(padding) + '\n' + data.ToString(padding);
}