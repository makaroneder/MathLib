#ifndef DebugTextUI_H
#define DebugTextUI_H
#include "TextUI.hpp"

struct DebugTextUI : TextUI {
    DebugTextUI(void);
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
    [[nodiscard]] virtual size_t GetWidth(void) const override;
    [[nodiscard]] virtual size_t GetHeight(void) const override;
    virtual void SetPosition(const MathLib::SingleTypePair<size_t>& pos) override;
    [[nodiscard]] virtual MathLib::SingleTypePair<size_t> GetPosition(void) const override;
};

#endif