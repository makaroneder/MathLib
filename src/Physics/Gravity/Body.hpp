#ifndef Body_H
#define Body_H
#include "Vector2.hpp"
#include <Renderer.hpp>

struct Body : MathLib::Printable {
    static constexpr float gravitationalConstant = 1;
    Vector2 position;
    Vector2 velocity;
    float mass;

    Body(void);
    Body(const Vector2& position, const Vector2& velocity, const float& mass);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] float GetRadius(void) const;
    void Draw(MathLib::Renderer& renderer, uint32_t color) const;
    void Move(const float& dt);
    void Update(Body& other, const float& dt);
};

#endif