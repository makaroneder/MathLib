#ifdef __x86_64__
#include "VGA.hpp"
#include "IO.hpp"
#include <Emulator/Register.hpp>
#include <Math/Vector.hpp>
#include <Memory.hpp>

VGA::VGA(uint32_t bgColor, uint32_t fgColor, uint8_t* buffer, size_t width, size_t height) : TextUI(bgColor, fgColor), position(MathLib::SingleTypePair<size_t>(0, 0)), buffer(buffer), width(width), height(height) {}
void VGA::Backspace(void) {
    if (position.first) position.first--;
    else if (position.second) {
        position.first = width - 1;
        position.second--;
    }
}
size_t VGA::WriteSizedBuffer(const void* buffer, size_t size) {
    const uint8_t* buff8 = (const uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) {
        const size_t pos = position.second * width + position.first;
        const size_t offset = pos * 2;
        switch (buff8[i]) {
            case '\n': {
                position.first = 0;
                position.second++;
                break;
            }
            case '\r': {
                position.first = 0;
                break;
            }
            case '\b': {
                Backspace();
                if (!Write<char>(' ')) return i;
                Backspace();
                break;
            }
            case '\t': {
                uint8_t buff[tabSize - position.first % tabSize];
                for (size_t i = 0; i < SizeOfArray(buff); i++) buff[i] = ' ';
                if (WriteSizedBuffer(buff, SizeOfArray(buff)) != SizeOfArray(buff)) return i;
                break;
            }
            default: {
                if (pos < GetSize()) {
                    this->buffer[offset] = buff8[i];
                    this->buffer[offset + 1] = ((uint8_t)ToVGAColor(bgColor) << 4) | (uint8_t)ToVGAColor(fgColor);
                    position.first++;
                }
            }
        }
        if (position.first >= width) {
            position.first = 0;
            position.second++;
        }
        if (position.second >= height) {
            const size_t diff = position.second - height + 1;
            for (size_t y = diff; y < height; y++)
                MathLib::MemoryCopy(&this->buffer[y * width * 2], &this->buffer[(y - diff) * width * 2], width * 2);
            for (size_t x = 0; x < width; x++) this->buffer[((height - diff) * width + x) * 2] = ' ';
            position.second -= diff;
        }
    }
    SetPosition(position);
    return size;
}
size_t VGA::GetWidth(void) const {
    return width;
}
size_t VGA::GetHeight(void) const {
    return height;
}
void VGA::SetPosition(const MathLib::SingleTypePair<size_t>& pos) {
    position.first = pos.first;
    position.second = pos.second;
    MathLib::Register reg;
    reg.Set16(pos.second * width + pos.first, false);
    WritePort<uint8_t>(0x03d4, 0x0f);
    WritePort<uint8_t>(0x03d5, reg.Get8(false));
    WritePort<uint8_t>(0x03d4, 0x0e);
    WritePort<uint8_t>(0x03d5, reg.Get8(true));
}
MathLib::SingleTypePair<size_t> VGA::GetPosition(void) const {
    return position;
}
VGA::Color VGA::ToVGAColor(uint32_t color) const {
    uint8_t ret = (uint8_t)Color::Black;
    for (uint8_t c = (uint8_t)Color::Blue; c < (uint8_t)Color::ColorCount; c++)
        if (MathLib::Abs(color - colors[c]) < MathLib::Abs(color - colors[ret])) ret = c;
    return (Color)ret;
}

#endif