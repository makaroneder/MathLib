#ifndef TextUI_H
#define TextUI_H
#include <Interfaces/ByteDevice.hpp>
#include <SingleTypePair.hpp>

struct TextUI : MathLib::Writeable {
    TextUI(uint32_t bgColor, uint32_t fgColor);
    [[nodiscard]] size_t GetSize(void) const;
    [[nodiscard]] virtual size_t GetWidth(void) const = 0;
    [[nodiscard]] virtual size_t GetHeight(void) const = 0;
    virtual void SetPosition(const MathLib::SingleTypePair<size_t>& pos) = 0;
    [[nodiscard]] virtual MathLib::SingleTypePair<size_t> GetPosition(void) const = 0;
    [[nodiscard]] bool Clear(void);

    uint32_t bgColor;
    uint32_t fgColor;
};
extern TextUI* textUI;

#endif