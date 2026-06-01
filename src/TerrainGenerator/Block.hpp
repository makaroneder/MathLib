#ifndef Block_H
#define Block_H
#include <Interfaces/Printable.hpp>

struct Block : MathLib::Printable {
    Block(void);
    Block(const MathLib::Array<uint8_t>& options);
    Block(uint8_t maxOption);
    size_t GetOptionCount(void) const;
    uint8_t Get(void) const;
    uint8_t GetRandomOption(void);
    bool Intersect(const Block& other);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    MathLib::Array<uint8_t> options;
    bool checked;
};

#endif