#ifdef __x86_64__
#ifndef VGA_H
#define VGA_H
#include "../../TextUI.hpp"

struct VGA : TextUI {
    enum class Color {
        Black = 0,
        Blue,
        Green,
        Cyan,
        Red,
        Purple,
        Brown,
        Gray,
        DarkGray,
        LightBlue,
        LightGreen,
        LightCyan,
        LightRed,
        LightPurple,
        Yellow,
        White,
        ColorCount,
    };
    static constexpr uint32_t colors[] = {
        0x000000ff,
        0x0000aaff,
        0x00aa00ff,
        0x00aaaaff,
        0xaa0000ff,
        0xaa00aaff,
        0xaa5500ff,
        0xaaaaaaff,
        0x555555ff,
        0x5555ffff,
        0x55ff55ff,
        0x55ffffff,
        0xff5555ff,
        0xff55ffff,
        0xffff55ff,
        0xffffffff,
    };
    VGA(uint32_t bgColor, uint32_t fgColor, uint8_t* buffer, size_t width, size_t height);
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
    [[nodiscard]] virtual size_t GetWidth(void) const override;
    [[nodiscard]] virtual size_t GetHeight(void) const override;
    virtual void SetPosition(const MathLib::SingleTypePair<size_t>& pos) override;
    [[nodiscard]] virtual MathLib::SingleTypePair<size_t> GetPosition(void) const override;
    [[nodiscard]] Color ToVGAColor(uint32_t color) const;

    private:
    void Backspace(void);

    static constexpr size_t tabSize = 4;
    MathLib::SingleTypePair<size_t> position;
    uint8_t* buffer;
    size_t width;
    size_t height;
};

#endif
#endif