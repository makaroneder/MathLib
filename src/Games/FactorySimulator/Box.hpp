#ifndef Box_H
#define Box_H
#include <Image/Animation.hpp>
#include <Bitmap.hpp>

struct Box;
struct Box : MathLib::Comparable<Box> {
    static constexpr size_t materialCount = 1;
    MathLib::Bitmap materials;
    MathLib::Animation machine;

    Box(void);
    bool ContainsMaterial(size_t material) const;

    protected:
    [[nodiscard]] virtual bool Equals(const Box& other) const override;
};

#endif