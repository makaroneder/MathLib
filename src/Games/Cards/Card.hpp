#ifndef Card_H
#define Card_H
#include <Geometry/Cuboid.hpp>

struct Card : MathLib::Orderable, MathLib::Printable {
    static constexpr MathLib::num_t width = 0.64;
    static constexpr MathLib::num_t height = 0.96;
    static constexpr MathLib::num_t padding = 0.1;
    enum class Type : uint8_t {
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
    enum class Color : uint8_t {
        Clubs = 0,
        Diamonds,
        Hearts,
        Spades,
        ColorCount,
    };
    Type type;
    Color color;
    bool selected;

    Card(Type type = Type::TypeCount, Color color = Color::ColorCount);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    template <typename T>
    [[nodiscard]] static MathLib::Cuboid<T> ToCuboid(const MathLib::Matrix<T>& position) {
        return MathLib::Cuboid<T>(position, MathLib::CreateVector<T>(width, height, 0.01));
    }

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;
};

#endif