#ifndef LifeAutomatonRule_H
#define LifeAutomatonRule_H
#include <Interfaces/Printable.hpp>

struct LifeAutomatonRule : MathLib::Printable {
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

    LifeAutomatonRule(const MathLib::Array<uint8_t>& birth, const MathLib::Array<uint8_t>& survival);
    LifeAutomatonRule(const MathLib::String& str);
    [[nodiscard]] bool Apply(bool prev, uint8_t neighbours) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::Array<uint8_t> birth;
    MathLib::Array<uint8_t> survival;
};

#endif