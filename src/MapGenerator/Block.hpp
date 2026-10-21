#ifndef Block_H
#define Block_H
#include <Typedefs.hpp>

struct Block;
struct Block : MathLib::Comparable<Block> {
    MathLib::String file;
    ssize_t x;
    ssize_t y;

    Block(void);
    Block(const MathLib::String& file, ssize_t x, ssize_t y);
    [[nodiscard]] virtual bool Equals(const Block& other) const override;
};

#endif