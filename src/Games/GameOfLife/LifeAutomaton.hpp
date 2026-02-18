#ifndef LifeAutomaton_H
#define LifeAutomaton_H
#include "Pattern.hpp"
#include <Renderer.hpp>

struct LifeAutomaton : MathLib::Printable {
    static constexpr const char* replicator = "B1357/S1357";
    static constexpr const char* seeds = "B2/S";
    static constexpr const char* bouncingBackGlider = "B25/S4";
    static constexpr const char* lifeWithoutDeath = "B3/S012345678";
    static constexpr const char* life = "B3/S23";
    static constexpr const char* life34 = "B34/S34";
    static constexpr const char* diamoeba = "B35678/S5678";
    static constexpr const char* rule2x2 = "B36/S125";
    static constexpr const char* highLife = "B36/S23";
    static constexpr const char* dayAndNight = "B3678/S34678";
    static constexpr const char* morley = "B368/S245";
    static constexpr const char* anneal = "B4678/S35678";

    LifeAutomaton(size_t width, size_t height, const MathLib::Sequence<char>& rule);
    [[nodiscard]] size_t GetWidth(void) const;
    [[nodiscard]] size_t GetHeight(void) const;
    void Set(size_t x, size_t y, bool value);
    void Set(const Pattern& pattern);
    [[nodiscard]] uint8_t GetNeighbours(size_t x, size_t y) const;
    [[nodiscard]] LifeAutomaton Update(void) const;
    [[nodiscard]] MathLib::Image ToImage(uint32_t deadColor, uint32_t aliveColor) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    MathLib::Matrix<uint8_t> data;
    MathLib::Array<uint8_t> birth;
    MathLib::Array<uint8_t> survival;
};

#endif