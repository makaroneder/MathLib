#ifndef MusicNote_H
#define MusicNote_H
#include <Physics/SIUnits.hpp>

template <typename T>
struct MusicNote : MathLib::Allocatable {
    enum class Type : size_t {
        None = 0,
        C1 = 4,
        D2 = 18,
        E3 = 32,
        C4 = 40,
        D4 = 42,
        E4 = 44,
        F4 = 45,
        G4 = 47,
        A4 = 49,
        A4B4 = 50,
        B4 = 51,
        C5 = 52,
    } type;
    MathLib::Second<T> duration;

    MusicNote(void) {}
    MusicNote(Type type, const MathLib::Second<T>& duration) : type(type), duration(duration) {}
    [[nodiscard]] MathLib::Hertz<T> GetFrequency(void) const {
        return type == Type::None ? MathLib::Hertz<T>(0) : MathLib::Hertz<T>(440) * MathLib::Pow(2, ((ssize_t)type - 49) / 12);
    }
};

#endif