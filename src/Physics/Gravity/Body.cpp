#include "Body.hpp"

Body::Body(void) : position(), velocity(), mass(0) {}
Body::Body(const Vector2& position, const Vector2& velocity, const float& mass) : position(position), velocity(velocity), mass(mass) {}
MathLib::String Body::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    return "{\n\t"_M + padd + "Position: " + position.ToString() + "\n\t" + padd + "Velocity: " + velocity.ToString() + "\n\t" + padd + "Mass: " + MathLib::ToString(mass) + '\n' + padd + '}';
}
float Body::GetRadius(void) const {
    return 0.3 * mass;
}
void Body::Draw(MathLib::Renderer& renderer, uint32_t color) const {
    renderer.DrawCircle2D<float>(MathLib::CreateVector<float>(position.x, position.y, 0), GetRadius(), color);
}
void Body::Move(const float& dt) {
    position += velocity * dt;
}
void Body::Update(Body& other, const float& dt) {
    const Vector2 posDiff = position - other.position;
    const float distanceSquared = posDiff.GetLengthSquared();
    const float radius1 = GetRadius();
    const float radius2 = other.GetRadius();
    const float totalRadius = radius1 + radius2;
    if (distanceSquared <= totalRadius * totalRadius) {
        const Vector2 tmp = posDiff * (velocity - other.velocity).DotProduct(posDiff) * 2 / ((mass + other.mass) * distanceSquared);
        velocity -= tmp * other.mass;
        other.velocity += tmp * mass;
    }
    const Vector2 tmp = posDiff * gravitationalConstant / MathLib::Pow(distanceSquared, 3 / 2);
    velocity -= tmp * other.mass * dt;
    other.velocity += tmp * mass * dt;
}