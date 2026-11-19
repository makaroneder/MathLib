#ifndef Block_H
#define Block_H
#include <Interfaces/Saveable.hpp>

struct Block;
struct Block : MathLib::Orderable<Block>, MathLib::Saveable {
    MathLib::String path;
    ssize_t x;
    ssize_t y;

    Block(void);
    Block(const MathLib::String& path, ssize_t x, ssize_t y);
    [[nodiscard]] virtual bool LessThanEqual(const Block& other) const override;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
};

#endif