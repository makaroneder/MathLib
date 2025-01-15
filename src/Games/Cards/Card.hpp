#ifndef Card_H
#define Card_H
#include <DummyRenderer.hpp>
#include <Geometry/Cuboid.hpp>

struct Card : MathLib::Printable {
    static constexpr MathLib::num_t width = 0.64;
    static constexpr MathLib::num_t height = 0.96;
    static constexpr MathLib::num_t padding = 0.1;
    enum class Type {
        C2 = 0,
        C3,
        C4,
        C5,
        C6,
        C7,
        C8,
        C9,
        C10,
        Jack,
        Queen,
        King,
        Ace,
        TypeCount,
    };
    enum class Color {
        Clubs = 0,
        Diamonds,
        Hearts,
        Spades,
        ColorCount,
    };
    MathLib::Image* image;
    bool free;

    Card(MathLib::Image* image = nullptr);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
    template <typename T>
    [[nodiscard]] static MathLib::Cuboid<T> ToCuboid(const MathLib::Matrix<T>& position) {
        return MathLib::Cuboid<T>(position, MathLib::CreateVector<T>(width, height, 0.01));
    }
    template <typename T>
    [[nodiscard]] bool Draw(MathLib::Renderer& renderer, const MathLib::Matrix<T>& position) const {
        if (!image) return false;
        renderer.DrawImage<T>(*image, position);
        return true;
    }
};

#endif