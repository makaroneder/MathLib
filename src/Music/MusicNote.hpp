#ifndef MusicNote_H
#define MusicNote_H
#include <Physics/SIUnits.hpp>

template <typename T>
struct MusicNote : Allocatable {
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
    Second<T> duration;

    MusicNote(Type type, const Second<T>& duration) : type(type), duration(duration) {}
    Hertz<T> GetFrequency(void) const {
        return type == Type::None ? Hertz<T>(0) : Hertz<T>(440) * Pow(2, ((ssize_t)type - 49) / 12);
    }
};

#endif