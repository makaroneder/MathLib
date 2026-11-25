#ifndef Bullet_H
#define Bullet_H
#include <Image/Animation.hpp>

struct Bullet;
struct Bullet : MathLib::Comparable<Bullet> {
    static constexpr float speed = 100;
    enum class Type : uint8_t {
        None,
        Up,
        Down,
        Left,
        Right,
    };
    MathLib::Animation animation;
    float x;
    float y;
    Type type;

    Bullet(void);
    Bullet(const MathLib::Video* video, float x, float y, Type type);
    [[nodiscard]] bool IsValid(void) const;
    [[nodiscard]] bool Draw(MathLib::Renderer& renderer, float offsetX, float offsetY, float deltaTime);
    [[nodiscard]] virtual bool Equals(const Bullet& other) const override;
};

#endif